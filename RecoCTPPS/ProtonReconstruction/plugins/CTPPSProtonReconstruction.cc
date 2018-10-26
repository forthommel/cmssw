/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/ProtonReco/interface/ProtonTrack.h"

#include "RecoCTPPS/ProtonReconstruction/interface/ProtonReconstructionAlgorithm.h"

#include "RecoCTPPS/ProtonReconstruction/interface/alignment_classes.h"
#include "RecoCTPPS/ProtonReconstruction/interface/fill_info.h"

//----------------------------------------------------------------------------------------------------

class CTPPSProtonReconstruction : public edm::stream::EDProducer<>
{
  public:
    explicit CTPPSProtonReconstruction(const edm::ParameterSet&);
    ~CTPPSProtonReconstruction();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  private:
    virtual void produce(edm::Event&, const edm::EventSetup&) override;

    edm::EDGetTokenT< std::vector<CTPPSLocalTrackLite> > tracksToken_;

    unsigned int verbosity;

    edm::ParameterSet beamConditions_;
    std::vector<edm::ParameterSet> detectorPackages_;

    edm::FileInPath opticsFileBeam1_, opticsFileBeam2_;

    bool applyExperimentalAlignment;
    std::vector<std::string> alignmentFiles_;

    bool applyTestAlignment;
    double de_x_N, de_x_F, de_y_N, de_y_F;

    AlignmentResultsCollection alignmentCollection_;

    ProtonReconstructionAlgorithm algorithm_;
};

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

CTPPSProtonReconstruction::CTPPSProtonReconstruction( const edm::ParameterSet& iConfig ) :
  tracksToken_( consumes< std::vector<CTPPSLocalTrackLite> >( iConfig.getParameter<edm::InputTag>( "tagLocalTrackLite" ) ) ),

  verbosity                   ( iConfig.getUntrackedParameter<unsigned int>( "verbosity", 0 ) ),

  beamConditions_             ( iConfig.getParameter<edm::ParameterSet>( "beamConditions" ) ),
  detectorPackages_           ( iConfig.getParameter<std::vector<edm::ParameterSet>>( "detectorPackages" ) ),
  opticsFileBeam1_            ( iConfig.getParameter<edm::FileInPath>( "opticsFileBeam1" ) ),
  opticsFileBeam2_            ( iConfig.getParameter<edm::FileInPath>( "opticsFileBeam2" ) ),

  applyExperimentalAlignment  ( iConfig.getParameter<bool>( "applyExperimentalAlignment" ) ),
  alignmentFiles_              ( iConfig.getParameter<std::vector<std::string>>( "alignmentFiles" ) ),

  applyTestAlignment  ( iConfig.getParameter<bool>( "applyTestAlignment" ) ),
  de_x_N(iConfig.getParameter<double>("de_x_N")),
  de_x_F(iConfig.getParameter<double>("de_x_F")),
  de_y_N(iConfig.getParameter<double>("de_y_N")),
  de_y_F(iConfig.getParameter<double>("de_y_F")),

  algorithm_(opticsFileBeam1_.fullPath(), opticsFileBeam2_.fullPath(), beamConditions_, detectorPackages_, verbosity)
{
  produces<vector<reco::ProtonTrack>>();

  //  load alignment collection
  if (applyExperimentalAlignment)
  {
    for (const auto &f : alignmentFiles_)
    {
      FileInPath fip(f);
      if (alignmentCollection_.Load(fip.fullPath()) != 0)
        throw cms::Exception("CTPPSProtonReconstruction") << "Cannot load alignment from file '" << f << "'.";
    }
  }

  // load fill-alignment mapping
  InitFillInfoCollection();
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstruction::fillDescriptions(ConfigurationDescriptions& descriptions)
{
  ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//----------------------------------------------------------------------------------------------------

CTPPSProtonReconstruction::~CTPPSProtonReconstruction()
{
}

//----------------------------------------------------------------------------------------------------

void CTPPSProtonReconstruction::produce(Event& event, const EventSetup&)
{
  // get input
  Handle< vector<CTPPSLocalTrackLite> > tracks;
  event.getByToken(tracksToken_, tracks);

  // prepare output
  unique_ptr<vector<reco::ProtonTrack>> output( new vector<reco::ProtonTrack> );

  // keep only tracks from tracker RPs
  vector<CTPPSLocalTrackLite> tracksFiltered;
  for (const auto &tr : *tracks)
  {
    CTPPSDetId rpId(tr.getRPId());
    if (rpId.subdetId() == CTPPSDetId::sdTrackingStrip || rpId.subdetId() == CTPPSDetId::sdTrackingPixel)
      tracksFiltered.push_back(tr);
  }

  // get and apply alignment
  auto tracksAligned = tracksFiltered;

  if (applyExperimentalAlignment)
  {
    FillInfo fillInfo;
    unsigned int ret = fillInfoCollection.FindByRun(event.id().run(), fillInfo);
    if (ret != 0)
    {
      printf("ERROR: cannot get fill info.\n");
      event.put(move(output));
      return;
    }

    const auto alignment_it = alignmentCollection_.find(fillInfo.alignmentTag);
    if (alignment_it == alignmentCollection_.end())
    {
      printf("ERROR: cannot get alignment.\n");
      event.put(move(output));
      return;
    }

    tracksAligned = alignment_it->second.Apply(tracksFiltered);
  }

  if (applyTestAlignment)
  {
    tracksAligned.clear();
    for (const auto &t : tracksFiltered)
    {
      CTPPSDetId rpId(t.getRPId());

      double de_x = 0., de_y = 0.;
      // TODO: this is not always correct
      if (rpId.rp() == 2) { de_x = de_x_N; de_y = de_y_N; }
      if (rpId.rp() == 3) { de_x = de_x_F; de_y = de_y_F; }

      tracksAligned.emplace_back(t.getRPId(), t.getX() + de_x, t.getXUnc(), t.getY() + de_y, t.getYUnc());
    }
  }

  if (verbosity)
  {
    printf("\n---------- %u:%llu ----------\n", event.id().run(), event.id().event());
    for (const auto &tr : tracksAligned)
    {
      CTPPSDetId rpId(tr.getRPId());
      unsigned int decRPId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();
      printf("%u (%u): x=%.3f, y=%.3f mm\n", tr.getRPId(), decRPId, tr.getX(), tr.getY());
    }
  }

  // split input per sector
  vector<const CTPPSLocalTrackLite*> tracks_45, tracks_56;
  map<unsigned int, unsigned int> nTracksPerRP;
  for (const auto &tr : tracksAligned)
  {
    CTPPSDetId rpId(tr.getRPId());
    if (rpId.arm() == 0)
      tracks_45.push_back(&tr);
    else
      tracks_56.push_back(&tr);

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

  // run reconstruction per sector
  algorithm_.reconstructFromSingleRP(tracks_45, *output);
  algorithm_.reconstructFromSingleRP(tracks_56, *output);

  if (singleTrack_45)
    algorithm_.reconstructFromMultiRP(tracks_45, *output);
  if (singleTrack_56)
    algorithm_.reconstructFromMultiRP(tracks_56, *output);

  // save output to event
  event.put(move(output));
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(CTPPSProtonReconstruction);
