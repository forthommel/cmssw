#include "CondFormats/HGCalObjects/interface/HGCalLabTestConditions.h"
#include "CondFormats/Serialization/interface/Test.h"

int main() {
  testSerialization<HGCalLabTestConditions>();
  return 0;
}
