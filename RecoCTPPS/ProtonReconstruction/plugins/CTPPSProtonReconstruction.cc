/****************************************************************************
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 ****************************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESWatcher.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/ProtonReco/interface/ProtonTrack.h"
#include "DataFormats/ProtonReco/interface/ProtonTrackExtra.h"

#include "RecoCTPPS/ProtonReconstruction/interface/ProtonReconstructionAlgorithm.h"

#include "CondFormats/RunInfo/interface/LHCInfo.h"
#include "CondFormats/DataRecord/interface/LHCInfoRcd.h"

#include "CondFormats/DataRecord/interface/CTPPSOpticsRcd.h"
#include "CondFormats/CTPPSReadoutObjects/interface/LHCOpticalFunctionsCollection.h"

//----------------------------------------------------------------------------------------------------

class CTPPSProtonReconstruction : public edm::stream::EDProducer<>
{
  public:
    explicit CTPPSProtonReconstruction(const edm::ParameterSet&);
    ~CTPPSProtonReconstruction() {}

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void produce(edm::Event&, const edm::EventSetup&) override;

    edm::EDGetTokenT<CTPPSLocalTrackLiteCollection> tracksToken_;

    unsigned int verbosity_;

    bool doSingleRPReconstruction_;
    bool doMultiRPReconstruction_;

    ProtonReconstructionAlgorithm algorithm_;

    edm::ESWatcher<LHCInfoRcd> lhcInfoWatcher_;
    float currentCrossingAngle_;

    std::unordered_map<unsigned int, LHCOpticalFunctionsSet> opticalFunctions_;

    const std::string singleRPLabel = "singleRP";
    const std::string multiRPLabel = "multiRP";
};

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

CTPPSProtonReconstruction::CTPPSProtonReconstruction(const edm::ParameterSet& iConfig) :
  tracksToken_(consumes<CTPPSLocalTrackLiteCollection>(iConfig.getParameter<edm::InputTag>("tagLocalTrackLite"))),
  verbosity_(iConfig.getUntrackedParameter<unsigned int>("verbosity", 0)),
  doSingleRPReconstruction_(iConfig.getParameter<bool>("doSingleRPReconstruction")),
  doMultiRPReconstruction_(iConfig.getParameter<bool>("doMultiRPReconstruction")),
  algorithm_(iConfig.getParameter<bool>("fitVtxY"), verbosity_),
  currentCrossingAngle_(-1.)
{
  if (doSingleRPReconstruction_) {
    produces<reco::ProtonTrackCollection>(singleRPLabel);
    produces<reco::ProtonTrackExtraCollection>(singleRPLabel);
  }

  if (doMultiRPReconstruction_) {
    produces<reco::ProtonTrackCollection>(multiRPLabel);
    produces<reco::ProtonTrackExtraCollection>(multiRPLabel);
  }
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstruction::fillDescriptions(ConfigurationDescriptions& descriptions)
{
  ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstruction::produce(Event& event, const EventSetup &eventSetup)
{
  if (verbosity_)
    printf("\n---------- %u:%llu ----------\n", event.id().run(), event.id().event());

  // get conditions
  edm::ESHandle<LHCInfo> hLHCInfo;
  eventSetup.get<LHCInfoRcd>().get(hLHCInfo);

  edm::ESHandle<LHCOpticalFunctionsCollection> hOpticalFunctionCollection;
  eventSetup.get<CTPPSOpticsRcd>().get(hOpticalFunctionCollection);

  // re-initialise algorithm upon crossing-angle change
  if (lhcInfoWatcher_.check(eventSetup))
  {
    const LHCInfo* pLHCInfo = hLHCInfo.product();
    if (pLHCInfo->crossingAngle() != currentCrossingAngle_)
    {
      currentCrossingAngle_ = pLHCInfo->crossingAngle();

      if (currentCrossingAngle_ == 0.)
      {
        LogWarning("CTPPSProtonReconstruction") << "Invalid crossing angle, reconstruction disabled.";
        algorithm_.release();
      }

      if (verbosity_)
        printf("CTPPSProtonReconstruction::produce >> Setting crossing angle %.1f\n", currentCrossingAngle_);

      // interpolate optical functions
      opticalFunctions_.clear();
      hOpticalFunctionCollection->interpolateFunctions(currentCrossingAngle_, opticalFunctions_);
      for (auto &p : opticalFunctions_)
        p.second.initializeSplines();

      // reinitialise algorithm
      algorithm_.init(opticalFunctions_);
    }
  }

  // get input
  Handle<CTPPSLocalTrackLiteCollection> hTracks;
  event.getByToken(tracksToken_, hTracks);

  // keep only tracks from tracker RPs, split them by LHC sector
  ProtonReconstructionAlgorithm::TracksCollection tracks_45, tracks_56;
  map<unsigned int, unsigned int> nTracksPerRP;
  for (size_t i = 0; i < hTracks->size(); ++i) {
    const auto& tr = hTracks->at(i);
    CTPPSDetId rpId(tr.getRPId());
    if (rpId.subdetId() != CTPPSDetId::sdTrackingStrip
     && rpId.subdetId() != CTPPSDetId::sdTrackingPixel)
      continue;

    if (verbosity_) {
      unsigned int decRPId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();
      printf("%u (%u): x=%.3f, y=%.3f mm\n", tr.getRPId(), decRPId, tr.getX(), tr.getY());
    }

    if (rpId.arm() == 0)
      tracks_45.emplace_back(CTPPSLocalTrackLiteRef(hTracks, i));
    if (rpId.arm() == 1)
      tracks_56.emplace_back(CTPPSLocalTrackLiteRef(hTracks, i));

    nTracksPerRP[tr.getRPId()]++;
  }

  // for the moment: check whether there is no more than 1 track in each arm
  bool singleTrack_45 = true, singleTrack_56 = true;
  for (const auto &p : nTracksPerRP)
  {
    if (p.second > 1)
    {
      CTPPSDetId rpId(p.first);
      if (rpId.arm() == 0)
        singleTrack_45 = false;
      if (rpId.arm() == 1)
        singleTrack_56 = false;
    }
  }

  // single-RP reconstruction
  if (doSingleRPReconstruction_)
  {
    unique_ptr<reco::ProtonTrackCollection> output(new reco::ProtonTrackCollection);
    unique_ptr<reco::ProtonTrackExtraCollection> extras(new reco::ProtonTrackExtraCollection);

    // prepare the reference to extra proton tracks attributes
    reco::ProtonTrackExtraRefProd rExtras = event.getRefBeforePut<reco::ProtonTrackExtraCollection>(singleRPLabel);

    algorithm_.reconstructFromSingleRP(tracks_45, *output, *extras, rExtras, *hLHCInfo);
    algorithm_.reconstructFromSingleRP(tracks_56, *output, *extras, rExtras, *hLHCInfo);

    event.put(move(output), singleRPLabel);
    event.put(move(extras), singleRPLabel);
  }

  // multi-RP reconstruction
  if (doMultiRPReconstruction_)
  {
    unique_ptr<reco::ProtonTrackCollection> output(new reco::ProtonTrackCollection);
    unique_ptr<reco::ProtonTrackExtraCollection> extras(new reco::ProtonTrackExtraCollection);

    // prepare the reference to extra proton tracks attributes
    reco::ProtonTrackExtraRefProd rExtras = event.getRefBeforePut<reco::ProtonTrackExtraCollection>(multiRPLabel);

    if (singleTrack_45)
      algorithm_.reconstructFromMultiRP(tracks_45, *output, *extras, rExtras, *hLHCInfo);
    if (singleTrack_56)
      algorithm_.reconstructFromMultiRP(tracks_56, *output, *extras, rExtras, *hLHCInfo);

    event.put(move(output), multiRPLabel);
    event.put(move(extras), multiRPLabel);
  }
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(CTPPSProtonReconstruction);
