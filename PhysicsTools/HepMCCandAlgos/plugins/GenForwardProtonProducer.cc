/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include <algorithm>
#include <memory>

class GenForwardProtonProducer : public edm::stream::EDProducer<> {
public:
  explicit GenForwardProtonProducer(const edm::ParameterSet& iConfig)
      : genParticlesToken_(
            mayConsume<reco::GenParticleCollection>(iConfig.getParameter<edm::InputTag>("genParticlesTag"))),
        hepMCToken_(mayConsume<edm::HepMCProduct>(iConfig.getParameter<edm::InputTag>("hepMCTag"))),
        protons_status_codes_(iConfig.getParameter<std::vector<int> >("protonsStatusCodes")),
        extrapolate_from_partons_(iConfig.getParameter<bool>("extrapolateFromPartons")),
        incoming_partons_(iConfig.getParameter<std::vector<int> >("incomingPartons")),
        partons_status_codes_(iConfig.getParameter<std::vector<int> >("incomingPartonsStatusCodes")) {
    produces<reco::GenParticleCollection>();
  }

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add("genParticlesTag", edm::InputTag("genParticles"))
        ->setComment("label for the GenParticleCollection retrieval");
    desc.add("hepMCTag", edm::InputTag("generator", "unsmeared"))
        ->setComment("label for the alternative HepMCProduct retrieval");
    desc.add("protonsStatusCodes", std::vector{1, 83})
        ->setComment("list of (process- and generator-dependent) integer status codes characterising forward protons");
    desc.add("extrapolateFromPartons", false)
        ->setComment(
            "populate the forward protons collection with scattered particle from momentum balance after partons "
            "emission?");
    desc.add("incomingPartons", std::vector{22})
        ->setComment("list of PDG ids accepted as partons candidates in case the extrapolation is enabled");
    desc.add("incomingPartonsStatusCodes", std::vector{-1, 21})
        ->setComment("list of (process- and generator-dependent) integer status codes characterising incoming partons");
    descriptions.add("genForwardProtons", desc);
  }

