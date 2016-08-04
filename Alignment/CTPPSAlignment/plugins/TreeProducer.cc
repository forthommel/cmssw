// -*- C++ -*-
//
// Package:    Alignment/CTPPSAlignment
// Class:      TreeProducer
// 
/**\class TreeProducer TreeProducer.cc Alignment/CTPPSAlignment/plugins/TreeProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme
//         Created:  Thu, 04 Aug 2016 11:31:35 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// CT-PPS objects
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"

#include "TTree.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class TreeProducer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
 public:
  explicit TreeProducer(const edm::ParameterSet&);
  ~TreeProducer();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
 private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  // ----------member data ---------------------------
  edm::EDGetTokenT< edm::DetSetVector<TotemRPLocalTrack> > localTrackToken_;
  
  TTree* tree_;
  unsigned int aRun, aLumiSection, aEvent;
  std::vector<float> aTrackX, aTrackY, aTrackZ, aTrackSigmaX, aTrackSigmaY;
  std::vector<unsigned int> aTrackArm, aTrackSide;
  
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TreeProducer::TreeProducer(const edm::ParameterSet& iConfig) :
  localTrackToken_(consumes<edm::DetSetVector<TotemRPLocalTrack>>(iConfig.getParameter<edm::InputTag>("rpLocalTracksTag")))
{
   usesResource("TFileService");
   edm::Service<TFileService> fs;

   TFileDirectory dir = fs->mkdir(".");
   tree_ = fs->make<TTree>("ctpps_alca", "CT-PPS objects calibration tree");
   tree_->Branch("run", &aRun);
   tree_->Branch("lumisection", &aLumiSection);
   tree_->Branch("event", &aEvent);
   tree_->Branch("rp_track_x", &aTrackX);
   tree_->Branch("rp_track_y", &aTrackY);
   tree_->Branch("rp_track_z", &aTrackZ);
   tree_->Branch("rp_track_sigma_x", &aTrackSigmaX);
   tree_->Branch("rp_track_sigma_y", &aTrackSigmaY);
   tree_->Branch("rp_track_arm", &aTrackArm);
   tree_->Branch("rp_track_side", &aTrackSide);
}


TreeProducer::~TreeProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  aRun = iEvent.id().run();
  aLumiSection = iEvent.id().luminosityBlock();
  aEvent = iEvent.id().event();

  edm::Handle< edm::DetSetVector<TotemRPLocalTrack> > rplt;
  iEvent.getByToken(localTrackToken_, rplt);
  for (edm::DetSetVector<TotemRPLocalTrack>::const_iterator rp=rplt->begin(); rp!=rplt->end(); rp++) {
    const unsigned int det_id = rp->detId();
    const unsigned short arm  = (det_id%100==2); // 0->F, 1->N (003/103->F, 002/102->N)
    const unsigned short side = (det_id/100==1); // 0->L, 1->R (002/003->L, 102/103->R)
    for (edm::DetSet<TotemRPLocalTrack>::const_iterator trk=rp->begin(); trk!=rp->end(); trk++) {
      if (!trk->isValid()) continue;
      aTrackX.push_back(trk->getX0()*1.e3);
      aTrackY.push_back(trk->getY0()*1.e3);
      aTrackZ.push_back(trk->getZ0()*1.e3);
      aTrackArm.push_back(arm);
      aTrackSide.push_back(side);
    }
  }
}


// ------------ method called once each job just before starting event loop  ------------
void 
TreeProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TreeProducer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TreeProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TreeProducer);
