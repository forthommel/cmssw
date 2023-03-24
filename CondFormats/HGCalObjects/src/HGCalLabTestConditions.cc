/****************************************************************************
 *
 * This is a part of HGCal offline software.
 * Authors:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include <algorithm>

#include "CondFormats/HGCalObjects/interface/HGCalLabTestConditions.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

HGCalLabTestConditions::HGCalLabTestConditions() {}

HGCalLabTestConditions::~HGCalLabTestConditions() {}

std::vector<std::string> HGCalLabTestConditions::keys() const {
  std::vector<std::string> keys;
  std::transform(params_.begin(), params_.end(), std::back_inserter(keys), [](const auto& pair) { return pair.first; });
  std::sort(keys.begin(), keys.end());
  return keys;
}

const std::vector<int>& HGCalLabTestConditions::parameters(const std::string& key) const {
  if (params_.count(key) == 0)
    throw cms::Exception("HGCalLabTestConditions") << "Failed to retrieve a parameter with key='" << key << "'.";
  return params_.at(key);
}

HGCalLabTestConditions& HGCalLabTestConditions::addParameter(const std::string& key, const std::vector<int>& values) {
  if (params_.count(key) != 0)
    edm::LogWarning("HGCalLabTestConditions")
        << "Parameter with key='" << key << "' was already present in collection.";
  params_[key] = values;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const HGCalLabTestConditions& cond) {
  os << "HGCalLabTestConditions{";
  std::string sep1;
  for (const auto& key : cond.keys()) {
    os << sep1 << key << ": {";
    std::string sep2;
    for (const auto& param : cond.parameters(key))
      os << sep2 << param, sep2 = ", ";
    os << "}";
    sep1 = ", ";
  }
  return os << "}";
}
