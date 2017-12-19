#include "IOMC/EventVertexGenerators/interface/CrossingAngleVtxGenerator.h"

CrossingAngleVtxGenerator::CrossingAngleVtxGenerator( const edm::ParameterSet& iConfig ) :
  sourceToken_( consumes<edm::HepMCProduct>( iConfig.getParameter<edm::InputTag>( "src" ) ) ),
  sourceBisToken_( consumes<LHEEventProduct>( iConfig.getParameter<edm::InputTag>( "src" ) ) ),
  vertexSize_               ( iConfig.getParameter<double>( "vertexSize" ) ),
  beamDivergence_           ( iConfig.getParameter<double>( "beamDivergence" ) ),
  halfCrossingAngleSector45_( iConfig.getParameter<double>( "halfCrossingAngleSector45" ) ),
  halfCrossingAngleSector56_( iConfig.getParameter<double>( "halfCrossingAngleSector56" ) ),
  simulateVertexX_          ( iConfig.getParameter<bool>( "simulateVertexX" ) ),
  simulateVertexY_          ( iConfig.getParameter<bool>( "simulateVertexY" ) ),
  simulateBeamDivergence_   ( iConfig.getParameter<bool>( "simulateBeamDivergence" ) ),
  rnd_( 0 )
{
  edm::Service<edm::RandomNumberGenerator> rng;
  if ( !rng.isAvailable() ) {
    throw cms::Exception("Configuration")
      << "The CrossingAngleVtxGenerator requires the RandomNumberGeneratorService\n"
         "which is not present in the configuration file. \n" 
         "You must add the service\n"
         "in the configuration file or remove the modules that require it.";
  }
  produces<edm::HepMCProduct>();
}

CrossingAngleVtxGenerator::~CrossingAngleVtxGenerator() 
{}

void
CrossingAngleVtxGenerator::produce( edm::Event& iEvent, const edm::EventSetup& )
{
  edm::Service<edm::RandomNumberGenerator> rng;
  rnd_ = &( rng->getEngine( iEvent.streamID() ) );

  HepMC::GenEvent* genevt = 0;

  edm::Handle<edm::HepMCProduct> HepUnsmearedMCEvt;
  bool found = iEvent.getByToken( sourceToken_, HepUnsmearedMCEvt );
  if ( found ) {
    // copy the HepMC::GenEvent
    genevt = new HepMC::GenEvent( *HepUnsmearedMCEvt->GetEvent() );
  }
  else {
    edm::Handle<LHEEventProduct> lheEvent;
    found = iEvent.getByToken( sourceBisToken_, lheEvent );
    auto event = lheEvent->hepeup();

    genevt = new HepMC::GenEvent;
    auto vtx = new HepMC::GenVertex;
    genevt->add_vertex( vtx );
    for ( unsigned short i = 0; i < event.NUP; ++i ) {
      if ( event.ISTUP[i] < 0 ) continue;
      vtx->add_particle_out( new HepMC::GenParticle( HepMC::FourVector( event.PUP[i][0], event.PUP[i][1], event.PUP[i][2], event.PUP[i][3] ), event.IDUP[i], event.ISTUP[i] ) );
    }
  }
  if ( !found ) return;
  std::unique_ptr<edm::HepMCProduct> pEvent( new edm::HepMCProduct( genevt ) );
  pEvent->applyVtxGen( vertexPosition().get() );
  for ( HepMC::GenEvent::particle_iterator part = genevt->particles_begin(); part != genevt->particles_end(); ++part ) {
    rotateParticle( *part );
  }

  iEvent.put( std::move( pEvent ) );
}

std::shared_ptr<HepMC::FourVector>
CrossingAngleVtxGenerator::vertexPosition() const
{
  double vtx_x = 0.0, vtx_y = 0.0; // express in metres
  if ( simulateVertexX_ ) vtx_x += CLHEP::RandGauss::shoot( rnd_ ) * vertexSize_;
  if ( simulateVertexY_ ) vtx_y += CLHEP::RandGauss::shoot( rnd_ ) * vertexSize_;
  return std::make_shared<HepMC::FourVector>( vtx_x, vtx_y, 0.0, 0.0 );
}

void
CrossingAngleVtxGenerator::rotateParticle( HepMC::GenParticle* part ) const
{
  const HepMC::FourVector mom = part->momentum();

  // convert physics kinematics to the LHC reference frame
  double th_x = atan2( mom.x(), mom.z() ), th_y = atan2( mom.y(), mom.z() );

  // CMS convention
  if ( mom.z() > 0.0 ) th_x += halfCrossingAngleSector45_;
  if ( mom.z() < 0.0 ) th_x -= halfCrossingAngleSector56_;

  const short sign = ( mom.z() > 0 ) ? 1 : -1;

  // generate beam divergence
  if ( simulateBeamDivergence_ ) {
    th_x += CLHEP::RandGauss::shoot( rnd_ ) * beamDivergence_;
    th_y += CLHEP::RandGauss::shoot( rnd_ ) * beamDivergence_;
  }

  const double tan_th_x = tan( th_x ), tan_th_y = tan( th_y );
  const double pz = sign * mom.rho() * sqrt( 1.0/( 1.0 + tan_th_x*tan_th_x + tan_th_y*tan_th_y ) );
  HepMC::FourVector mom_smeared( pz*tan_th_x, pz*tan_th_y, pz, mom.e() );
  part->set_momentum( mom_smeared );
}

