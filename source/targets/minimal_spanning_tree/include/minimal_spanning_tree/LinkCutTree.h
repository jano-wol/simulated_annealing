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
  LinkCutTree(int n) : nodes(n, nullptr) {}

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
  }

  std::vector<int> getPath(int u, int v)
  {
    makeRoot(nodes[u]);
    access(nodes[v]);
    std::vector<int> path;
    Node* current = nodes[v];
    while (current) {
      path.push_back(current->value);
      current = current->right;
    }
    return path;
  }

  bool isTreeEdge(int u, int v)
  {
    makeRoot(nodes[u]);
    access(nodes[v]);
    return nodes[v]->left == nodes[u] || nodes[u]->parent == nodes[v];
  }

  std::vector<std::pair<int, int>> getEdges()
  {
    std::vector<std::pair<int, int>> edges;
    for (Node* node : nodes) {
      if (node && node->parent) {
        edges.push_back({node->value, node->parent->value});
      }
    }

    return edges;
  }

  int size() { return sizeof(std::vector<Node*>) + Node::size() * nodes.capacity(); }

  LinkCutTree::CPtr clone()
  {
    LinkCutTree::CPtr clonedTree = std::make_unique<LinkCutTree>(nodes.size());
    std::unordered_map<Node*, Node*> originalToCloneMap;
    for (Node* originalNode : nodes) {
      if (originalNode) {
        clonedTree->addNode(originalNode->value);
        Node* clonedNode = new Node(originalNode->value);
        clonedNode->isReversed = originalNode->isReversed;
        originalToCloneMap[originalNode] = clonedNode;
      }
    }
    for (Node* originalNode : nodes) {
      if (originalNode && originalNode->parent) {
        Node* originalParent = originalNode->parent;
        Node* clonedNode = originalToCloneMap[originalNode];
        Node* clonedParent = originalToCloneMap[originalParent];
        clonedNode->parent = clonedParent;
      }
    }
    for (Node* originalNode : nodes) {
      if (originalNode) {
        Node* clonedNode = originalToCloneMap[originalNode];
        if (originalNode->left) {
          clonedNode->left = originalToCloneMap[originalNode->left];
        }
        if (originalNode->right) {
          clonedNode->right = originalToCloneMap[originalNode->right];
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
