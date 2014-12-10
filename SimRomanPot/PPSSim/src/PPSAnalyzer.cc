// -*- C++ -*-
//
// Package:    PPSAnalyzer
// Class:      PPSAnalyzer
// 
/**\class PPSAnalyzer PPSAnalyzer.cc SimRomanPot/PPSSim/src/PPSAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Luiz Martins Mundim Filho,22 1-020,+41227677686,
//         Created:  Thu Jan 31 11:10:02 CET 2013
// $Id$
//
//


// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Particle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
//
#include "SimRomanPot/PPSSim/interface/PPSSim.h"
#include "TFile.h"
#include "TTree.h"
//
// class declaration
//

class PPSAnalyzer : public edm::EDAnalyzer {
   public:
      explicit PPSAnalyzer(const edm::ParameterSet&);
      ~PPSAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
      bool    verbosity;
      string  outFileName;
      PPSSim* pps;
      TFile*  fOutputFile;
      TTree*  tree;
      PPSSpectrometer* fGen;
      PPSSpectrometer* fSim;
      PPSSpectrometer* fReco;
      edm::InputTag    gensrc;
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
PPSAnalyzer::PPSAnalyzer(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
//
   string beam1filename       = iConfig.getParameter<string>("Beam1File");
   string beam2filename       = iConfig.getParameter<string>("Beam2File");
   int    beam1dir            = iConfig.getParameter<int>("Beam1Direction");
   int    beam2dir            = iConfig.getParameter<int>("Beam2Direction");
   bool   showbeam            = iConfig.getUntrackedParameter<bool>("ShowBeamLine",false);
   bool   simbeam             = iConfig.getUntrackedParameter<bool>("SimBeamProfile",false);
   double fVtxMeanX           = iConfig.getUntrackedParameter<double>("VtxMeanX",0.);
   double fVtxMeanY           = iConfig.getUntrackedParameter<double>("VtxMeanY",0.);
   double fVtxMeanZ           = iConfig.getUntrackedParameter<double>("VtxMeanZ",0.);
   string ip                  = iConfig.getParameter<string>("CollisionPoint");
          verbosity           = iConfig.getUntrackedParameter<int>("Verbosity",0);
   double fTrackerLength      = iConfig.getParameter<double>("TrackerLength");
   double fTrackerPosition    = iConfig.getParameter<double>("TrackerPosition");
   double fTrk1XOffset        = iConfig.getParameter<double>("TrkDet1XOffset");
   double fTrk2XOffset        = iConfig.getParameter<double>("TrkDet2XOffset");
   double fToFPosition        = iConfig.getParameter<double>("ToFPosition");
   double fTCL4Position       = iConfig.getUntrackedParameter<double>("TCL4Position",0.);
   double fTCL5Position       = iConfig.getUntrackedParameter<double>("TCL5Position",0.);
   bool   fSmearHit           = iConfig.getParameter<bool>("SmearHit");
   double fHitSigmaX          = iConfig.getParameter<double>("HitSigmaX");
   double fHitSigmaY          = iConfig.getParameter<double>("HitSigmaY");
   //double fHitSigmaZ          = iConfig.getParameter<double>("HitSigmaZ");
   double fTimeSigma          = iConfig.getParameter<double>("TimeSigma");
   bool   fSmearAngle         = iConfig.getParameter<bool>("SmearAngle");
   double fBeamAngleRMS       = iConfig.getParameter<double>("BeamAngleRMS");
   bool   fSmearEnergy        = iConfig.getParameter<bool>("SmearEnergy");
   double fBeamEnergyRMS      = iConfig.getParameter<double>("BeamEnergyRMS");
   double fPhiMin             = iConfig.getParameter<double>("PhiMin");
   double fPhiMax             = iConfig.getParameter<double>("PhiMax");
   double fCentralMass        = iConfig.getParameter<double>("CentralMass");
   double fCentralMassErr     = iConfig.getParameter<double>("CentralMassErr");
   bool   fKickersOFF         = iConfig.getParameter<bool>("KickersOFF");
   bool   fCrossAngleCorr     = iConfig.getParameter<bool>("CrossAngleCorr");
   double fCrossingAngle      = iConfig.getParameter<double>("CrossingAngle");
   double fEtaMin             = iConfig.getParameter<double>("EtaMin");
   double fMomentumMin        = iConfig.getParameter<double>("MomentumMin");
   double fImpParcut          = iConfig.getParameter<double>("TrackImpactParameterCut"); // exclude hit combination that lead to high imp. par. reco tracks (in cm)
   double fMinthx             = iConfig.getParameter<double>("MinThetaXatDet1"); // minimum thetaX at first tracker detector (in urad)
   double fMaxthx             = iConfig.getParameter<double>("MaxThetaXatDet1"); // maximum thetaX at first tracker detector (in urad)
   double fMinthy             = iConfig.getParameter<double>("MinThetaYatDet1"); // minimum thetaY at first tracker detector (in urad)
   double fMaxthy             = iConfig.getParameter<double>("MaxThetaYatDet1"); // maximum thetaY at first tracker detector (in urad)
   double fMaxXfromBeam       = iConfig.getParameter<double>("MaxXfromBeam");    // maximum distance (X) from beam a hit is accepted (in mm, negative)
   double fMaxYfromBeam       = iConfig.getParameter<double>("MaxYfromBeam");    // maximum distance (Y) from beam a hit is accepted (in mm, positive, simetric)
   double fDetectorClosestX   = iConfig.getParameter<double>("DetectorClosestX");// minimum distance (X) from beam a hit is accepted (in mm, negative)
   bool   fFilterHitMap       = iConfig.getParameter<bool>("FilterHitMap");       // apply geometrical cuts in the hit position (RP window+distance from beam)
          outFileName         = iConfig.getParameter<string>("OutputFile");
          gensrc              = iConfig.getUntrackedParameter<edm::InputTag>("genSource",edm::InputTag("genParticles"));

   pps = new PPSSim(true); // instanciate PPSSim with External Generator
   pps->set_KickersOFF(fKickersOFF);
   pps->set_BeamLineFile(beam1filename,beam2filename);
   pps->set_BeamDirection(beam1dir,beam2dir);
   pps->set_BeamEnergySmearing(fSmearEnergy);
   pps->set_BeamEnergyRMS(fBeamEnergyRMS);
   pps->set_BeamAngleSmearing(fSmearAngle);
   pps->set_BeamAngleRMS(fBeamAngleRMS);
   pps->set_TCLPosition("TCL4",fTCL4Position,fTCL4Position);
   pps->set_TCLPosition("TCL5",fTCL5Position,fTCL5Position);
   if (showbeam) pps->set_ShowBeamLine();
   if (simbeam)  pps->set_GenBeamProfile();
   pps->set_VertexPosition(fVtxMeanX,fVtxMeanY,fVtxMeanZ);
   pps->set_CollisionPoint(ip);
   pps->set_TrackerPosition(fTrackerPosition);
   pps->set_TrackerLength(fTrackerLength);
   pps->set_ToFPosition(fToFPosition);
   pps->set_ToFResolution(fTimeSigma);
   pps->set_TrackerMisAlignment(fTrk1XOffset,fTrk2XOffset,fTrk1XOffset,fTrk2XOffset); // use the same offset for the forward and backward arm
   pps->set_HitSmearing(fSmearHit);
   pps->set_VertexSmearing(false); // when using cmssw, vertex smearing is done somewhere else
   pps->set_phiMin(fPhiMin);
   pps->set_phiMax(fPhiMax);
   pps->set_etaMin(fEtaMin);
   pps->set_momentumMin(fMomentumMin);
   pps->set_CentralMass(fCentralMass,fCentralMassErr);
   pps->set_HitSmearing(fSmearHit);
   pps->set_TrackerResolution((fHitSigmaX+fHitSigmaY)/2.*um_to_mm);
   pps->set_Verbose(verbosity);
   pps->set_CrossingAngleCorrection(fCrossAngleCorr);
   pps->set_CrossingAngle(fCrossingAngle);
   pps->set_TrackImpactParameterCut(fImpParcut);
   pps->set_ThetaXRangeatDet1(fMinthx,fMaxthx);
   pps->set_ThetaYRangeatDet1(fMinthy,fMaxthy);
   pps->set_WindowForTrack(fMaxXfromBeam,fMaxYfromBeam,fDetectorClosestX);
   pps->set_FilterHitMap(fFilterHitMap);
}


PPSAnalyzer::~PPSAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void PPSAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace HepMC;
   using namespace CLHEP;

   pps->BeginEvent();
/*
   edm::Handle< std::vector<reco::GenParticle> > gpVec;
   iEvent.getByLabel("genParticles",gpVec);
   std::vector<reco::GenParticle> part=*gpVec;
   edm::Handle<reco::GenParticleCollection> genParticles;
   iEvent.getByLabel(gensrc,genParticles);
   try{
      iEvent.getByLabel("genParticles",genParticles);
      }
   catch(const Exception&) {
      std::cout<<"PPSAnalyzer::analyze: No genParticles informations" << std::endl;
      return;
   }
  int nPart = 0;
  for (reco::GenParticleCollection::const_iterator iter=genParticles->begin();iter!=genParticles->end();++iter){
    std::cout << (*iter).status() << std::endl;
    
    if ( (*iter).status() == 1 && (*iter).pdgId() == 2212 ) {
      nPart++;
      double nEcms = (*iter).energy();
      std::cout << nEcms << std::endl;
    }
    if ( (*iter).status() == 1) {
        std::cout << "Status 1 part # " << std::setw(4) << std::fixed << nPart
                  << std::setw(14) << std::fixed << (*iter).pdgId()
                  << std::setw(14) << std::fixed << (*iter).px()
                  << std::setw(14) << std::fixed << (*iter).py()
                  << std::setw(14) << std::fixed << (*iter).pz() << std::endl;
      }
    }
*/
   Handle<HepMCProduct> genHandle;
   try{
      iEvent.getByLabel("generator",genHandle);
      }
   catch(const Exception&){
      std::cout<<"PPSAnalyzer::analyze: No MC information" << std::endl;
      return;
   }
   const HepMC::GenEvent* evt = genHandle->GetEvent();
