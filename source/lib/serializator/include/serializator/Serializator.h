#ifndef SIMULATED_ANNEALING_SERIALIZATOR_SERIALIZATOR_H_
#define SIMULATED_ANNEALING_SERIALIZATOR_SERIALIZATOR_H_

#include <functional>
#include <string>
#include <unordered_map>

#include <core/IPosition.h>

#define REGISTER_TARGET_BY_ID(target)                                                  \
  static std::string getTypeIdStatic() { return #target; }                             \
  std::string getTypeId() const override { return getTypeIdStatic(); }                 \
  inline static const bool registered = []() {                                         \
    serializator::Serializator::registerFromStringType(getTypeIdStatic(), fromString); \
    serializator::Serializator::registerToStringType(getTypeIdStatic(), toString);     \
    return true;                                                                       \
  }();

namespace sa::serializator
{
class Serializator
{
public:
  using FromString = std::function<core::IPosition::CPtr(const std::string&)>;
  using ToString = std::function<std::string(const core::IPosition::CPtr&)>;

  static core::IPosition::CPtr fromString(const std::string& data);
  static std::string toString(const core::IPosition::CPtr& iPosition);
  static void registerFromStringType(const std::string& type, FromString fromString);
  static void registerToStringType(const std::string& type, ToString toString);

private:
  static std::unordered_map<std::string, FromString>& getFromStringMap();
  static std::unordered_map<std::string, ToString>& getToStringMap();
};
}  // namespace sa::serializator

#endif  // SIMULATED_ANNEALING_SERIALIZATOR_SERIALIZATOR_H_
