#include <salesman_angle/Move.h>

using namespace sa::targets::salesman_angle;

double SalesmanAngleMove::getDelta() const { return delta; }

int SalesmanAngleMove::size() const { return 2 * sizeof(std::size_t) + sizeof(double); }
