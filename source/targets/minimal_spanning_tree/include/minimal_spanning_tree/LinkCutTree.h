#ifndef SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_
#define SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

namespace sa::targets::minimal_spanning_tree
{
class LinkCutTree
{
private:
  struct Node
  {
    int sz, label;
    Node *p, *pp, *l, *r;
    Node() { p = pp = l = r = nullptr; }
    static int size() { return 2 * sizeof(int) + 4 * sizeof(Node*); }
  };

  void update(Node* x)
  {
    x->sz = 1;
    if (x->l)
      x->sz += x->l->sz;
    if (x->r)
      x->sz += x->r->sz;
  }

  void rotr(Node* x)
  {
    Node *y, *z;
    y = x->p, z = y->p;
    if ((y->l = x->r))
      y->l->p = y;
    x->r = y, y->p = x;
    if ((x->p = z)) {
      if (y == z->l)
        z->l = x;
      else
        z->r = x;
    }
    x->pp = y->pp;
    y->pp = 0;
    update(y);
  }

  void rotl(Node* x)
  {
    Node *y, *z;
    y = x->p, z = y->p;
    if ((y->r = x->l))
      y->r->p = y;
    x->l = y, y->p = x;
    if ((x->p = z)) {
      if (y == z->l)
        z->l = x;
      else
        z->r = x;
    }
    x->pp = y->pp;
    y->pp = 0;
    update(y);
  }

  void splay(Node* x)
  {
    Node *y, *z;
    while (x->p) {
      y = x->p;
      if (y->p == 0) {
        if (x == y->l)
          rotr(x);
        else
          rotl(x);
      } else {
        z = y->p;
        if (y == z->l) {
          if (x == y->l)
            rotr(y), rotr(x);
          else
            rotl(x), rotr(x);
        } else {
          if (x == y->r)
            rotl(y), rotl(x);
          else
            rotr(x), rotl(x);
        }
      }
    }
    update(x);
  }

  Node* access(Node* x)
  {
    splay(x);
    if (x->r) {
      x->r->pp = x;
      x->r->p = 0;
      x->r = 0;
      update(x);
    }

    Node* last = x;
    while (x->pp) {
      Node* y = x->pp;
      last = y;
      splay(y);
      if (y->r) {
        y->r->pp = y;
        y->r->p = 0;
      }
      y->r = x;
      x->p = y;
      x->pp = 0;
      update(y);
      splay(x);
    }
    return last;
  }

  Node* rootN(Node* x)
  {
    access(x);
    while (x->l) x = x->l;
    splay(x);
    return x;
  }

  void cutN(Node* x)
  {
    access(x);
    x->l->p = 0;
    x->l = 0;
    update(x);
  }

  void linkN(Node* x, Node* y)
  {
    access(x);
    access(y);
    x->l = y;
    y->p = x;
    update(x);
  }

  Node* lcaN(Node* x, Node* y)
  {
    access(x);
    return access(y);
  }

  int depthN(Node* x)
  {
    access(x);
    return x->sz - 1;
  }

  Node* nodes;

public:
  using CPtr = std::unique_ptr<LinkCutTree>;
  int n = 0;
  LinkCutTree(int n_)
  {
    n = n_;
    nodes = new Node[n];
    for (int i = 0; i < n; i++) {
      nodes[i].label = i;
      update(&nodes[i]);
    }
  }

  ~LinkCutTree() { delete[] nodes; }

  void link(int u, int v) { linkN(&nodes[u], &nodes[v]); }

  void cut(int u) { cutN(&nodes[u]); }

  void cut(int u, int v)
  {
    if (isTreeEdge(u, v)) {
      auto w = lca(u, v);
      if (w == u) {
        cut(v);
      } else {
        cut(u);
      }
    }
  }

  int root(int u) { return rootN(&nodes[u])->label; }

  int depth(int u) { return depthN(&nodes[u]); }

  int lca(int u, int v) { return lcaN(&nodes[u], &nodes[v])->label; }

  std::vector<int> getPath(int u, int v)
  {
    access(&nodes[u]);
    access(&nodes[v]);
    auto w = lca(u, v);
    std::vector<int> uPath;
    std::vector<int> vPath;
    int curr = u;
    while (curr != w) {
      uPath.push_back(curr);
      curr = nodes[curr].p->label;
    }
    uPath.push_back(w);
    curr = v;
    while (curr != w) {
      vPath.push_back(curr);
      curr = nodes[curr].p->label;
    }
    std::vector<int> reversedVPath = vPath;
    std::reverse(reversedVPath.begin(), reversedVPath.end());
    uPath.insert(uPath.end(), reversedVPath.begin(), reversedVPath.end());
    return uPath;
  }

  bool isTreeEdge(int u, int v)
  {
    access(&nodes[u]);
    access(&nodes[v]);
    auto w = lca(u, v);
    return (u == w) || (v == w);
  }

  std::vector<std::pair<int, int>> getEdges()
  {
    std::vector<std::pair<int, int>> edges;
    for (int i = 0; i < n; ++i) {
      access(&nodes[i]);
      Node* node = &nodes[i];
      if (node && node->p) {
        edges.push_back({node->label, node->p->label});
      }
    }
    return edges;
  }

  int size() { return sizeof(int) + Node::size() * n; }

  LinkCutTree::CPtr clone()
  {
    LinkCutTree::CPtr clonedTree = std::make_unique<LinkCutTree>(n);
    for (int i = 0; i < n; ++i) {
      auto& originalNode = nodes[i];
      clonedTree->nodes[i].sz = originalNode.sz;
      clonedTree->nodes[i].label = originalNode.label;
      if (originalNode.l) {
        int l = originalNode.l->label;
        clonedTree->nodes[i].l = &clonedTree->nodes[l];
      } else {
        clonedTree->nodes[i].l = nullptr;
      }
      if (originalNode.r) {
        int r = originalNode.r->label;
        clonedTree->nodes[i].r = &clonedTree->nodes[r];
      } else {
        clonedTree->nodes[i].r = nullptr;
      }
      if (originalNode.p) {
        int p = originalNode.p->label;
        clonedTree->nodes[i].p = &clonedTree->nodes[p];
      } else {
        clonedTree->nodes[i].p = nullptr;
      }
      if (originalNode.pp) {
        int pp = originalNode.pp->label;
        clonedTree->nodes[i].pp = &clonedTree->nodes[pp];
      } else {
        clonedTree->nodes[i].pp = nullptr;
      }
    }
    return clonedTree;
  }
};
}  // namespace sa::targets::minimal_spanning_tree

#endif  // SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_
