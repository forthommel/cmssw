#include <memory>

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/NanoAOD/interface/FlatTable.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

class ExclusiveVariablesProducer : public edm::global::EDProducer<> {
public:
  explicit ExclusiveVariablesProducer(const edm::ParameterSet& iConfig)
      : verticesToken_(consumes<std::vector<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertices"))),
        electronsToken_(consumes<std::vector<pat::Electron>>(iConfig.getParameter<edm::InputTag>("electrons"))),
        muonsToken_(consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("muons"))),
        pfCandidatesToken_(
            consumes<std::vector<pat::PackedCandidate>>(iConfig.getParameter<edm::InputTag>("pfCandidates"))),
        goodVerticesCuts_str_(iConfig.getParameter<std::string>("goodVerticesCuts")),
        goodVerticesCuts_(goodVerticesCuts_str_, true),
        max3DDistance_(iConfig.getParameter<double>("max3DDistance")) {
    produces<nanoaod::FlatTable>("exclusiveVariablesTable");
  }

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<edm::InputTag>("vertices")->setComment("offline vertices collection");
    desc.add<edm::InputTag>("electrons")->setComment("electrons collection");
    desc.add<edm::InputTag>("muons")->setComment("muons collection");
    desc.add<edm::InputTag>("pfCandidates")->setComment("particle flow candidates collection");
    desc.add<std::string>("goodVerticesCuts")->setComment("selection to define a vertex as 'good'");
    desc.add<double>("max3DDistance", 1.e-2)->setComment("maximum 3D distance between vertex/lepton vertex");
    descriptions.add("exclusiveVariablesProducer", desc);
  }

  void produce(edm::StreamID id, edm::Event& iEvent, const edm::EventSetup& iSetup) const override {
    const auto& electrons = iEvent.get(electronsToken_);
    const auto& muons = iEvent.get(muonsToken_);
    const auto& pfcands = iEvent.get(pfCandidatesToken_);

    // book output variables for vertices
    std::vector<int> vertex_id, vertex_good;
    std::vector<int> vertex_dilepton_type;
    std::vector<int> vertex_lepton1, vertex_lepton2;
    std::vector<int> vertex_pfcandidates, vertex_pfcandidates_0p5mm, vertex_pfcandidates_1mm, vertex_pfcandidates_2mm,
        vertex_pfcandidates_5mm, vertex_pfcandidates_1cm, vertex_pfcandidates_10cm;

    size_t vtx_id = 0;
    for (const auto& vtx : iEvent.get(verticesToken_)) {
      const auto& vtx_pos = vtx.position();
      std::vector<int> sel_electrons, sel_muons;
      for (size_t i = 0; i < electrons.size(); ++i)
        if (const auto& ele_vtx = electrons.at(i).vertex(); (ele_vtx - vtx_pos).rho() < max3DDistance_)
          sel_electrons.emplace_back(i);
      if (sel_electrons.size() > 2)
        continue;  // skip vertices with more than two same-flavour leptons
      for (size_t i = 0; i < muons.size(); ++i)
        if (const auto& muon_vtx = muons.at(i).vertex(); (muon_vtx - vtx_pos).rho() < max3DDistance_)
          sel_muons.emplace_back(i);
      if (sel_muons.size() > 2)
        continue;  // skip vertices with more than two same-flavour leptons

      // particularise the dilepton type
      if (sel_electrons.size() == 1 && sel_muons.size() == 1) {  // e-mu/mu-e candidate
        const auto id_ele = sel_electrons.at(0), id_mu = sel_muons.at(0);
        if (electrons.at(id_ele).pt() > muons.at(id_mu).pt()) {
          vertex_lepton1.emplace_back(id_ele);
          vertex_lepton2.emplace_back(id_mu);
          vertex_dilepton_type.emplace_back(DileptonType::emu);
        } else {
          vertex_lepton1.emplace_back(id_mu);
          vertex_lepton2.emplace_back(id_ele);
          vertex_dilepton_type.emplace_back(DileptonType::mue);
        }
      } else if (sel_electrons.size() == 2) {  // di-electron candidate
        const auto id_ele1 = sel_electrons.at(0), id_ele2 = sel_electrons.at(1);
        if (electrons.at(id_ele1).pt() > electrons.at(id_ele2).pt()) {
          vertex_lepton1.emplace_back(id_ele1);
          vertex_lepton2.emplace_back(id_ele2);
        } else {
          vertex_lepton1.emplace_back(id_ele2);
          vertex_lepton2.emplace_back(id_ele1);
        }
        vertex_dilepton_type.emplace_back(DileptonType::ee);
      } else if (sel_muons.size() == 2) {  // di-muon candidate
        const auto id_mu1 = sel_muons.at(0), id_mu2 = sel_muons.at(1);
        if (muons.at(id_mu1).pt() > muons.at(id_mu2).pt()) {
          vertex_lepton1.emplace_back(id_mu1);
          vertex_lepton2.emplace_back(id_mu2);
        } else {
          vertex_lepton1.emplace_back(id_mu2);
          vertex_lepton2.emplace_back(id_mu1);
        }
        vertex_dilepton_type.emplace_back(DileptonType::mumu);
      } else
        continue;  // this vertex has no dilepton candidate ; skipping

      // at this stage we have a dilepton vertex candidate
      vertex_id.emplace_back(vtx_id++);
      vertex_good.emplace_back(goodVerticesCuts_(vtx));

      // count PF tracks associated (within a certain distance) to this vertex
      size_t num_pfcands = 0, num_pfcands_0p5mm = 0, num_pfcands_1mm = 0, num_pfcands_2mm = 0, num_pfcands_5mm = 0,
             num_pfcands_1cm = 0, num_pfcands_10cm = 0;
      for (const auto& pfcand : pfcands) {
        const auto distance = (pfcand.vertex() - vtx_pos).rho();
        if (distance < max3DDistance_)
          ++num_pfcands;
        if (distance < 0.05)
          ++num_pfcands_0p5mm;
        if (distance < 0.1)
          ++num_pfcands_1mm;
        if (distance < 0.2)
          ++num_pfcands_2mm;
        if (distance < 0.5)
          ++num_pfcands_5mm;
        if (distance < 1.)
          ++num_pfcands_1cm;
        if (distance < 10.)
          ++num_pfcands_10cm;
      }
      vertex_pfcandidates.emplace_back(num_pfcands);
      vertex_pfcandidates_0p5mm.emplace_back(num_pfcands_0p5mm);
      vertex_pfcandidates_1mm.emplace_back(num_pfcands_1mm);
      vertex_pfcandidates_2mm.emplace_back(num_pfcands_2mm);
      vertex_pfcandidates_5mm.emplace_back(num_pfcands_5mm);
      vertex_pfcandidates_1cm.emplace_back(num_pfcands_1cm);
      vertex_pfcandidates_10cm.emplace_back(num_pfcands_10cm);
    }

    // build track table
    auto exclVarsTab = std::make_unique<nanoaod::FlatTable>(vertex_id.size(), "ExclusiveVertex", false);
    exclVarsTab->addColumn<int>("id", vertex_id, "index of vertex in offline vertices collection");
    exclVarsTab->addColumn<int>(
        "good",
        vertex_good,
        "is vertex considered as 'good'? i.e. passing the '" + goodVerticesCuts_str_ + "' selection?");
    exclVarsTab->addColumn<int>(
        "dileptonType", vertex_dilepton_type, "type of dilepton associated to vertex (e/e=0, e/mu=1, mu/mu=2, mu/e=3)");
    exclVarsTab->addColumn<int>("lepton1", vertex_lepton1, "index of first lepton");
    exclVarsTab->addColumn<int>("lepton2", vertex_lepton2, "index of second lepton");
    exclVarsTab->addColumn<int>(
        "pfCandidates", vertex_pfcandidates, "number of particle flow candidates associated to this vertex");
    exclVarsTab->addColumn<int>("pfCandidates0p5mm",
                                vertex_pfcandidates_0p5mm,
                                "number of particle flow candidates associated to this vertex (d<0.5mm)");
    exclVarsTab->addColumn<int>("pfCandidates1mm",
                                vertex_pfcandidates_1mm,
                                "number of particle flow candidates associated to this vertex (d<1mm)");
    exclVarsTab->addColumn<int>("pfCandidates2mm",
                                vertex_pfcandidates_2mm,
                                "number of particle flow candidates associated to this vertex (d<2mm)");
    exclVarsTab->addColumn<int>("pfCandidates5mm",
                                vertex_pfcandidates_5mm,
                                "number of particle flow candidates associated to this vertex (d<5mm)");
    exclVarsTab->addColumn<int>("pfCandidates1cm",
                                vertex_pfcandidates_1cm,
                                "number of particle flow candidates associated to this vertex (d<1cm)");
    exclVarsTab->addColumn<int>("pfCandidates10cm",
                                vertex_pfcandidates_10cm,
                                "number of particle flow candidates associated to this vertex (d<10cm)");
    exclVarsTab->setDoc("exclusive analyses variables");

    iEvent.put(std::move(exclVarsTab), "exclusiveVariablesTable");  // store output
  }

private:
  enum DileptonType { ee = 0, emu = 1, mumu = 2, mue = 3 };

  const edm::EDGetTokenT<std::vector<reco::Vertex>> verticesToken_;
  const edm::EDGetTokenT<std::vector<pat::Electron>> electronsToken_;
  const edm::EDGetTokenT<std::vector<pat::Muon>> muonsToken_;
  const edm::EDGetTokenT<std::vector<pat::PackedCandidate>> pfCandidatesToken_;
  const std::string goodVerticesCuts_str_;
  const StringCutObjectSelector<reco::Vertex> goodVerticesCuts_;
  const double max3DDistance_;
};

DEFINE_FWK_MODULE(ExclusiveVariablesProducer);
