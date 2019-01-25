#include "DataFormats/Common/interface/Wrapper.h"

//Add includes for your classes here
#include "DataFormats/DateDigi/interface/DateEvent.h"
#include <vector>

namespace FWCore_Skeletons {
  struct DataFormats_DateDigi {
    date::Event de;
    edm::Wrapper<date::Event> wrp_de;
    std::vector<date::Event> vec_de;
    edm::Wrapper<std::vector<date::Event> > wrp_vec_de;
/*
    These classes are commented out because they are used more rarely. If you need them, move them
    outside the comments and make the corresponding change in classes_def.xml

uncomment_h_here

      edm::Ref<std::vector<YOUR_CLASS_GOES_HERE> > dummy4;
      edm::RefVector<std::vector<YOUR_CLASS_GOES_HERE> > dummy5;
      edm::RefProd<std::vector<YOUR_CLASS_GOES_HERE> > dummy6;
*/

   };
}
