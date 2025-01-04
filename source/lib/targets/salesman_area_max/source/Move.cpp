#include <salesman_area_max/Move.h>

using namespace sa::targets::salesman_area_max;

double SalesmanAreaMaxMove::getDelta() const { return delta; }

int SalesmanAreaMaxMove::size() const { return 2 * sizeof(std::size_t); }
