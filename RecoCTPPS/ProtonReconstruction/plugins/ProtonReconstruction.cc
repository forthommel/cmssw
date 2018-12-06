/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"

#include "DataFormats/ProtonReco/interface/ProtonTrack.h"
#include "DataFormats/ProtonReco/interface/ProtonTrackFwd.h"
#include "DataFormats/ProtonReco/interface/ProtonTrackExtra.h"

#include "RecoCTPPS/ProtonReconstruction/interface/AlignmentsFactory.h"

class ProtonReconstruction : public edm::stream::EDProducer<>
{
  public:
    explicit ProtonReconstruction( const edm::ParameterSet& );
    ~ProtonReconstruction() = default;

    static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

  private:
    void produce( edm::Event&, const edm::EventSetup& ) override;

    edm::EDGetTokenT<std::vector<CTPPSLocalTrackLite> > tracksToken_;
    pps::AlignmentsFactory align_;
};

ProtonReconstruction::ProtonReconstruction( const edm::ParameterSet& iConfig ) :
  tracksToken_( consumes<std::vector<CTPPSLocalTrackLite> >( iConfig.getParameter<edm::InputTag>( "localTrackLiteTag" ) ) ),
  align_( iConfig.getParameter<edm::FileInPath>( "alignmentFilePath" ).fullPath() )
{
  produces<reco::ProtonTrackCollection>();
  produces<reco::ProtonTrackExtraCollection>();
}

void
ProtonReconstruction::produce( edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  edm::Handle<std::vector<CTPPSLocalTrackLite> > tracks;
  iEvent.getByToken( tracksToken_, tracks );

  reco::ProtonTrackExtraRefProd r_extras = iEvent.getRefBeforePut<reco::ProtonTrackExtraCollection>();
  std::unique_ptr<reco::ProtonTrackCollection> output( new reco::ProtonTrackCollection );
  std::unique_ptr<reco::ProtonTrackExtraCollection> extras( new reco::ProtonTrackExtraCollection );

  unsigned short fill_number = 0;

  std::for_each( tracks->begin(), tracks->end(), [this,&fill_number]( const CTPPSLocalTrackLite& trk ) {
    const auto& align_per_pot = align_.get( fill_number );
  } );
}

void
ProtonReconstruction::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>( "localTrackLiteTag", edm::InputTag( "ctppsLocalTrackLiteProducer" ) )
    ->setComment( "input lite local tracks collection" );
  desc.add<edm::FileInPath>( "alignmentFilePath", edm::FileInPath( "RecoCTPPS/ProtonReconstruction/data/collect_alignments_2017_01_17.out" ) )
    ->setComment( "per-pot alignments table" );

  descriptions.add( "protontracks", desc );
}

DEFINE_FWK_MODULE( ProtonReconstruction );

