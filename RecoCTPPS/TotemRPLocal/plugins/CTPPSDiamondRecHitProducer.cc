/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *   Nicola Minafra (nicola.minafra@cern.ch)
 *
 ****************************************************************************/

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSet.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSDiamondDigi.h"
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondRecHit.h"

class CTPPSDiamondRecHitProducer : public edm::stream::EDProducer<>
{
  public:
    explicit CTPPSDiamondRecHitProducer( const edm::ParameterSet& );
    ~CTPPSDiamondRecHitProducer();

    static void fillDescriptions( edm::ConfigurationDescriptions& );

  private:
    virtual void produce( edm::Event&, const edm::EventSetup& ) override;

    edm::EDGetTokenT< edm::DetSetVector<CTPPSDiamondDigi> > recHitsToken_;
};

CTPPSDiamondRecHitProducer::CTPPSDiamondRecHitProducer( const edm::ParameterSet& iConfig ) :
  recHitsToken_( consumes< edm::DetSetVector<CTPPSDiamondDigi> >( iConfig.getParameter<edm::InputTag>( "recHitsTag" ) ) )
{
  produces< std::vector<CTPPSDiamondRecHit> >();
}

CTPPSDiamondRecHitProducer::~CTPPSDiamondRecHitProducer()
{
}

void
CTPPSDiamondRecHitProducer::produce( edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  std::unique_ptr< std::vector<CTPPSDiamondRecHit> > pOut( new std::vector<CTPPSDiamondRecHit> );

  edm::Handle< edm::DetSetVector<CTPPSDiamondDigi> > digis;
  iEvent.getByToken( recHitsToken_, digis );

  for ( edm::DetSetVector<CTPPSDiamondDigi>::const_iterator dsv_digi = digis->begin(); dsv_digi != digis->end(); dsv_digi++ ) {
    const CTPPSDiamondDetId detid( dsv_digi->detId() );

    for ( edm::DetSet<CTPPSDiamondDigi>::const_iterator ds_digi = dsv_digi->begin(); ds_digi != dsv_digi->end(); ds_digi++ ) {
      // ...
    }
  }

  iEvent.put( std::move( pOut ) );
}

void
CTPPSDiamondRecHitProducer::fillDescriptions( edm::ConfigurationDescriptions& descr )
{
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descr.addDefault( desc );
}

DEFINE_FWK_MODULE( CTPPSDiamondRecHitProducer );
