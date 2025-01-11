#include <serializator/Serializator.h>

#include <sstream>

using namespace sa::core;
using namespace sa::serializator;

IPosition::CPtr Serializator::fromString(const std::string& data)
{
  std::istringstream ss(data);
  std::string typeId;
  ss >> typeId;
  std::string dataRaw;
  std::getline(ss, dataRaw);
  dataRaw.erase(0, dataRaw.find_first_not_of(" \t"));
  auto it = getFromStringMap().find(typeId);
  if (it == getFromStringMap().end()) {
    return nullptr;
  }
  return it->second(dataRaw);
}

std::string Serializator::toString(const core::IPosition::CPtr& iPosition)
{
  std::string typeId = iPosition->getTypeId();
  auto it = getToStringMap().find(typeId);
  if (it == getToStringMap().end()) {
    return "";
  }
  std::stringstream ss;
  ss << typeId << " " << it->second(iPosition);
  return ss.str();
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
