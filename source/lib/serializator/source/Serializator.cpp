#include <serializator/Serializator.h>

#include <salesman/Position.h>

using namespace sa::core;
using namespace sa::serializator;
using namespace sa::targets::salesman;

std::string Serializator::getTypeId(const core::IPosition::CPtr& iPosition)
{
  if (dynamic_cast<SalesmanPosition*>(iPosition.get())) {
    return SalesmanPosition::getTypeId();
  } else {
    return "";
  }
}

IPosition::CPtr Serializator::fromString(const std::string& data)
{
  std::istringstream ss(data);
  std::string typeId;
  ss >> typeId;
  auto it = getFromStringMap().find(typeId);
  return it->second(data);
}

std::string Serializator::toString(const core::IPosition::CPtr& iPosition)
{
  std::string typeId = getTypeId(iPosition);
  auto it = getToStringMap().find(typeId);
  return it->second(iPosition);
}

void Serializator::registerFromStringType(const std::string& type, FromString fromString)
{
  getFromStringMap()[type] = std::move(fromString);
}

void Serializator::registerToStringType(const std::string& type, ToString toString)
{
  getToStringMap()[type] = std::move(toString);
}

std::unordered_map<std::string, Serializator::FromString>& Serializator::getFromStringMap()
{
  static std::unordered_map<std::string, FromString> fromStringMap;
  return fromStringMap;
}

std::unordered_map<std::string, Serializator::ToString>& Serializator::getToStringMap()
{
  static std::unordered_map<std::string, ToString> toStringMap;
  return toStringMap;
}

static bool initFactory = [] {
  Serializator::registerFromStringType(SalesmanPosition::getTypeId(), SalesmanPosition::fromString);
  Serializator::registerToStringType(SalesmanPosition::getTypeId(), SalesmanPosition::toString);
  return true;
}();
