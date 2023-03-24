/****************************************************************************
 *
 * This is a part of HGCal offline software.
 * Authors:
 *   Laurent Forthomme
 *
 ****************************************************************************/

#ifndef CondFormats_HGCalObjects_HGCalLabTestConditions_h
#define CondFormats_HGCalObjects_HGCalLabTestConditions_h

#include <string>
#include <map>
#include <vector>

#include "CondFormats/Serialization/interface/Serializable.h"

class HGCalLabTestConditions {
public:
  HGCalLabTestConditions();
  ~HGCalLabTestConditions();

  std::vector<std::string> keys() const;
  const std::vector<int>& parameters(const std::string&) const;

  HGCalLabTestConditions& addParameter(const std::string&, const std::vector<int>&);
  inline HGCalLabTestConditions& addParameter(const std::string& key, int value) {
    return addParameter(key, std::vector<int>{value});
  }

  friend std::ostream& operator<<(std::ostream&, const HGCalLabTestConditions&);

private:
  std::map<std::string, std::vector<int> > params_;

  COND_SERIALIZABLE;
};

#endif
