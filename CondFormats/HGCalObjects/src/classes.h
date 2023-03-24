#include "CondFormats/HGCalObjects/src/headers.h"

namespace CondFormats_HGCalObjects {
  struct dictionary {
    std::vector<int> v_i;
    std::pair<std::string, std::vector<int> > p_s_v_i;
    std::map<std::string, std::vector<int> > m_s_v_i;
    HGCalLabTestConditions h_ltc();
  };
}  // namespace CondFormats_HGCalObjects
