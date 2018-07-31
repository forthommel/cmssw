#include "IOMC/EventVertexGenerators/interface/BeamDivergenceVtxGenerator.h"

//----------------------------------------------------------------------------------------------------

BeamDivergenceVtxGenerator::BeamDivergenceVtxGenerator(const edm::ParameterSet& iConfig) :
  sourceToken_( consumes<edm::HepMCProduct>( iConfig.getParameter<edm::InputTag>( "src" ) ) ),

  // python config gives numbers in cm, we store them in mm
  vertexMeanX_               ( iConfig.getParameter<double>( "vertexMeanX" ) * 1E1 ),
  vertexMeanY_               ( iConfig.getParameter<double>( "vertexMeanY" ) * 1E1 ),
  vertexMeanZ_               ( iConfig.getParameter<double>( "vertexMeanZ" ) * 1E1 ),
  vertexSigmaX_              ( iConfig.getParameter<double>( "vertexSigmaX" ) * 1E1 ),
  vertexSigmaY_              ( iConfig.getParameter<double>( "vertexSigmaY" ) * 1E1 ),
  vertexSigmaZ_              ( iConfig.getParameter<double>( "vertexSigmaZ" ) * 1E1 ),

  beamDivergenceX_           ( iConfig.getParameter<double>( "beamDivergenceX" ) ),
  beamDivergenceY_           ( iConfig.getParameter<double>( "beamDivergenceY" ) ),

  simulateVertex_            ( iConfig.getParameter<bool>( "simulateVertex" ) ),
  simulateBeamDivergence_    ( iConfig.getParameter<bool>( "simulateBeamDivergence" ) )
{
  edm::Service<edm::RandomNumberGenerator> rng;
  if ( !rng.isAvailable() ) {
    throw cms::Exception("Configuration")
      << "The BeamDivergenceVtxGenerator requires the RandomNumberGeneratorService\n"
         "which is not present in the configuration file. \n" 
         "You must add the service\n"
         "in the configuration file or remove the modules that require it.";
  }

  produces<edm::HepMCProduct>();
}

//----------------------------------------------------------------------------------------------------

BeamDivergenceVtxGenerator::~BeamDivergenceVtxGenerator() 
{
}

//----------------------------------------------------------------------------------------------------

void BeamDivergenceVtxGenerator::produce(edm::Event& iEvent, const edm::EventSetup&)
{
  // get random engine
  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine* rnd = &(rng->getEngine(iEvent.streamID()));

  // get input
  edm::Handle<edm::HepMCProduct> hepUnsmearedMCEvt;
  iEvent.getByToken(sourceToken_, hepUnsmearedMCEvt);

  // prepare output
  HepMC::GenEvent* genevt = new HepMC::GenEvent(*hepUnsmearedMCEvt->GetEvent());
  std::unique_ptr<edm::HepMCProduct> pEvent(new edm::HepMCProduct(genevt));

  // apply vertex smearing
  if (simulateVertex_)
  {
    const double vtx_x = vertexMeanX_ + CLHEP::RandGauss::shoot(rnd) * vertexSigmaX_;
    const double vtx_y = vertexMeanY_ + CLHEP::RandGauss::shoot(rnd) * vertexSigmaY_;
    const double vtx_z = vertexMeanZ_ + CLHEP::RandGauss::shoot(rnd) * vertexSigmaZ_;

    HepMC::FourVector shift(vtx_x, vtx_y, vtx_z, 0.);
    pEvent->applyVtxGen(&shift);
  }

  // apply beam divergence
  if (simulateBeamDivergence_)
  {
    const double bd_x_45 = CLHEP::RandGauss::shoot(rnd) * beamDivergenceX_;
    const double bd_x_56 = CLHEP::RandGauss::shoot(rnd) * beamDivergenceX_;

    const double bd_y_45 = CLHEP::RandGauss::shoot(rnd) * beamDivergenceY_;
    const double bd_y_56 = CLHEP::RandGauss::shoot(rnd) * beamDivergenceY_;

    for (HepMC::GenEvent::particle_iterator part = genevt->particles_begin(); part != genevt->particles_end(); ++part)
    {
      const HepMC::FourVector mom = (*part)->momentum();

      // TODO: this is an oversimplified implemetation
      // the TOTEM smearing module should be taken as reference

      double th_x = mom.x() / mom.z();
      double th_y = mom.y() / mom.z();

      if (mom.z() > 0.)
      {
        th_x += bd_x_45;
        th_y += bd_y_45;
      } else {
        th_x += bd_x_56;
        th_y += bd_y_56;
      }

      // calculate consistent p_z component
      const double sign = (mom.z() > 0.) ? 1. : -1.;
      const double p_z = sign * mom.rho() / sqrt(1. + th_x*th_x + th_y*th_y);

      // set smeared momentum
      (*part)->set_momentum(HepMC::FourVector(p_z * th_x, p_z * th_y, p_z, mom.e()));
    }
  }

  // save output
  iEvent.put(std::move(pEvent));
}
