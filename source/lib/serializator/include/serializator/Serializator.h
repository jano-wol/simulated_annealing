#ifndef SIMULATED_ANNEALING_SERIALIZATOR_SERIALIZATOR_H_
#define SIMULATED_ANNEALING_SERIALIZATOR_SERIALIZATOR_H_

#include <functional>
#include <string>

#include <core/IPosition.h>

namespace sa::serializator
{
class Serializator
{
public:
  using FromString = std::function<core::IPosition::CPtr(const std::string&)>;
  using ToString = std::function<std::string(const core::IPosition::CPtr&)>;

  static std::string getTypeId(const core::IPosition::CPtr& iPosition);
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
