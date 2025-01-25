#include <minimal_spanning_tree/Move.h>

using namespace sa::targets::minimal_spanning_tree;

double MinimalSpanningTreeMove::getDelta() const { return delta; }

int MinimalSpanningTreeMove::size() const { return 3 * sizeof(int) + sizeof(double); }
