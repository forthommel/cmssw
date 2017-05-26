#include "IOMC/ParticleGuns/interface/FlatRandomXiGunProducer.h"

FlatRandomXiGunProducer::FlatRandomXiGunProducer( const edm::ParameterSet& iConfig ) :
  partGunParams_( iConfig.getParameter<edm::ParameterSet>( "PGunParameters" ) ),
  partIds_( partGunParams_.getParameter< std::vector<int> >( "PartID" ) ),
  sqrtS_( partGunParams_.getParameter<double>( "SqrtS" ) ),
  beamConditions_( iConfig.getParameter<edm::ParameterSet>( "BeamConditions" ) ),
  simulateVertexX_( iConfig.getParameter<bool>( "simulateVertexX" ) ),
  simulateVertexY_( iConfig.getParameter<bool>( "simulateVertexY" ) ),
  vertexSize_( beamConditions_.getParameter<double>( "vertexSize" ) ),
  simulateScatteringAngleX_( iConfig.getParameter<bool>( "simulateScatteringAngleX" ) ),
  simulateScatteringAngleY_( iConfig.getParameter<bool>( "simulateScatteringAngleY" ) ),
  thetaPhys_( iConfig.getParameter<double>( "scatteringAngle" ) ),
  simulateBeamDivergence_( iConfig.getParameter<bool>( "simulateBeamDivergence" ) ),
  beamDivergence_( beamConditions_.getParameter<double>( "beamDivergence" ) ),
  minXi_( partGunParams_.getParameter<double>( "MinXi" ) ),
  maxXi_( partGunParams_.getParameter<double>( "MaxXi" ) )
{
  produces<HepMCProduct>( "unsmeared" );
  produces<GenEventInfoProduct>();
}


FlatRandomXiGunProducer::~FlatRandomXiGunProducer()
{}


// ------------ method called to produce the data  ------------
void
FlatRandomXiGunProducer::produce( edm::Event& iEvent, const edm::EventSetup& iSetup )
{
  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine* rnd = &( rng->getEngine( iEvent.streamID() ) );

  

  std::unique_ptr<HepMCProduct pOut( new HepMCProduct );
  std::unique_ptr<GenEventInfoProduct> pGenEventInfo( new GenEventInfoProduct( *pOut ) );

  // generate vertex
  std::unique_ptr<HepMC::GenVertex> pVertex( new HepMC::GenVertex() );
  if ( simulateVertexX_ ) pVertex->position().setX( CLHEP::RandGauss::shoot( rnd ) * vertexSize_ );
  if ( simulateVertexY_ ) pVertex->position().setY( CLHEP::RandGauss::shoot( rnd ) * vertexSize_ );

  unsigned short barcode = 0;
  for ( const auto& part : partIds_ ) {
    auto part_data = std::make_unique<HepPDT::ParticleData>( pdgTable_->particle( HepPDT::ParticleID( abs( part ) ) ) );
    const double mass = part_data->mass().value();

    std::unique_ptr<HepMC::GenParticle> p( new HepMC::GenParticle( shoot( rnd, mass ), part, 1 ) );
    p->suggest_barcode( barcode );
    pVertex->add_particle_out( p );

    barcode++;
  }

  iEvent.put( std::move( pOut ) );
  iEvent.put( std::move( pGenEventInfo ) );
}

//----------------------------------------------------------------------------------------------------

HepMC::FourVector
FlatRandomXiGunProducer::shoot( CLHEP::HepRandomEngine* rnd, double mass )
{
  // generate xi
  double xi = 0.;
  xi = minXi_ + CLHEP::RandFlat::shoot( rnd ) * ( maxXi_-minXi_ );
  
  // generate scattering angles (physics)
  double th_x_phys = 0., th_y_phys = 0.;

  if ( simulateScatteringAngleX_ ) th_x_phys += CLHEP::RandGauss::shoot( rnd ) * thetaPhys_;
  if ( simulateScatteringAngleY_ ) th_y_phys += CLHEP::RandGauss::shoot( rnd ) * thetaPhys_;

  // generate beam divergence, calculate complete angle
  double th_x = th_x_phys, th_y = th_y_phys;

  if ( simulateBeamDivergence_ ) {
    th_x += CLHEP::RandGauss::shoot( rnd ) * beamDivergence_;
    th_y += CLHEP::RandGauss::shoot( rnd ) * beamDivergence_;
  }

  const double e_part = ( ... ); //FIXME

  return HepMC::FourVector( vtx, Local3DVector( th_x, th_y, 0. ),  );
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
FlatRandomXiGunProducer::beginRun( const edm::Run&, const edm::EventSetup& iSetup )
{
  iSetup.getData( pdgTable_ );
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
FlatRandomXiGunProducer::endRun( const edm::Run&, const edm::EventSetup& )
{}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
FlatRandomXiGunProducer::fillDescriptions( edm::ConfigurationDescriptions& descriptions )
{
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault( desc );
}

// define this as a plug-in
DEFINE_FWK_MODULE( FlatRandomXiGunProducer );