//
   pps->ReadGenEvent(evt);
   pps->Run();
   pps->EndEvent();
   fGen = pps->get_GenDataHolder();
   fSim = pps->get_SimDataHolder();
   fReco= pps->get_RecoDataHolder();
   tree->Fill();
}

// ------------ method called once each job just before starting event loop  ------------
void PPSAnalyzer::beginJob()
{
     fOutputFile = new TFile(outFileName.c_str(),"recreate");
     tree = new TTree("T","T");
     fGen = new PPSSpectrometer();
     fSim = new PPSSpectrometer();
     fReco= new PPSSpectrometer();
     tree->Branch("Gen.","PPSSpectrometer",&fGen);
     tree->Branch("Sim.","PPSSpectrometer",&fSim);
     tree->Branch("Reco.","PPSSpectrometer",&fReco);
     if (pps) pps->BeginRun();
}

// ------------ method called once each job just after ending the event loop  ------------
void PPSAnalyzer::endJob() 
{
      tree->Write();
      std::cout << "PPSAnalyzer::endRun: " << tree->GetEntries() << " events produced." << std::endl;
      fOutputFile->Close();
}

// ------------ method called when starting to processes a run  ------------
void PPSAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void PPSAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
      if(pps) pps->EndRun();
}

// ------------ method called when starting to processes a luminosity block  ------------
void PPSAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void PPSAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void PPSAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PPSAnalyzer);
