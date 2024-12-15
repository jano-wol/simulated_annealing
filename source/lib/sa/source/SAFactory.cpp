#include <sa/SAFactory.h>

#include <sstream>

using namespace sa::core;
using namespace sa::sa;

SA::CPtr SAFactory::create() const
{
  return make_unique<SA>(resourcePolicy->clone(), acceptancePolicy->clone(), coolingPolicy->clone(),
                         moveSelectorPolicy->clone(), monitor->clone());
}
