/****************************************************************************
 * Authors:
 *   Jan Kašpar
 ****************************************************************************/

#ifndef IOMC_EventVertexGenerators_BeamDivergenceVtxGenerator_h
#define IOMC_EventVertexGenerators_BeamDivergenceVtxGenerator_h

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

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
    explicit BeamDivergenceVtxGenerator(const edm::ParameterSet&);
    ~BeamDivergenceVtxGenerator() override = default;

    static void fillDescriptions(edm::ConfigurationDescriptions&);

    void produce(edm::Event&, const edm::EventSetup&) override;

  private:
    edm::EDGetTokenT<edm::HepMCProduct> sourceToken_;

    bool simulateVertex_;
    bool simulateBeamDivergence_;
};

#endif

