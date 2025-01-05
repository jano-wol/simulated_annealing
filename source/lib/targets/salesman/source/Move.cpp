#include <salesman/Move.h>

using namespace sa::targets::salesman;

double SalesmanMove::getDelta() const { return delta; }

int SalesmanMove::size() const { return 2 * sizeof(std::size_t) + sizeof(double); }