private:
  void produce(edm::Event& iEvent, const edm::EventSetup&) override {
    auto generator_protons = std::make_unique<reco::GenParticleCollection>();  // output collection

    edm::Handle<reco::GenParticleCollection> gen_particles_handle;
    edm::Handle<edm::HepMCProduct> hepmc_product_handle;
    if (iEvent.getByToken(genParticlesToken_, gen_particles_handle); gen_particles_handle.isValid())
      extractFromGenParticles(*gen_particles_handle, *generator_protons);
    else if (iEvent.getByToken(hepMCToken_, hepmc_product_handle); hepmc_product_handle.isValid())
      extractFromHepMCProduct(*hepmc_product_handle, *generator_protons);
    else
      throw cms::Exception("GenForwardProtonProducer")
          << "Neither a reco::GenParticleCollection, nor a edm::HepMCProduct was found in the event content. Failed to "
             "reconstruct the forward protons kinematics.";
    iEvent.put(std::move(generator_protons));
  }

  void extractFromGenParticles(const reco::GenParticleCollection& gen_particles,
                               reco::GenParticleCollection& protons) const {
    // first pass: identify forward protons from particles content (works for a certain class of generators)
    if (extrapolate_from_partons_) {
      for (const auto& gen_particle : gen_particles)
        if (std::find(incoming_partons_.begin(), incoming_partons_.end(), gen_particle.pdgId()) !=
                incoming_partons_.end()  // incoming parton
            && std::find(partons_status_codes_.begin(), partons_status_codes_.end(), gen_particle.status()) !=
                   partons_status_codes_.end()        // in a given set of PDG identifiers
            && gen_particle.numberOfMothers() > 0) {  // with at least a parent (incoming beam particle)
          const auto* beam_particle = gen_particle.mother();
          protons.emplace_back(beam_particle->charge() - gen_particle.charge(),
                               beam_particle->p4() - gen_particle.p4(),
                               gen_particle.vertex(),
                               beam_particle->pdgId(),
                               1,
                               false);
        }
    } else  // compute forward protons kinematics from incoming partons kinematics
      for (const auto& gen_particle : gen_particles)
        if (gen_particle.pdgId() == 2212  // accept protons
            && std::find(protons_status_codes_.begin(), protons_status_codes_.end(), gen_particle.status()) !=
                   protons_status_codes_.end())  // in a given set of status codes
          protons.emplace_back(gen_particle);
  }

  void extractFromHepMCProduct(const edm::HepMCProduct& hepmc_product, reco::GenParticleCollection& protons) const {
    const auto* event = hepmc_product.GetEvent();
    if (!event)
      throw cms::Exception("GenForwardProtonProducer") << "Invalid HepMC event content.";
    if (extrapolate_from_partons_) {  // compute forward protons kinematics from incoming partons kinematics
      for (auto it_part = event->particles_begin(); it_part != event->particles_end();
           ++it_part)  // all particles in event
        if (const auto* part = *it_part;
            part &&
            std::find(incoming_partons_.begin(), incoming_partons_.end(), part->pdg_id()) !=
                incoming_partons_.end()  // incoming parton
            && std::find(partons_status_codes_.begin(), partons_status_codes_.end(), part->status()) !=
                   partons_status_codes_.end()  // in a given set of PDG identifiers
            && part->production_vertex()) {
          const auto* parton_production_vertex = part->production_vertex();
          if (parton_production_vertex->particles_in_size() == 1) {  // with sharply one parent (incoming beam particle)
            const auto* beam_particle = *(parton_production_vertex->particles_in_const_begin());
            if (std::abs(beam_particle->pdg_id()) != 2212)  // skip non-proton ancestors
              continue;
            const auto parton = convertHepMCProton(*part);
            auto& outgoing_beam_particle = protons.emplace_back(convertHepMCProton(*beam_particle));
            outgoing_beam_particle.setP4(outgoing_beam_particle.p4() - parton.p4());
            outgoing_beam_particle.setVertex(parton.vertex());
          }
        }
    } else  // identify forward protons from particles content (works for a certain class of generators)
      for (auto it_vtx = event->vertices_begin(); it_vtx != event->vertices_end(); ++it_vtx) {  // event vertices
        if (const auto* vtx = *it_vtx; vtx)
          for (auto it_part = vtx->particles_out_const_begin(); it_part != vtx->particles_out_const_end();
               ++it_part) {  // outgoing particles
            const auto* part = *it_part;
            if (std::abs(part->pdg_id()) == 2212  // accept protons
                && std::find(protons_status_codes_.begin(), protons_status_codes_.end(), part->status()) !=
                       protons_status_codes_.end()) {  // in a given set of status codes
              protons.emplace_back(convertHepMCProton(*part));
            }
          }
      }
  }

  static reco::GenParticle convertHepMCProton(const HepMC::GenParticle& hepmc_particle) {
    const auto& hepmc_momentum = hepmc_particle.momentum();
    const auto* hepmc_vertex = hepmc_particle.production_vertex();
    const auto& hepmc_vertex_position = hepmc_vertex->position();
    return reco::GenParticle(
        hepmc_particle.pdg_id() / std::abs(hepmc_particle.pdg_id()),  // also allows antiprotons, FWIW...
        reco::LeafCandidate::LorentzVector{
            hepmc_momentum.px(), hepmc_momentum.py(), hepmc_momentum.pz(), hepmc_momentum.e()},
        reco::LeafCandidate::Point{hepmc_vertex_position.x(), hepmc_vertex_position.y(), hepmc_vertex_position.z()},
        hepmc_particle.pdg_id(),
        hepmc_particle.status(),
        false);
  }

  const edm::EDGetTokenT<reco::GenParticleCollection> genParticlesToken_;
  const edm::EDGetTokenT<edm::HepMCProduct> hepMCToken_;
  const std::vector<int> protons_status_codes_;
  const bool extrapolate_from_partons_;
  const std::vector<int> incoming_partons_;
  const std::vector<int> partons_status_codes_;
};

//define this as a plug-in
DEFINE_FWK_MODULE(GenForwardProtonProducer);
