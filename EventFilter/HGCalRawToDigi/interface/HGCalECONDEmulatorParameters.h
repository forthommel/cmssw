#ifndef EventFilter_HGCalRawToDigi_HGCalECONDEmulatorParameters_h
#define EventFilter_HGCalRawToDigi_HGCalECONDEmulatorParameters_h

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include <vector>

namespace hgcal::econd {
  struct EmulatorParameters {
    explicit EmulatorParameters(const edm::ParameterSet& iConfig)
        : chan_surv_prob(iConfig.getParameter<double>("channelSurv")),
          active(iConfig.getParameter<bool>("active")),
          passthrough_mode(iConfig.getParameter<bool>("passthroughMode")),
          expected_mode(iConfig.getParameter<bool>("expectedMode")),
          characterisation_mode(iConfig.getParameter<bool>("characterisationMode")),
          matching_ebo_numbers(iConfig.getParameter<bool>("matchingEBOnumbers")),
          bo_truncated(iConfig.getParameter<bool>("bufferOverflowTruncated")),
          enabled_erxs(iConfig.getParameter<std::vector<unsigned int> >("enabledERxs")),
          header_marker(iConfig.getParameter<unsigned int>("headerMarker")),
          num_channels_per_erx(iConfig.getParameter<unsigned int>("numChannelsPerERx")),
          programmable_pattern(iConfig.getParameter<unsigned int>("programmablePattern")),
          error_prob({.bitO = iConfig.getParameter<double>("bitOError"),
                      .bitB = iConfig.getParameter<double>("bitBError"),
                      .bitE = iConfig.getParameter<double>("bitEError"),
                      .bitT = iConfig.getParameter<double>("bitTError"),
                      .bitH = iConfig.getParameter<double>("bitHError"),
                      .bitS = iConfig.getParameter<double>("bitSError")}) {}

    static edm::ParameterSetDescription description() {
      edm::ParameterSetDescription desc;
      desc.add<double>("channelSurv", 1.);
      desc.add<bool>("active", true)->setComment("is the ECON-D activated?");
      desc.add<bool>("passthroughMode", false)->setComment("ECON-D in pass-through mode?");
      desc.add<bool>("expectedMode", false)
          ->setComment("is an Event HDR/TRL expected to be received from the HGCROCs?");
      desc.add<bool>("characterisationMode", false);
      desc.add<bool>("matchingEBOnumbers", true)
          ->setComment(
              "is the transmitted E/B/O (according to mode selected by user) matching the E/B/O value in the ECON-D "
              "L1A FIFO?");
      desc.add<bool>("bufferOverflowTruncated", false)->setComment("is the packet truncated for buffer overflow?");
      {  // list the enabled eRxs in all ECON-Ds
        const unsigned int max_erxs_per_econd = 12;
        std::vector<unsigned int> default_enabled_erxs;
        for (size_t i = 0; i < max_erxs_per_econd; ++i)
          default_enabled_erxs.emplace_back(i);
        desc.add<std::vector<unsigned int> >("enabledERxs", default_enabled_erxs)
            ->setComment("list of channels to be enabled in readout");
      }
      desc.add<unsigned int>("headerMarker", 0x154)->setComment("9b programmable pattern; default is '0xAA' + '0b0'");
      desc.add<unsigned int>("numChannelsPerERx", 37)->setComment("number of channels managed in each ECON-D eRx");
      desc.add<unsigned int>("programmablePattern", 0xa5a5a5)
          ->setComment("a 24b programmable pattern used by backend to find event packet");
      desc.add<double>("bitOError", 0.)->setComment("probability that the bit-O error is set");
      desc.add<double>("bitBError", 0.)->setComment("probability that the bit-B error is set");
      desc.add<double>("bitEError", 0.)->setComment("probability that the bit-E error is set");
      desc.add<double>("bitTError", 0.)->setComment("probability that the bit-T error is set");
      desc.add<double>("bitHError", 0.)->setComment("probability that the bit-H error is set");
      desc.add<double>("bitSError", 0.)->setComment("probability that the bit-S error is set");
      return desc;
    }

    const double chan_surv_prob{1.};
    const bool active{true};
    const bool passthrough_mode{false};
    const bool expected_mode{false};
    const bool characterisation_mode{false};
    const bool matching_ebo_numbers{true};
    const bool bo_truncated{false};
    const std::vector<unsigned int> enabled_erxs{};
    const unsigned int header_marker{0};
    const unsigned int num_channels_per_erx{0};
    const unsigned int programmable_pattern{0};
    struct ErrorProbabilities {
      double bitO{0.}, bitB{0.}, bitE{0.}, bitT{0.}, bitH{0.}, bitS{0.};
    };
    const ErrorProbabilities error_prob;
  };
}  // namespace hgcal::econd

#endif
