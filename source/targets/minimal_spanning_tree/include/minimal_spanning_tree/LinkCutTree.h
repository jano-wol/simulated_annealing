#ifndef SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_
#define SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_

#include <algorithm>
#include <memory>
#include <vector>

namespace sa::targets::minimal_spanning_tree
{
class LinkCutTree
{
public:
  using CPtr = std::unique_ptr<LinkCutTree>;

  LinkCutTree(int n) : node(n) {}

  void link(int u, int v)
  {
    makeRoot(&node[u]);
    node[u].pp = &node[v];
    edges.push_back({std::min(u, v), std::max(u, v)});
  }

  void cut(int edgeIdx)
  {
    auto [u, v] = edges[edgeIdx];
    Node *x = &node[u], *top = &node[v];
    makeRoot(top);
    x->splay();
    if (x->pp)
      x->pp = 0;
    else {
      x->c[0] = top->p = 0;
      x->fix();
    }
    std::swap(edges[edgeIdx], edges.back());
    edges.pop_back();
  }

  bool connected(int u, int v)
  {
    Node* nu = access(&node[u])->first();
    return nu == access(&node[v])->first();
  }

  std::vector<std::pair<int, int>>& getEdges() { return edges; }

  int size() const
  {
    int totalSize = sizeof(*this);
    totalSize += int(sizeof(Node) * node.capacity());
    return totalSize;
  }

  LinkCutTree::CPtr clone() const
  {
    LinkCutTree::CPtr newTree = std::make_unique<LinkCutTree>(int(node.size()));
    for (size_t i = 0; i < node.size(); ++i) {
      const Node& oldNode = node[i];
      Node& newNode = newTree->node[i];
      newNode.flip = oldNode.flip;
      newNode.p = oldNode.p ? &newTree->node[oldNode.p - &node[0]] : nullptr;
      newNode.pp = oldNode.pp ? &newTree->node[oldNode.pp - &node[0]] : nullptr;
      newNode.c[0] = oldNode.c[0] ? &newTree->node[oldNode.c[0] - &node[0]] : nullptr;
      newNode.c[1] = oldNode.c[1] ? &newTree->node[oldNode.c[1] - &node[0]] : nullptr;
    }
    newTree->edges = edges;
    return newTree;
  }

private:
  struct Node
  {
    Node *p = 0, *pp = 0, *c[2];
    bool flip = 0;
    Node()
    {
      c[0] = c[1] = 0;
      fix();
    }
    void fix()
    {
      if (c[0])
        c[0]->p = this;
      if (c[1])
        c[1]->p = this;
    }
    void pushFlip()
    {
      if (!flip)
        return;
      flip = 0;
      std::swap(c[0], c[1]);
      if (c[0])
        c[0]->flip ^= 1;
      if (c[1])
        c[1]->flip ^= 1;
    }
    int up() { return p ? p->c[1] == this : -1; }
    void rot(int i, int b)
    {
      int h = i ^ b;
      Node *x = c[i], *y = b == 2 ? x : x->c[h], *z = b ? y : x;
      if ((y->p = p))
        p->c[up()] = y;
      c[i] = z->c[i ^ 1];
      if (b < 2) {
        x->c[h] = y->c[h ^ 1];
        y->c[h ^ 1] = x;
      }
      z->c[i ^ 1] = this;
      fix();
      x->fix();
      y->fix();
      if (p)
        p->fix();
      std::swap(pp, y->pp);
    }
    void splay()
    {
      for (pushFlip(); p;) {
        if (p->p)
          p->p->pushFlip();
        p->pushFlip();
        pushFlip();
        int c1 = up(), c2 = p->up();
        if (c2 == -1)
          p->rot(c1, 2);
        else
          p->p->rot(c2, c1 != c2);
      }
    }
    Node* first()
    {
      pushFlip();
      return c[0] ? c[0]->first() : (splay(), this);
    }
  };

  void makeRoot(Node* u)
  {
    access(u);
    u->splay();
    if (u->c[0]) {
      u->c[0]->p = 0;
      u->c[0]->flip ^= 1;
      u->c[0]->pp = u;
      u->c[0] = 0;
      u->fix();
    }
  }
  Node* access(Node* u)
  {
    u->splay();
    while (Node* pp = u->pp) {
      pp->splay();
      u->pp = 0;
      if (pp->c[1]) {
        pp->c[1]->p = 0;
        pp->c[1]->pp = pp;
      }
      pp->c[1] = u;
      pp->fix();
      u = pp;
    }
    return u;
  }

  std::vector<Node> node;
  std::vector<std::pair<int, int>> edges;
};

}  // namespace sa::targets::minimal_spanning_tree

#endif  // SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_
