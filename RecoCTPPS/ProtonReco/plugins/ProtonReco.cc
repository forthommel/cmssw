// -*- C++ -*-
//
// Package:    RecoCTPPS/ProtonReco
// Class:      ProtonReco
// 
/**\class ProtonReco ProtonReco.cc RecoCTPPS/ProtonReco/plugins/ProtonReco.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme
//         Created:  Fri, 07 Jul 2017 15:21:58 GMT
//
//


#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/Common/interface/View.h"

#include "DataFormats/CTPPSReco/interface/CTPPSLocalTrackLite.h"
#include "DataFormats/CTPPSReco/interface/Proton.h"

#include "RecoCTPPS/ProtonReco/interface/ProtonReconstruction.h"
#include "RecoCTPPS/ProtonReco/interface/Alignment.h"

class ProtonReco : public edm::stream::EDProducer<> {
  public:
    explicit ProtonReco( const edm::ParameterSet& );
    ~ProtonReco();

    static void fillDescriptions( edm::ConfigurationDescriptions& );

  private:
    virtual void produce( edm::Event&, const edm::EventSetup& ) override;
    virtual void beginRun( const edm::Run&, const edm::EventSetup& ) override;

    edm::EDGetTokenT< edm::View<CTPPSLocalTrackLite> > tracksToken_;

    std::unique_ptr<ctpps::ProtonReconstruction> algo_;
    std::unique_ptr<ctpps::Alignment> align_;

    std::map<unsigned int, ctpps::Alignment::align_t> run_align_;
};

ProtonReco::ProtonReco( const edm::ParameterSet& iConfig ) :
  tracksToken_( consumes<edm::View<CTPPSLocalTrackLite> >( iConfig.getParameter<edm::InputTag>( "tracksTag" ) ) ),
  algo_( std::make_unique<ctpps::ProtonReconstruction>( iConfig.getParameter<edm::ParameterSet>( "protonRecoAlgo" ) ) )
{
  produces<std::vector<reco::Proton> >();
  if ( iConfig.exists( "alignmentFile" ) ) {
    align_ = std::make_unique<ctpps::Alignment>( iConfig.getParameter<edm::FileInPath>( "alignmentFile" ).fullPath().c_str() );
  }
}


ProtonReco::~ProtonReco()
{}

void
ProtonReco::produce( edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  std::unique_ptr<std::vector<reco::Proton> > pOut( new std::vector<reco::Proton>() );

  edm::Handle<edm::View<CTPPSLocalTrackLite> > tracks;
  iEvent.getByToken( tracksToken_, tracks );

  for ( const auto& trk : *tracks ) {
    const unsigned int rp_id = trk.getRPId();
    double xi, err_xi;
    const double x_shift = ( align_ ) ? run_align_.at( rp_id ).x : 0.; 
    double x_rp = trk.getX() + x_shift; //FIXME units??
    algo_->reconstruct( rp_id, x_rp, xi, err_xi );
    pOut->emplace_back( rp_id, xi, err_xi );
  }

  iEvent.put( std::move( pOut ) ); 
}

void
ProtonReco::beginRun( const edm::Run&, const edm::EventSetup& )
{
  int fill = 0; //FIXME LUT from run
  if ( align_ ) run_align_ = align_->get( fill );
}
 
void
ProtonReco::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE( ProtonReco );
