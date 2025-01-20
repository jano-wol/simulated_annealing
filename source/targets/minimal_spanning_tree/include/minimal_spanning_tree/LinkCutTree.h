#ifndef SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_
#define SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_

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
    int value;
    Node* left;
    Node* right;
    Node* parent;
    bool isReversed;

    Node(int v) : value(v), left(nullptr), right(nullptr), parent(nullptr), isReversed(false) {}
    static int size() { return sizeof(int) + sizeof(bool) + 3 * sizeof(Node*); }
  };

  std::vector<Node*> nodes;

  // Helper functions
  void push(Node* x)
  {
    if (x && x->isReversed) {
      std::swap(x->left, x->right);
      if (x->left)
        x->left->isReversed ^= true;
      if (x->right)
        x->right->isReversed ^= true;
      x->isReversed = false;
    }
  }

  void rotate(Node* x)
  {
    Node* p = x->parent;
    Node* g = p ? p->parent : nullptr;

    if (p) {
      push(p);
      push(x);

      if (x == p->left) {
        p->left = x->right;
        if (x->right)
          x->right->parent = p;
        x->right = p;
      } else {
        p->right = x->left;
        if (x->left)
          x->left->parent = p;
        x->left = p;
      }
      p->parent = x;
    }

    x->parent = g;
    if (g) {
      if (g->left == p)
        g->left = x;
      else if (g->right == p)
        g->right = x;
    }
  }

  void splay(Node* x)
  {
    while (x->parent) {
      Node* p = x->parent;
      Node* g = p->parent;
      if (!g) {
        rotate(x);
      } else if ((g->left == p) == (p->left == x)) {
        rotate(p);
        rotate(x);
      } else {
        rotate(x);
        rotate(x);
      }
    }
    push(x);
  }

  Node* access(Node* x)
  {
    Node* last = nullptr;
    for (Node* y = x; y; y = y->parent) {
      splay(y);
      y->right = last;
      last = y;
    }
    splay(x);
    return last;
  }

  void makeRoot(Node* x)
  {
    access(x);
    x->isReversed ^= true;
    push(x);
  }

public:
  using CPtr = std::unique_ptr<LinkCutTree>;
  LinkCutTree(int n) : nodes(n, nullptr)
  {
    for (int i = 0; i < n; ++i) {
      addNode(i);
    }
    validate();
  }

  void addNode(int value)
  {
    if (value >= int(nodes.size())) {
      nodes.resize(value + 1, nullptr);
    }
    if (!nodes[value]) {
      nodes[value] = new Node(value);
    }
  }

  void link(int u, int v)
  {
    Node* x = nodes[u];
    Node* y = nodes[v];
    makeRoot(x);
    x->parent = y;
    validate();
  }

  void cut(int u, int v)
  {
    Node* x = nodes[u];
    Node* y = nodes[v];
    makeRoot(x);
    access(y);
    if (y->left == x) {
      y->left = nullptr;
      x->parent = nullptr;
    }
    validate();
  }

  void collectPath(Node* x, std::vector<int>& path)
  {
    if (!x)
      return;

    push(x);
    collectPath(x->left, path);
    path.push_back(x->value);
    collectPath(x->right, path);
    validate();
  }

  void validate()
  {
    for (auto node : nodes) {
      if (node->parent == node) {
        exit(1);
      }
    }
  }

  std::vector<int> getPath(int u, int v)
  {
    makeRoot(nodes[u]);
    access(nodes[v]);
    std::vector<int> path;
    collectPath(nodes[v], path);
    validate();
    return path;
  }

  bool isTreeEdge(int u, int v)
  {
    if ((nodes[u]->parent) && (nodes[u]->parent->value == v)) {
      return true;
    }
    if ((nodes[v]->parent) && (nodes[v]->parent->value == u)) {
      return true;
    }
    validate();
    return false;
  }

  std::vector<std::pair<int, int>> getEdges()
  {
    std::vector<std::pair<int, int>> edges;
    for (Node* node : nodes) {
      if (node && node->parent) {
        edges.push_back({node->value, node->parent->value});
      }
    }
    validate();
    return edges;
  }

  int size() { return sizeof(std::vector<Node*>) + Node::size() * nodes.capacity(); }

  LinkCutTree::CPtr clone()
  {
    LinkCutTree::CPtr clonedTree = std::make_unique<LinkCutTree>(nodes.size());
    for (int i = 0; i < int(nodes.size()); ++i) {
      const auto& originalNode = nodes[i];
      if (originalNode) {
        clonedTree->nodes[i]->value = originalNode->value;
        clonedTree->nodes[i]->isReversed = originalNode->isReversed;
        if (originalNode->left) {
          int l = originalNode->left->value;
          clonedTree->nodes[i]->left = clonedTree->nodes[l];
        } else {
          clonedTree->nodes[i]->left = nullptr;
        }
        if (originalNode->right) {
          int r = originalNode->right->value;
          clonedTree->nodes[i]->right = clonedTree->nodes[r];
        } else {
          clonedTree->nodes[i]->right = nullptr;
        }
        if (originalNode->parent) {
          int p = originalNode->parent->value;
          clonedTree->nodes[i]->parent = clonedTree->nodes[p];
        } else {
          clonedTree->nodes[i]->parent = nullptr;
        }
      }
    }
    return clonedTree;
  }

  ~LinkCutTree()
  {
    for (Node* node : nodes) {
      delete node;
    }
  }
};
}  // namespace sa::targets::minimal_spanning_tree

#endif  // SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_
