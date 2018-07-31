#ifndef IOMC_EventVertexGenerators_BeamDivergenceVtxGenerator_h
#define IOMC_EventVertexGenerators_BeamDivergenceVtxGenerator_h

#include <memory>

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "DataFormats/Provenance/interface/Provenance.h"
#include <CLHEP/Random/RandGauss.h>

namespace HepMC {
  class FourVector;
  class GenParticle;
}

namespace CLHEP {
  class HepRandomEngine;
}

//----------------------------------------------------------------------------------------------------

class BeamDivergenceVtxGenerator : public edm::stream::EDProducer<>
{
  public:
    explicit BeamDivergenceVtxGenerator( const edm::ParameterSet& );
    ~BeamDivergenceVtxGenerator();

    void produce(edm::Event&, const edm::EventSetup&) override;

  private:
    edm::EDGetTokenT<edm::HepMCProduct> sourceToken_;

    // values in mm
    double vertexMeanX_, vertexMeanY_, vertexMeanZ_;
    double vertexSigmaX_, vertexSigmaY_, vertexSigmaZ_;

    // values in rad
    double beamDivergenceX_, beamDivergenceY_;

    bool simulateVertex_;
    bool simulateBeamDivergence_;
};

#endif
