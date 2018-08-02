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
    edm::FileInPath alignmentFile_;

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
  alignmentFile_              ( iConfig.getParameter<edm::FileInPath>( "alignmentFile" ) ),

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
    alignmentCollection_.Load(alignmentFile_.fullPath());
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

  // get and apply alignment
  auto tracksAligned = *tracks;

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

    tracksAligned = alignment_it->second.Apply(*tracks);
  }

  if (applyTestAlignment)
  {
    tracksAligned.clear();
    for (const auto &t : *tracks)
    {
      CTPPSDetId rpId(t.getRPId());

      double de_x = 0., de_y = 0.;
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
  for (const auto &tr : tracksAligned)
  {
    CTPPSDetId rpId(tr.getRPId());
    if (rpId.arm() == 0)
      tracks_45.push_back(&tr);
    else
      tracks_56.push_back(&tr);
  }

  // run reconstruction per sector
  // TODO
  algorithm_.reconstruct(tracks_45, *output);
  algorithm_.reconstruct(tracks_56, *output);

  // TODO
  algorithm_.reconstructFromSingleRP(tracks_45, *output);
  algorithm_.reconstructFromSingleRP(tracks_56, *output);

  // save output to event
  event.put(move(output));
}

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(CTPPSProtonReconstruction);
