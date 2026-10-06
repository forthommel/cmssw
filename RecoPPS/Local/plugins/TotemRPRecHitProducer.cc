/****************************************************************************
*
* This is a part of TOTEM offline software.
* Authors:
* 	Hubert Niewiadomski
*   Jan Kašpar (jan.kaspar@gmail.com)
*
****************************************************************************/

#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSReco/interface/TotemRPCluster.h"
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"

#include "RecoPPS/Local/interface/TotemRPRecHitProducerAlgorithm.h"

//----------------------------------------------------------------------------------------------------

class TotemRPRecHitProducer : public edm::stream::EDProducer<> {
public:
  explicit TotemRPRecHitProducer(const edm::ParameterSet& iConfig)
      : tokenCluster_(consumes<edm::DetSetVector<TotemRPCluster>>(iConfig.getParameter<edm::InputTag>("tagCluster"))),
        verbosity_(iConfig.getParameter<int>("verbosity")),
        algorithm_(iConfig) {
    produces<std::vector<TotemRPRecHit>>();
  }

  void produce(edm::Event& event, const edm::EventSetup&) override {
    auto output = std::make_unique<std::vector<TotemRPRecHit>>();  // prepare output
    for (auto& ids : event.get(tokenCluster_))
      algorithm_.buildRecoHits(ids, *output);  // build reco hits
    event.put(std::move(output));              // save output
  }

  static void fillDescriptions(edm::ConfigurationDescriptions& descr) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("tagCluster", edm::InputTag("totemRPClusterProducer"))
        ->setComment("input clusters collection to retrieve");
    desc.add<int>("verbosity", 0);
    descr.add("totemRPRecHitProducer", desc);
  }

private:
  const edm::EDGetTokenT<edm::DetSetVector<TotemRPCluster>> tokenCluster_;
  const int verbosity_;

  TotemRPRecHitProducerAlgorithm algorithm_;
};

//----------------------------------------------------------------------------------------------------

DEFINE_FWK_MODULE(TotemRPRecHitProducer);
