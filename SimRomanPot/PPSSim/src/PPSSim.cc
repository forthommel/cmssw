// ROOT #includes
#include "SimRomanPot/PPSSim/interface/PPSSim.h"
#include <TMatrixD.h>
//=====================================================================================================
PPSSim::PPSSim(bool ext_gen): fExternalGenerator(ext_gen),
               fVerbose(false),NEvent(0),fGenMode(""),
               fBeamLine1File(""),fBeamLine2File(""),fBeam1Direction(1),fBeam2Direction(1),fShowBeamLine(false),
               fCollisionPoint(""),fBeamLineLength(500),fBeamEnergy(0),fBeamMomentum(0),
               fCrossingAngle(0.),fCrossAngleCorr(false),fKickersOFF(false),
               fDetectorClosestX(-2.),fMaxXfromBeam(-25),fMaxYfromBeam(10),
               fTrackerPosition(0.),fTrackerLength(0.),fToFPosition(0.),
               fTCL4Position1(0.),fTCL4Position2(0.),fTCL5Position1(0.),fTCL5Position2(0.),
               fSmearVertex(false),fVtxMeanX(0.),fVtxMeanY(0.),fVtxMeanZ(0.),fVtxSigmaX(0.),fVtxSigmaY(0.),fVtxSigmaZ(0.),
               fSmearHit(1.),fHitSigmaX(0.),fHitSigmaY(0.),fHitSigmaZ(0.),fTimeSigma(0.),
               fDet1XOffsetF(0.),fDet2XOffsetF(0.),fDet1XOffsetB(0.),fDet2XOffsetB(0.),
               fSmearAngle(false),fBeamAngleRMS(0.),fSmearEnergy(false),fBeamEnergyRMS(0.),
               fFilterHitMap(true),fTrackImpactParameterCut(0.),fMinThetaXatDet1(-200),fMaxThetaXatDet1(200),
               fMinThetaYatDet1(-200),fMaxThetaYatDet1(200),
               xi_min(0.),xi_max(0.),t_min(0.),t_max(0.),fPhiMin(-TMath::Pi()),fPhiMax(TMath::Pi()),
               fEtaMin(7.),fMomentumMin(3000.),fCentralMass(0.),fCentralMassErr(0.),
               CheckPoints(),fSimBeam(false)
{
     beam1profile=NULL;
     beam2profile=NULL;
     gRandom3  = new TRandom3(0);
}
void PPSSim::BeginRun()
{
     extern int kickers_on;
     kickers_on = (fKickersOFF)?0:1;
     beamlineF = new H_BeamLine(-1,fBeamLineLength);
     beamlineB = new H_BeamLine( 1,fBeamLineLength);
     beamlineF->fill(fBeamLine2File,fBeam2Direction,fCollisionPoint);
     beamlineB->fill(fBeamLine1File,fBeam1Direction,fCollisionPoint);
//
// The offset is always positive, since both outgoind beamline is in the outside of the reference orbit
//
/*
     std::map<std::string,double> strengths;
     std::map<std::string,double> betaX;
     std::map<std::string,double> betaY;
     std::map<std::string,double> DX;
     std::map<std::string,double> DY;
     ifstream in("beta90m.par");
     if (!in) exit(2);
     std::string opt_elm;
     double str;
     double betax, betay, dx, dy;
     while(!in.eof()) {
        in >> opt_elm >> betax >> betay >> dx >> dy >> str;
        strengths[opt_elm] = str;
        betaX[opt_elm] = betax;
        betaY[opt_elm] = betay;
        DX[opt_elm] = dx;
        DY[opt_elm] = dy;
     }
*/
     beamlineF->offsetElements( 120, 0.097);
     beamlineB->offsetElements( 120, 0.097);
     beamlineF->calcMatrix(); beamlineB->calcMatrix();
/*
     std::cout << "===========================================================================================================================" << endl;
     std::cout << "BeamLine F optical functions: " << endl;
     for(int i=0;i<beamlineF->getNumberOfElements();i++){
        H_OpticalElement* optE = beamlineF->getElement(i);
        std::cout << optE->getName() << " " << optE->getBetaX() << " " << optE->getBetaY() << " " << optE->getDX() << " " << optE->getDY() << " " << optE->getK() << std::endl;
     }
     std::cout << "BeamLine B optical functions: " << endl;
     for(int i=0;i<beamlineB->getNumberOfElements();i++){
        H_OpticalElement* optE = beamlineB->getElement(i);
        std::cout << optE->getName() << " " << optE->getBetaX() << " " << optE->getBetaY() << " " << optE->getDX() << " " << optE->getDY() << " " << optE->getK() << std::endl;
     }
     std::cout << "===========================================================================================================================" << endl;
*/
/*
     for(int i=0;i<beamlineF->getNumberOfElements();i++){
        H_OpticalElement* optE = beamlineF->getElement(i);
        std::string type = optE->getTypeString();
        if (type.find("Dipole")<type.length()||type.find("Quadrupole")<type.length()) {
           std::cout << "Type: " << optE->getTypeString() << " Name: " << optE->getName()
                     << " BetaX : " << optE->getBetaX() << " BetaY : " << optE->getBetaY()
                     << " DX : " << optE->getDX() << " DY : " << optE->getDY()
                     << " k : " << optE->getK() << endl;
           std::string name = optE->getName();
           optE->setK(strengths[name]);
           optE->setBetaX(betaX[name]);
           optE->setBetaY(betaY[name]);
           optE->setDX(DX[name]);
           optE->setDY(DY[name]);
           std::cout << "Type: " << optE->getTypeString() << " Name: " << optE->getName()
                     << " BetaX : " << optE->getBetaX() << " BetaY : " << optE->getBetaY()
                     << " DX : " << optE->getDX() << " DY : " << optE->getDY()
                     << " k : " << optE->getK() << endl;
        }
     }
     for(int i=0;i<beamlineB->getNumberOfElements();i++){
        H_OpticalElement* optE = beamlineB->getElement(i);
        std::string type = optE->getTypeString();
        if (type.find("Dipole")<type.length()||type.find("Quadrupole")<type.length()) {
           std::cout << "Type: " << optE->getTypeString() << " Name: " << optE->getName()
                     << " BetaX : " << optE->getBetaX() << " BetaY : " << optE->getBetaY()
                     << " DX : " << optE->getDX() << " DY : " << optE->getDY()
                     << " k : " << optE->getK() << endl;
           std::string name = optE->getName();
           optE->setK(strengths[name]);
           optE->setBetaX(betaX[name]);
           optE->setBetaY(betaY[name]);
           optE->setDX(DX[name]);
           optE->setDY(DY[name]);
           std::cout << "Type: " << optE->getTypeString() << " Name: " << optE->getName()
                     << " BetaX : " << optE->getBetaX() << " BetaY : " << optE->getBetaY()
                     << " DX : " << optE->getDX() << " DY : " << optE->getDY()
                     << " k : " << optE->getK() << endl;
        }
     }
*/
     if (fShowBeamLine) {
        std::cout << "====================================================================\n"
                  << "                  Forward beam line elements \n" << std::endl;
        beamlineF->showElements();
        std::cout << "====================================================================\n"
                  << "                 Backward beam line elements \n" << std::endl;
        beamlineB->showElements();
     }
// Create a particle to get the beam energy from the beam file
     H_BeamParticle *beam = new H_BeamParticle(ProtonMass,1);
     fBeamEnergy = beam->getE(); // redefine globally the beam energy
     std::cout << "BEAM ENERGY = " << fBeamEnergy << std::endl;
     fBeamMomentum = sqrt(fBeamEnergy*fBeamEnergy - ProtonMassSQ);
     delete beam;
//
     pps_stationF = new H_RecRPObject(fTrackerPosition,fTrackerPosition+fTrackerLength,*beamlineF);
     pps_stationB = new H_RecRPObject(fTrackerPosition,fTrackerPosition+fTrackerLength,*beamlineB);
//
// check the kinematic limits in case it is requested to generate the higgs mass in the central system
     if (fGenMode=="M_X") { // check the kinematic limits
        if (xi_min*(2.*fBeamEnergy)>fCentralMass+fCentralMassErr||xi_max*(2.*fBeamEnergy)<fCentralMass-fCentralMassErr) {
           std::cout << "xi limits outside kinematic limits for the given Central mass. Stopping..." << std::endl;
           exit(1);
        }
     }
     if (fSimBeam) {
        TH2F* h1 = new TH2F(*GenBeamProfile(-fTrackerPosition));
        TH2F* h2 = new TH2F(*GenBeamProfile(fTrackerPosition));
        if (h1) {h1->SetName("BeamProfileB_1"); h1->Write(); delete h1;}
        if (h2) {h2->SetName("BeamProfileF_1"); h2->Write(); delete h2;}
        h1 = GenBeamProfile(-(fTrackerPosition+fTrackerLength));
        h2 = GenBeamProfile(  fTrackerPosition+fTrackerLength);
        if (h1) {h1->SetName("BeamProfileB_2"); h1->Write(); delete h1;}
        if (h2) {h2->SetName("BeamProfileF_2"); h2->Write(); delete h2;}
        TH2F* ht = GenBeamProfile(fToFPosition);
        if (ht) {ht->SetName("BeamProfileB_ToF"); ht->Write(); delete ht;}
        ht = GenBeamProfile(-fToFPosition);
        if (ht) {ht->SetName("BeamProfileF_ToF"); ht->Write(); delete ht;}
// if colimator position not defined, try to get it from the beamline
        if (fTCL4Position1*fTCL4Position2==0) {
           H_OpticalElement* tcl = beamlineB->getElement("TCL.4R5.B1"); if (tcl) fTCL4Position1=tcl->getS();
           tcl = beamlineF->getElement("TCL.4L5.B2");if (tcl) fTCL4Position2=tcl->getS();
        }
        if (fTCL5Position1*fTCL5Position2==0) {
           H_OpticalElement* tcl = beamlineB->getElement("TCL.5R5.B1"); if (tcl) fTCL5Position1=tcl->getS();
           tcl = beamlineF->getElement("TCL.5L5.B2");if (tcl) fTCL5Position2=tcl->getS();
           tcl=beamlineF->getElement("TCL.5R5.B1");
        }
        if (fTCL4Position1*fTCL4Position2>0){
           TH2F* h1 = GenBeamProfile(-fTCL4Position1); // use negative position to tell gen. which beamline to chose
           fBeam1PosAtTCL4=make_pair<double,double>(h1->GetMean(1),h1->GetMean(2));
           fBeam1RMSAtTCL4=make_pair<double,double>(h1->GetRMS(1),h1->GetRMS(2));
           TH2F* h2 = GenBeamProfile( fTCL4Position2);
           fBeam2PosAtTCL4=make_pair<double,double>(h2->GetMean(1),h2->GetMean(2));
           fBeam2RMSAtTCL4=make_pair<double,double>(h2->GetRMS(1),h2->GetRMS(2));
        }
        if (fTCL5Position1*fTCL5Position2>0){
           TH2F* h1 = GenBeamProfile(-fTCL5Position1); // use negative position to tell gen. which beamline to chose
           fBeam1PosAtTCL5=make_pair<double,double>(h1->GetMean(1),h1->GetMean(2));
           fBeam1RMSAtTCL5=make_pair<double,double>(h1->GetRMS(1),h1->GetRMS(2));
           TH2F* h2 = GenBeamProfile( fTCL5Position2);
           fBeam2PosAtTCL5=make_pair<double,double>(h2->GetMean(1),h2->GetMean(2));
           fBeam2RMSAtTCL5=make_pair<double,double>(h2->GetRMS(1),h2->GetRMS(2));
        }
     }
// 
     fGen = new PPSSpectrometer();
     fGen = new PPSSpectrometer();
     fSim = new PPSSpectrometer();
     fReco= new PPSSpectrometer();
//
//   Check the overall kinematic limits
     if (fGenMode=="M_X") {
        if (xi_min*(2.*fBeamEnergy)>fCentralMass+fCentralMassErr||xi_max*(2.*fBeamEnergy)<fCentralMass-fCentralMassErr) {
           std::cout << "xi limits outside kinematic limits for the given Central mass. Stopping..." << std::endl;
           exit(1);
        }
     }
}

//
void PPSSim::EndRun()
{
}
void PPSSim::BeginEvent()
{
     fGen->clear();
     fSim->clear();
     fReco->clear();
     protonF = NULL;
     protonB = NULL;
     fHasStoppedF = false;
     fHasStoppedB = false;
     //protonF.SetPxPyPzE(0.,0.,0.,0.);
     //protonB.SetPxPyPzE(0.,0.,0.,0.);
     NVertex=0;
     fVertex.clear();
     protonsOut.clear();
     fHasStopped.clear();
}
void PPSSim::EndEvent()
{
     for(int i=0;i<NVertex;i++) {
        protonF = (protonsOut[i].first); protonB = (protonsOut[i].second);
        if (protonF) delete protonF;
        if (protonB) delete protonB;
     }
     protonsOut.clear();
     protonF=NULL;
     protonB=NULL;
}
void PPSSim::Run() {
     if (!fExternalGenerator) Generation();
     Simulation();
     Digitization(); // a fake digitization procedure
     Reconstruction();
}
void PPSSim::Generation()
{
// Uses the CMS units in the vertex and mm for the PPS parameters
// sorts a proton in the forward direction
     double t1,xi1,phi1;
     double t2,xi2,phi2;
     if (fGenMode=="M_X") {
        if (fCentralMass==0) {
           std::cout << "PPSSim::Generation: Central mass not defined. Exiting..." << std::endl;
           exit(1);
        }
        GenCentralMass(t1,t2,xi1,xi2,phi1,phi2);
     }
     else {
        GenSingleParticle(t1,xi1,phi1);
        GenSingleParticle(t2,xi2,phi2);
     }
     int nvtx=add_Vertex(fVtxMeanX,fVtxMeanY,fVtxMeanZ);
     protonF=new TLorentzVector(shoot(t1,xi1,phi1,1)); //  1 = positive/forward direction
     protonB=new TLorentzVector(shoot(t2,xi2,phi2,-1));
     add_OutgoingParticle(nvtx-1,protonF,protonB);
     fHasStoppedF=false;fHasStoppedB=false;
     set_GenData();
}
//
// fill the tree structures 
void PPSSim::ReadGenEvent(const std::vector<reco::GenParticle>* genP)
{
     if (!genP) return;
     int ivtx = -1;
     int colId = -1;
     double momB = 0.;
     double momF = 0.;
     TLorentzVector* pF = NULL;
     TLorentzVector* pB = NULL;
     double vtxX=0.;
     double vtxY=0.;
     double vtxZ=0.;
     for(size_t i=0;i<genP->size();++i) {
        const reco::GenParticle& p = (*genP)[i];
        if (p.pdgId()!=2212) continue;
	if (p.status()  !=1) continue;
        //double pz = p.pt()*sinh (p.eta());
        double px = p.px();double py=p.py();double pz = p.pz();
        if (ivtx<0) {
           ivtx=0;
           vtxX=p.vx();vtxY=p.vy();vtxZ=p.vz();// Contrary to HepMC, reco::genParticle already uses cm, so no convertion is needed
           colId = p.collisionId();
        }

        if (colId!=p.collisionId()) {
           if (fVerbose) {
              cout << "Vertex position: " << vtxX << " " << vtxY << " " << vtxZ << endl;
              if (pF) pF->Print();
              if (pB) pB->Print();
           }
           int nvtx=add_Vertex(vtxX,vtxY,vtxZ);
           if (ivtx!=nvtx-1) {cout << "WARNING: unexpected vertex number." << endl;}
           add_OutgoingParticle(nvtx-1,pF,pB);
           colId = p.collisionId();
           vtxX=p.vx();vtxY=p.vy();vtxZ=p.vz();
           ivtx++;
           momF=0.; momB=0.;
           pF=NULL;pB=NULL;
        } else {
// verify the vertex consistency
           if (vtxX!=p.vx()||vtxY!=p.vy()||vtxZ!=p.vz()) {
              cout << "WARNING: unexpected new vertex position" << endl;
           }
        }
        if (p.eta()>0&&momF<pz) {
           momF=pz; pF = new TLorentzVector(px,py,pz,sqrt(px*px+py*py+pz*pz+ProtonMassSQ));
        } else if (p.eta()<0&&momB<fabs(pz)) {
           momB=fabs(pz);pB = new TLorentzVector(px,py,pz,sqrt(px*px+py*py+pz*pz+ProtonMassSQ));
        }
// this is the  last particle, add it anyway..
        if (i==genP->size()-1) {
           int nvtx=add_Vertex(vtxX,vtxY,vtxZ);
           if (ivtx!=nvtx-1) {cout << "WARNING: unexpected vertex number." << endl;}
           if (fVerbose) {if(pF) pF->Print();if (pB) pB->Print();}
           add_OutgoingParticle(nvtx-1,pF,pB);
        }
     }
     set_GenData();
}
void PPSSim::ReadGenEvent(const HepMC::GenEvent* evt)
{
   using namespace CLHEP;
   //TLorentzVector* pOut = NULL;
   TLorentzVector* pF = NULL;
   TLorentzVector* pB = NULL;
   if (!evt) return;
   int nvtx =0;
   double vtxX = 0.;
   double vtxY = 0.;
   double vtxZ = 0.;
   for(HepMC::GenEvent::vertex_const_iterator ivtx = evt->vertices_begin();ivtx!=evt->vertices_end();ivtx++) {
      if (fVerbose) (*ivtx)->print();
      if ((*ivtx)->id()!=0) continue;
      vtxX = (*ivtx)->position().x()/cm; // CMS uses cm but HepMC uses mm
      vtxY = (*ivtx)->position().y()/cm;
      vtxZ = (*ivtx)->position().z()/cm;

// choose the highest momentum particle on each side to be propagated
      double momF = 0.; double momB = 0.;

      for(HepMC::GenVertex::particles_out_const_iterator pItr = (*ivtx)->particles_out_const_begin();
                                                            pItr!= (*ivtx)->particles_out_const_end();pItr++) {
         if (fVerbose) (*pItr)->print();
         if ((*pItr)->status() != 1) continue; // this is not a final state particle
         if ((*pItr)->pdg_id()!=2212) continue; // only protons to be processed
         if (fabs((*pItr)->momentum().eta()) < fEtaMin) continue; 
         if ((*pItr)->momentum().e()<fMomentumMin) continue; 
         if ((*pItr)->momentum().pz()>0&&(*pItr)->momentum().pz()>momF) {
            momF= (*pItr)->momentum().pz();
            pF  = new TLorentzVector((*pItr)->momentum().px(),(*pItr)->momentum().py(),(*pItr)->momentum().pz(),
                  sqrt(ProtonMassSQ+pow((*pItr)->momentum().px(),2)+pow((*pItr)->momentum().py(),2)+pow((*pItr)->momentum().pz(),2)));
         } else if ((*pItr)->momentum().pz()<0&&fabs((*pItr)->momentum().pz())>momB){
               momB = fabs((*pItr)->momentum().pz());
               pB   = new TLorentzVector((*pItr)->momentum().px(),(*pItr)->momentum().py(),(*pItr)->momentum().pz(),
                      sqrt(ProtonMassSQ+pow((*pItr)->momentum().px(),2)+pow((*pItr)->momentum().py(),2)+pow((*pItr)->momentum().pz(),2)));
         }
      }
   }
   if (!pF&&!pB) return;
   nvtx=add_Vertex(vtxX,vtxY,vtxZ);
   add_OutgoingParticle(nvtx-1,pF,pB);
   if (fVerbose) {
      if (pF) {pF->Print();}
      if (pB) {pB->Print();}
   }
   set_GenData();
}
void PPSSim::set_GenData()
{
//     PPSData* arm=NULL;
     for(int i=0;i<NVertex;i++) {
        fGen->AddVertex(fVertex[i].x(),fVertex[i].y(),fVertex[i].z());
//
// protons->first = protonsF
// protons->second = protonsB
//
        protonF = (protonsOut[i].first); protonB = (protonsOut[i].second);
        if (fVerbose) {
           cout << "Vertex position:  " << fVertex[i].x() << " " << fVertex[i].y() << "  "  << fVertex[i].z() << endl;
           cout << "Outgoing protons: "; if (protonF) protonF->Print();else {cout << endl;}
           cout << "                  "; if (protonB) protonB->Print();else {cout << endl;}
        }
        TLorentzVectorToPPSData(&(fGen->ArmF),protonF);
        TLorentzVectorToPPSData(&(fGen->ArmB),protonB);
        
        if (protonF){
           ApplyBeamSmearing(const_cast<TLorentzVector&>(*protonF));
        }
        if (protonB) {
           ApplyBeamSmearing(const_cast<TLorentzVector&>(*protonB));
        }
     }
}
void PPSSim::TLorentzVectorToPPSData(PPSData* arm,const TLorentzVector* proton)
{
     double mom=0.;
     double energy=0.;
     double theta =0.;
     double phi   =0;
     double t     =0.;
     double xi    =0.;
     double pT    =0.;

     if (proton) {
        mom    = proton->P();
        if (mom>fBeamMomentum) mom=fBeamMomentum;
        energy = proton->E();
        phi    = proton->Phi();
        theta  = (proton->Pz()>0)?proton->Theta():TMath::Pi()-proton->Theta();
        t      = -2.*(ProtonMassSQ-fBeamEnergy*energy+fBeamMomentum*mom*cos(theta));
        xi     = (1.0-energy/fBeamEnergy);
        pT     = proton->Pt();
     }
     arm->t.push_back(t);
     arm->xi.push_back(xi);
     arm->phi.push_back(phi);
     arm->theta.push_back(theta);
     arm->momentum.push_back(mom);
     arm->pT.push_back(pT);
}
void PPSSim::Simulation()
{
     for(int i=0;i<NVertex;i++) {
        double vtxX=fVertex[i].x();
        double vtxY=fVertex[i].y();
        double vtxZ=fVertex[i].z();
        protonF = (protonsOut[i].first); protonB = (protonsOut[i].second);
        fHasStoppedF=false;fHasStoppedB=false;
        //SmearVertexPosition(vtxX,vtxY,vtxZ);
// At this point, one should be using the CMS units (cm)
        fSim->AddVertex(vtxX,vtxY,vtxZ);
// FIRST, propagate to the positive(forward) direction, then to the other side
//
// Propagate until PPS, filling pps_station accordingly for the reconstruction if needed
// Remember: 
//         HECTOR uses um for X and Y coordinates and m for Z
//         is backward for LHC, which means when propagating to the CMS forward direction, one needs to rotate it
//         by means of doing x_LHC = -x_CMS and z = -z 

        H_BeamParticle *part = NULL;

        if (protonF) {
           int Direction = 1;
           TLorentzVectorToPPSData(&(fSim->ArmF),protonF);
//
// TLorentzVectorToPPSData only copies the kinematic variables, so add the impact parameter and station Id
           fSim->ArmF.RatIP.push_back(sqrt(pow(vtxX-fVtxMeanX,2)+pow(vtxY-fVtxMeanY,2)));
           int station=(fTrackerPosition>=300)? 1:3;
           fSim->ArmF.stationId.push_back(station);
//
           if (fCrossAngleCorr) LorentzBoost(const_cast<TLorentzVector&>(*protonF),"LAB");
//
           part = new H_BeamParticle(ProtonMass,1);
           part->setPosition(-(vtxX-fVtxMeanX)*cm_to_um,(vtxY-fVtxMeanY)*cm_to_um,0.,0.,-(vtxZ)*cm_to_m);
           part->set4Momentum(-protonF->Px(),protonF->Py(),-protonF->Pz(),protonF->E());
           part->computePath(beamlineF);
           Propagate(part,Direction);
           if (part) {delete part;part = NULL;}
        } else {
           fSim->ArmF.TrkDet1.AddHit(0,0,0,0,0,1);
           fSim->ArmF.TrkDet2.AddHit(0,0,0,0,0,1);
           fSim->ArmF.ToFDet.AddHit(0,0,0,0,0,1);
           fSim->ArmF.Fill();
        }
//
//  Propagate to the negative/backward direction
//
        if (protonB) {
           int Direction = -1;
           TLorentzVectorToPPSData(&(fSim->ArmB),protonB);
//
// TLorentzVectorToPPSData only copies the kinematic variables, so add the impact parameter and station Id
           fSim->ArmB.RatIP.push_back(sqrt(pow(vtxX-fVtxMeanX,2)+pow(vtxY-fVtxMeanY,2)));
           int station=(fTrackerPosition>=300)? 2:4;
           fSim->ArmB.stationId.push_back(station);
//
           if (fCrossAngleCorr) LorentzBoost(const_cast<TLorentzVector&>(*protonB),"LAB");
           part = new H_BeamParticle(ProtonMass,1);
           part->setPosition(-(vtxX-fVtxMeanX)*cm_to_um,(vtxY-fVtxMeanY)*cm_to_um,0.,0.,-(vtxZ)*cm_to_m);
           part->set4Momentum(-protonB->Px(),protonB->Py(),-protonB->Pz(),protonB->E()); // HECTOR uses always positive z momentum
           part->computePath(beamlineB);
           Propagate(part,Direction);
           if (part) {delete part;part = NULL;}
        } else {
           fSim->ArmB.TrkDet1.AddHit(0,0,0,0,0,1);
           fSim->ArmB.TrkDet2.AddHit(0,0,0,0,0,1);
           fSim->ArmB.ToFDet.AddHit(0,0,0,0,0,1);
           fSim->ArmB.Fill();
        }
     }
}
void PPSSim::Reconstruction()
{
     int Direction;
     Direction=1;
     TrackerReco(Direction,pps_stationF,&(fReco->ArmF));
     Direction=-1;
     TrackerReco(Direction,pps_stationB,&(fReco->ArmB));
     ToFReco();
}
bool PPSSim::SearchTrack(int i,int j,int Direction,H_RecRPObject* station,PPSData* arm, int& stationId,double& xi,double& t,double& partP,double& pt,double& phi,double& theta,double& ImpPar)
{
     stationId = 0;
     xi = 0; t=0; partP=0; pt=0; phi=0; theta=0; ImpPar=0;
     //
     if (arm->TrkDet1.NHits<=i||arm->TrkDet2.NHits<=j) return false;
     if (arm->TrkDet1.HasStopped.at(i)==1||arm->TrkDet2.HasStopped.at(j)==1) return false;
//
     double x1 = arm->TrkDet1.X.at(i); double y1 = arm->TrkDet1.Y.at(i);
     double x2 = arm->TrkDet2.X.at(j); double y2 = arm->TrkDet2.Y.at(j);
     double thx = atan((x2-x1)*mm_to_cm/(fTrackerLength*m_to_cm))/urad;
     double thy = atan((y2-y1)*mm_to_cm/(fTrackerLength*m_to_cm))/urad;
     if (thx>fMaxThetaXatDet1||thx<fMinThetaXatDet1) return false;
     if (thy>fMaxThetaYatDet1||thy<fMinThetaYatDet1) return false;
     double tx,ty,eloss;
     ReconstructArm(station, x1,y1,x2,y2,tx,ty,eloss);
     double x0 = -station->getX0()*um_to_cm;
     double y0 = station->getY0()*um_to_cm;
     ImpPar=sqrt(x0*x0+y0*y0);
     if (fTrackImpactParameterCut>0.) {
        if (ImpPar>fTrackImpactParameterCut) return false;
     }
     if (eloss<0&&eloss>fBeamEnergy) return false;
     theta = sqrt(tx*tx+ty*ty)*urad;
     xi    = eloss/fBeamEnergy;
     double energy= fBeamEnergy*(1.-xi);
     partP = sqrt(energy*energy-ProtonMassSQ);
     t     = -2.*(ProtonMassSQ - fBeamEnergy*energy+fBeamMomentum*partP*cos(theta));
     pt    = sqrt(pow(partP*tx*urad,2)+pow(partP*ty*urad,2));
     phi   = (Direction>0)?-atan2(ty,-tx):atan2(ty,tx); // defined according to the positive direction
     if (xi<0.||xi>1.||t<0.||t>10.) {
        xi = 0; t=0; partP=0; pt=0; phi=0; theta=0; ImpPar=0;
        return false; // unphysical values 
     }
     stationId = 1; // use 1 for positive arm
     if (Direction<0) {
        theta=TMath::Pi()-theta;
        stationId=2;
     }
     return true;
}
void PPSSim::TrackerReco(int Direction,H_RecRPObject* station,PPSData* arm)
{
//
     double xi,t,partP,pt,phi,theta,ImpPar;
     int stationId;
     if (arm->TrkDet1.NHits==arm->TrkDet2.NHits) {
        for(int i=0;i<arm->TrkDet1.NHits;i++) {
           if (SearchTrack(i,i,Direction,station,arm,stationId,xi,t,partP,pt,phi,theta,ImpPar)) {
              double px = partP*sin(theta)*cos(phi);
              double py = partP*sin(theta)*sin(phi);
              double pz = partP*cos(theta);
              double  e = sqrt(partP*partP+ProtonMassSQ);
              TLorentzVector p(px,py,pz,e);
              if (fCrossAngleCorr) LorentzBoost(p,"MC");
              TLorentzVectorToPPSData(arm,&p);
              arm->stationId.push_back(stationId);
              arm->RatIP.push_back(ImpPar);
           }
           else {
              arm->Fill(); // fill with empty reco parameter
           };
        } 
     }
/*
     for(int i=0;i<arm->TrkDet1.NHits;i++) {
        for(int j=0;j<arm->TrkDet2.NHits;j++) {
           if (i==j) continue;
           if (SearchTrack(i,j,Direction,station,arm,stationId,xi,t,partP,pt,phi,theta,ImpPar)) {
              double thx = atan((arm->TrkDet2.X.at(j)-arm->TrkDet1.X.at(i))*mm_to_cm/(fTrackerLength*m_to_cm))/urad;
              double thy = atan((arm->TrkDet2.Y.at(j)-arm->TrkDet1.Y.at(i))*mm_to_cm/(fTrackerLength*m_to_cm))/urad;
              cout << "Comb. tracks : " << i << " " << j << " " << partP << ImpPar<<" " << thx << " " << thy << endl;
              arm->stationId.push_back(stationId);
              arm->xi.push_back(xi);
              arm->t.push_back(t);
              arm->momentum.push_back(partP);
              arm->pT.push_back(pt);
              arm->phi.push_back(phi);
              arm->theta.push_back(theta);
              arm->RatIP.push_back(ImpPar);
           }
        }
     }
*/
}
void PPSSim::ToFReco()
{
     int nvertex=0;
     for(unsigned int i=0;i<fReco->ArmF.ToF.size();i++) {
        for(unsigned int j=0;j<fReco->ArmB.ToF.size();j++) {
           double ToFtot = fReco->ArmF.ToF.at(i)+fReco->ArmB.ToF.at(j);
           if (fabs(ToFtot-2*fToFPosition/c_light_ns)>3*fTimeSigma) continue;
           fReco->vtxZ.push_back(-c_light_ns*(fReco->ArmF.ToF.at(i)-fReco->ArmB.ToF.at(j))/2.0*m_to_cm);
           fReco->vtxTracks.push_back(pair<unsigned int,unsigned int>(i,j));
           nvertex++;
        }
     }
     fReco->Nvtx=nvertex;
}
void PPSSim::ReconstructArm(H_RecRPObject* pps_station, double x1, double y1, double x2, double y2, double& tx, double& ty, double& eloss)
{
     tx=0.;
     ty=0.;
     eloss=0.;
     if (!pps_station) return;
// Change the orientation and units according to Hector
     x1*=-mm_to_um;
     x2*=-mm_to_um;
     y1*= mm_to_um;
     y2*= mm_to_um;
     pps_station->setPositions(x1,y1,x2,y2);
     double energy = pps_station->getE(AM); // dummy call needed to calculate some Hector internal paramenter
     if (std::isnan(energy)||std::isinf(energy)) return;
     tx =  -pps_station->getTXIP();  // change orientation to CMS
     ty =  pps_station->getTYIP();
     //if (fCrossAngleCorr) tx-=fCrossingAngle; // the - signal is due to the change to CMS system
     eloss = pps_station->getE();
}
void PPSSim::ReconstructArm(int Direction, H_RecRPObject* pps_station, PPSData* arm,int trk_idx ) {
/*
     Reconstruct one Arm giving the kinematics int he LAB ref. frame
*/
     if (!pps_station || !arm) return;

     if (arm->TrkDet1.NHits==0||arm->TrkDet1.NHits<trk_idx||arm->TrkDet2.NHits==0||arm->TrkDet2.NHits<trk_idx) return;
     
     double x1 = -arm->TrkDet1.X.at(trk_idx)*mm_to_um; double x2 = -arm->TrkDet2.X.at(trk_idx)*mm_to_um;
     double y1 =  arm->TrkDet1.Y.at(trk_idx)*mm_to_um; double y2 =  arm->TrkDet2.Y.at(trk_idx)*mm_to_um;
     //double z1 =  arm->TrkDet1.Z.at(0)         ; double z2 =  arm->TrkDet2.Z.at(0);

     pps_station->setPositions(x1,y1,x2,y2);
     double energy = pps_station->getE(AM);
     if (std::isnan(energy)||std::isinf(energy)) {
        arm->Fill();
        return;
     }
     double tx =  -pps_station->getTXIP();
     double ty =  pps_station->getTYIP();
     double x0 =  -pps_station->getX0()*um_to_cm;
     double y0 =  pps_station->getY0()*um_to_cm;

     double theta_reco = sqrt(tx*tx+ty*ty)*urad;
     
     double xi_reco    = pps_station->getE()/fBeamEnergy;
     double e_reco     = fBeamEnergy*(1.-xi_reco);
     double partP      = sqrt(e_reco*e_reco-ProtonMassSQ);
     double t_reco     = -2.*(ProtonMassSQ - fBeamEnergy*e_reco+fBeamMomentum*partP*cos(theta_reco));
     double pt_reco    = sqrt(pow(partP*tx*urad,2)+pow(partP*ty*urad,2));

     if (xi_reco<0||xi_reco>1||t_reco<0) {
        arm->Fill();
        return;
     }

     int station = 0;
     if (Direction>0) station=(fTrackerPosition>=300)? 1:3;
     if (Direction<0) station=(fTrackerPosition>=300)? 2:4;
// If backward, one need to rotate the coord. frame
     if (Direction<0) {theta_reco=TMath::Pi()-theta_reco;};
     double phi_reco   = (Direction>0)?-atan2(ty,-tx):atan2(ty,tx); // defined according to the positive direction

     arm->stationId.push_back(station);
     arm->xi.push_back(xi_reco);
     arm->t.push_back(t_reco);
     arm->momentum.push_back(partP);
     arm->pT.push_back(pt_reco);
     arm->phi.push_back(phi_reco);
     arm->theta.push_back(theta_reco);
     arm->RatIP.push_back(sqrt(x0*x0+y0*y0));
}
void PPSSim::Digitization()
{
//    Fake method to mimic a digitization procedure
//    Just copy the information from the fSim branch and smear the hit according to a given
//    detector resolution;
     TrackerDigi(&(fSim->ArmF),&(fReco->ArmF));
     TrackerDigi(&(fSim->ArmB),&(fReco->ArmB));
     ToFDigi(&(fSim->ArmF),&(fReco->ArmF));
     ToFDigi(&(fSim->ArmB),&(fReco->ArmB));
}
void PPSSim::TrackerDigi(const PPSData* arm_sim,PPSData* arm_reco)
{
     if(!arm_sim||!arm_reco) return;
//
     for(int i=0;i<arm_sim->TrkDet1.NHits;i++){
        if (arm_sim->TrkDet1.HasStopped.at(i)) {arm_reco->TrkDet1.AddHit(0,0,0,0,0,1);continue;}
        double x = arm_sim->TrkDet1.X.at(i)-fDet1XOffsetF*um_to_mm;
        double y = arm_sim->TrkDet1.Y.at(i);
        double z = arm_sim->TrkDet1.Z.at(i);
        HitSmearing(x,y,z);
        if (fFilterHitMap&&(x>fDetectorClosestX||x<fMaxXfromBeam||fabs(y)>fabs(fMaxYfromBeam))) {
           arm_reco->TrkDet1.AddHit(0,0,0,0,0,1);
           continue;
        } 
        arm_reco->TrkDet1.AddHit(x,y,z);
     }
     for(int i=0;i<arm_sim->TrkDet2.NHits;i++){
        if (arm_sim->TrkDet2.HasStopped.at(i)) {arm_reco->TrkDet2.AddHit(0,0,0,0,0,1);continue;}
        double x = arm_sim->TrkDet2.X.at(i)-fDet2XOffsetF*um_to_mm;
        double y = arm_sim->TrkDet2.Y.at(i);
        double z = arm_sim->TrkDet2.Z.at(i);
        HitSmearing(x,y,z);
        if (fFilterHitMap&&(x>fDetectorClosestX||x<fMaxXfromBeam||fabs(y)>fabs(fMaxYfromBeam))) {
           arm_reco->TrkDet2.AddHit(0,0,0,0,0,1);
           continue;
        } 
        arm_reco->TrkDet2.AddHit(x,y,z);
     }
}
void PPSSim::ToFDigi(const PPSData* arm_sim,PPSData* arm_reco)
{
     if(!arm_sim||!arm_reco) return;
//
     for(int i=0;i<arm_sim->ToFDet.NHits;i++){
        if (arm_sim->ToFDet.HasStopped.at(i)) {arm_reco->ToFDet.AddHit(0,0,0,0,0,1); arm_reco->ToF.push_back(0); continue;}
        double x = arm_sim->ToFDet.X.at(i);
        double y = arm_sim->ToFDet.Y.at(i);
        double z = arm_sim->ToFDet.Z.at(i);
        if (fFilterHitMap&&(x>fDetectorClosestX||x<fMaxXfromBeam||fabs(y)>fabs(fMaxYfromBeam))) {
           arm_reco->ToFDet.AddHit(0,0,0,0,0,1); arm_reco->ToF.push_back(0); continue;
        }
        arm_reco->ToFDet.AddHit(x,y,z);
        double t = arm_sim->ToF.at(i);
        if (t>0) ToFSmearing(t);
        arm_reco->ToF.push_back(t);
     }
}
void PPSSim::GenSingleParticle(double& t, double& xi, double& phi)
{
    phi = gRandom3->Uniform(fPhiMin,fPhiMax);
    if (fGenMode=="linear"||fGenMode=="uniform") {
       xi = gRandom3->Uniform(xi_min,xi_max);
       t  = gRandom3->Uniform(t_min,t_max);
    }
    else if (fGenMode=="log"){
       if (t_min==0) t_min = 1e-6; // limit t to 1 MeV
       xi = pow(10,gRandom3->Uniform(log10(xi_min),log10(xi_max)));
       t  = pow(10,gRandom3->Uniform(log10(t_min),log10(t_max)));
    }
    double min_t = Minimum_t(xi);
    if (t<min_t) t = min_t;
}
void PPSSim::GenCentralMass(double& t1, double& t2, double& xi1, double& xi2, double& phi1, double& phi2)
{
   if (fCentralMassErr>0) {
      double m_h = gRandom3->Gaus(fCentralMass,fCentralMassErr);
      while(1) {
         xi1 = gRandom3->Uniform(xi_min,xi_max);
         xi2 = gRandom3->Uniform(xi_min,xi_max);
         double mh_2 = sqrt(xi1*xi2)*2.*fBeamEnergy;
         if ((fabs(m_h-mh_2)<fCentralMassErr) &&
            (isPhysical(xi1)&&isPhysical(xi2))) break;// check validity of kinematic region
       }
    } else {
       while(1) {
          xi1 = gRandom3->Uniform(xi_min,xi_max);
          xi2 = pow(0.5*fCentralMass/fBeamEnergy,2)/xi1;
          if (isPhysical(xi1)&&isPhysical(xi2)) break;
       }
    }
       
    phi1 = gRandom3->Uniform(fPhiMin,fPhiMax);
    phi2 = gRandom3->Uniform(fPhiMin,fPhiMax);
    t1   = gRandom3->Uniform(Minimum_t(xi1),t_max);
    t2   = gRandom3->Uniform(Minimum_t(xi2),t_max);
}
void PPSSim::LorentzBoost(TLorentzVector& p_out, const string& frame)
{
// Use a matrix
     double microrad = 1.e-6;
     //double theta = p_out.Theta(); if (p_out.Pz()<0) theta=TMath::Pi()-theta;
     //double t = -2.*(ProtonMassSQ-fBeamEnergy*p_out.E()+fBeamMomentum*p_out.P()*cos(theta));
     //cout << "before boost P: "; p_out.Print();
     //cout << "before boost t: " << t << endl;
     TMatrixD tmpboost(4,4);
     double alpha_ = 0.;
     double phi_  = fCrossingAngle*microrad;
     if (p_out.Pz()<0) phi_*=-1;
     tmpboost(0,0) = 1./cos(phi_);
     tmpboost(0,1) = - cos(alpha_)*sin(phi_);
     tmpboost(0,2) = - tan(phi_)*sin(phi_);
     tmpboost(0,3) = - sin(alpha_)*sin(phi_);
     tmpboost(1,0) = - cos(alpha_)*tan(phi_);
     tmpboost(1,1) = 1.;
     tmpboost(1,2) = cos(alpha_)*tan(phi_);
     tmpboost(1,3) = 0.;
     tmpboost(2,0) = 0.;
     tmpboost(2,1) = - cos(alpha_)*sin(phi_);
     tmpboost(2,2) = cos(phi_);
     tmpboost(2,3) = - sin(alpha_)*sin(phi_);
     tmpboost(3,0) = - sin(alpha_)*tan(phi_);
     tmpboost(3,1) = 0.;
     tmpboost(3,2) = sin(alpha_)*tan(phi_);
     tmpboost(3,3) = 1.;

     if(frame=="LAB") tmpboost.Invert();

     TMatrixD p4(4,1);
     p4(0,0) = p_out.E();
     p4(1,0) = p_out.Px();
     p4(2,0) = p_out.Py();
     p4(3,0) = p_out.Pz();
     TMatrixD p4lab(4,1);
     p4lab = tmpboost * p4;
     p_out.SetPxPyPzE(p4lab(1,0),p4lab(2,0),p4lab(3,0),p4lab(0,0));
     //theta = p_out.Theta(); if (p_out.Pz()<0) theta=TMath::Pi()-theta;
     //t = -2.*(ProtonMassSQ-fBeamEnergy*p_out.E()+fBeamMomentum*p_out.P()*cos(theta));
     //cout << "after boost P: "; p_out.Print();
     //cout << "after boost t: " << t << endl;
}
void PPSSim::ApplyBeamSmearing(TLorentzVector& p_out)
{
     double microrad = 1.e-6;
     double theta = p_out.Theta(); if (p_out.Pz()<0) theta=TMath::Pi()-theta;
     double dtheta_x = (double)(fSmearAngle)?gRandom3->Gaus(0.,fBeamAngleRMS):0;
     double dtheta_y = (double)(fSmearAngle)?gRandom3->Gaus(0.,fBeamAngleRMS):0;
     double denergy  = (double)(fSmearEnergy)?gRandom3->Gaus(0.,fBeamEnergyRMS):0.;

     double px = p_out.P()*sin(theta+dtheta_x*microrad)*cos(p_out.Phi());
     double py = p_out.P()*sin(theta+dtheta_y*microrad)*sin(p_out.Phi());
     double pz = p_out.P()*(cos(theta)+denergy);

     if (p_out.Pz()<0) pz*=-1;

     double e  = sqrt(px*px+py*py+pz*pz+ProtonMassSQ);
     p_out.SetPxPyPzE(px,py,pz,e);
}
void PPSSim::CrossingAngleCorrection(TLorentzVector& p_out)
{
     double microrad = 1.e-6;
     double theta = p_out.Theta(); if (p_out.Pz()<0) theta=TMath::Pi()-theta;
     //double t = -2.*(ProtonMassSQ-fBeamEnergy*p_out.E()+fBeamMomentum*p_out.P()*cos(theta));
     //cout << "before Totem boost P: "; p_out.Print();
     //cout << "before Totem boost t: " << t << endl;
     double dtheta_x = (double)((fSmearAngle)?gRandom3->Gaus(0.,fBeamAngleRMS):0+
                                (p_out.Pz()>0)?fCrossingAngle:-fCrossingAngle);
     double dtheta_y = (double)(fSmearAngle)?gRandom3->Gaus(0.,fBeamAngleRMS):0;
     double denergy  = (double)(fSmearEnergy)?gRandom3->Gaus(0.,fBeamEnergyRMS):0.;

     double px = p_out.P()*(theta*cos(p_out.Phi())+dtheta_x*microrad);
     double py = p_out.P()*(theta*sin(p_out.Phi())+dtheta_y*microrad);
     double pz = p_out.P()*(cos(theta)+denergy);

     if (p_out.Pz()<0) pz*=-1;

     double e  = sqrt(px*px+py*py+pz*pz+ProtonMassSQ);
     p_out.SetPxPyPzE(px,py,pz,e);
     //theta = p_out.Theta(); if (p_out.Pz()<0) theta=TMath::Pi()-theta;
     //t = -2.*(ProtonMassSQ-fBeamEnergy*(p_out.E())+fBeamMomentum*(p_out.P())*cos(theta));
     //cout << "after Totem boost P: "; p_out.Print();
     //cout << "after Totem boost t: " << t << endl;
//
}
void PPSSim::CrossingAngleCorrection(H_BeamParticle& p_out, const int Direction)
{
// 
// Remember: Hector  used X,Z inverted in ref. to CMS, but pz is always positive
     double partP = sqrt(pow(p_out.getE(),2)-ProtonMassSQ);
     double px = -Direction*partP*p_out.getTX()*urad;
     double py = partP*p_out.getTY()*urad;
     double pz = Direction*partP*cos(sqrt(pow(p_out.getTX(),2)+pow(p_out.getTY(),2))*urad);
     TLorentzVector p(px,py,pz,p_out.getE());
     CrossingAngleCorrection(p);
     p_out.set4Momentum(-Direction*p.Px(),p.Py(),Direction*p.Pz(),p.E());
     return;
}
TLorentzVector PPSSim::shoot(const double& t,const double& xi, const double& phi,const int Direction)
{
    long double energy=fBeamEnergy*(1.-xi);
    long double partP = sqrt((long double)(energy*energy-ProtonMassSQ));
    long double theta = acos((-t/2. - ProtonMassSQ + fBeamEnergy*energy)/(fBeamMomentum*partP)); // this angle is the scattering one
    long double px = partP*sin(theta)*cos((long double)phi)*Direction;
    long double py = partP*sin(theta)*sin((long double)phi);
    long double pz = partP*cos(theta)*Direction;
/*
    long double thx = atan(px/pz*Direction);
    long double thy = atan(py/pz*Direction);
    if (fCrossAngleCorr) {
       thx  += CRANG*urad;
       theta = sqrt(pow(thx,2)+pow(thy,2));
       pz    = partP*cos(theta)*Direction;
       px    = fabs(pz)*tan(thx);
       double phi2  = atan2(py,px);
    }

    partP = sqrt(px*px+py*py+pz*pz); // just for cross check
*/
    return TLorentzVector((double)px,(double)py,(double)pz,(double)energy);
}
void PPSSim::Propagate(H_BeamParticle* pbeam,int Direction) {
     PPSData* arm = NULL;
     H_BeamLine* beamline = NULL;
     double startZ = -pbeam->getS(); // in the CMS ref. frame, in meters
     double tcl4pos = 0;
     double tcl5pos = 0;

     if (Direction>0) {arm = &(fSim->ArmF);beamline=beamlineF;tcl4pos=fTCL4Position2;tcl5pos=fTCL5Position2;}
     if (Direction<0) {arm = &(fSim->ArmB);beamline=beamlineB;tcl4pos=fTCL4Position1;tcl5pos=fTCL5Position1;}
     
     for(unsigned int i=0;i<CheckPoints.size();i++) {
        double i_chkpoint = CheckPoints.at(i);
        pbeam->propagate(i_chkpoint);
        if (pbeam->stopped(beamline) && pbeam->getStoppingElement()->getS()<i_chkpoint) break;
        arm->ChkPoint.AddHit(-pbeam->getX()*um_to_mm,pbeam->getY()*um_to_mm,i_chkpoint);
     }
// Propagate until TCL4 and 5
     if (tcl4pos>0) {
        double beampos = (Direction<0)?fBeam1PosAtTCL4.first:fBeam2PosAtTCL4.first;
        double beamrms = (Direction<0)?fBeam1RMSAtTCL4.first:fBeam2RMSAtTCL4.first;
        pbeam->propagate(tcl4pos);
        double xpos = -pbeam->getX()*um_to_mm;
        arm->XatTCL4.push_back(fabs(xpos-beampos)/beamrms);
     }
     if (tcl5pos>0) {
        double beampos = (Direction<0)?fBeam1PosAtTCL5.first:fBeam2PosAtTCL5.first;
        double beamrms = (Direction<0)?fBeam1RMSAtTCL5.first:fBeam2RMSAtTCL5.first;
        pbeam->propagate(tcl5pos);double xpos=-pbeam->getX()*um_to_mm;
        arm->XatTCL5.push_back(fabs(xpos-beampos)/beamrms);
     }
//
     pbeam->propagate(fTrackerPosition);

     int stopped = (pbeam->stopped(beamline) && pbeam->getStoppingElement()->getS()<fTrackerPosition)?1:0;
     
// uses mm for X,Y and m for Z in the PPS station
     double x1 = -pbeam->getX()*um_to_mm;
     double y1 = pbeam->getY()*um_to_mm;
     double z1 = fTrackerPosition;
//
     arm->TrkDet1.AddHit(x1,y1,z1,-pbeam->getTX(),pbeam->getTY(),stopped);

     pbeam->propagate(fTrackerPosition+fTrackerLength);

     stopped=(pbeam->stopped(beamline) && pbeam->getStoppingElement()->getS()<fTrackerPosition+fTrackerLength)?1:0;
     double x2 = -pbeam->getX()*um_to_mm;
     double y2 = pbeam->getY()*um_to_mm;
     double z2 = fTrackerPosition+fTrackerLength; // in m
     if (Direction>0) arm->stationId.push_back((fTrackerPosition>=300)? 1:3);
     if (Direction<0) arm->stationId.push_back((fTrackerPosition>=300)? 2:4);

     arm->TrkDet2.AddHit(x2,y2,z2,-pbeam->getTX(),pbeam->getTY(),stopped);
     //return false;

// Propagate until Time detector
     pbeam->propagate(fToFPosition);
     double xt = -pbeam->getX()*um_to_mm;
     double yt = pbeam->getY()*um_to_mm;
     double zt = fToFPosition; // in m
     stopped=(pbeam->stopped(beamline) && pbeam->getStoppingElement()->getS()<fToFPosition)?1:0;
//
     arm->ToFDet.AddHit(xt,yt,zt,-pbeam->getTX(),pbeam->getTY(),stopped);
     if (!stopped) arm->ToF.push_back((fToFPosition-Direction*startZ)/c_light_ns);
     else arm->ToF.push_back(0.);
}
void PPSSim::LoadParameters()
{
     ifstream in("PPSSim.cfg");
     if (!in.is_open()) {
        std::cout << "Warning: Could not open configuration file. Using default values:" << std::endl;
        return;
     }
     std::string line;
     while(1) {
        std::getline(in,line);
        if (in.eof()) break;
        std::string keyword = line.substr(0,line.find(" "));
        if (keyword[0] == '#') {continue;}
        else if (keyword=="TrackerPosition")fTrackerPosition= atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="TrackerLength")  fTrackerLength  = atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="ToFPosition")    fToFPosition    = atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="BeamLineLength") fBeamLineLength = atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="SmearVertex")    fSmearVertex    = true;
        else if (keyword=="VtxMeanX")       fVtxMeanX       = atof(line.substr(strlen(keyword.c_str())).c_str()); // in cm
        else if (keyword=="VtxMeanY")       fVtxMeanY       = atof(line.substr(strlen(keyword.c_str())).c_str()); // in cm
        else if (keyword=="VtxMeanZ")       fVtxMeanZ       = atof(line.substr(strlen(keyword.c_str())).c_str()); // in cm
        else if (keyword=="VtxSigmaX")      fVtxSigmaX      = atof(line.substr(strlen(keyword.c_str())).c_str()); // in cm
        else if (keyword=="VtxSigmaY")      fVtxSigmaY      = atof(line.substr(strlen(keyword.c_str())).c_str()); // in cm
        else if (keyword=="VtxSigmaZ")      fVtxSigmaZ      = atof(line.substr(strlen(keyword.c_str())).c_str()); // in cm
        else if (keyword=="SmearHit")       fSmearHit       = true;
        else if (keyword=="HitSigmaX")      fHitSigmaX      = atof(line.substr(strlen(keyword.c_str())).c_str()); // in mm
        else if (keyword=="HitSigmaY")      fHitSigmaY      = atof(line.substr(strlen(keyword.c_str())).c_str()); // in mm
        else if (keyword=="HitSigmaZ")      fHitSigmaZ      = atof(line.substr(strlen(keyword.c_str())).c_str()); // in mm
        else if (keyword=="TimeSigma")      fTimeSigma      = atof(line.substr(strlen(keyword.c_str())).c_str()); // in ns
        else if (keyword=="SimBeam")        fSimBeam        = true;
        else if (keyword=="PhiMin")         fPhiMin         = atof(line.substr(strlen(keyword.c_str())).c_str()); // >= -Pi
        else if (keyword=="PhiMax")         fPhiMax         = atof(line.substr(strlen(keyword.c_str())).c_str()); // <=  Pi
        else if (keyword=="EtaMin")         fEtaMin         = atof(line.substr(strlen(keyword.c_str())).c_str()); // eta cut (min. eta to be propagated)
        else if (keyword=="MomentumMin")    fMomentumMin    = atof(line.substr(strlen(keyword.c_str())).c_str()); // mom. cut (min. mom. to be propagated)
        else if (keyword=="CentralMass")    fCentralMass    = atof(line.substr(strlen(keyword.c_str())).c_str()); // in GeV/c^2
        else if (keyword=="CentralMassErr") fCentralMassErr = atof(line.substr(strlen(keyword.c_str())).c_str()); // in GeV/c^2
        else if (keyword=="CrossAngleCorr") fCrossAngleCorr = true;
        else if (keyword=="KickersOFF")     fKickersOFF = true;
        else if (keyword=="TrackImpactParameterCut") fTrackImpactParameterCut = atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="MinThetaXatDet1") fMinThetaXatDet1 = atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="MaxThetaXatDet1") fMaxThetaXatDet1 = atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="MinThetaYatDet1") fMinThetaYatDet1 = atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="MaxThetaYatDet1") fMaxThetaYatDet1 = atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="MaxXfromBeam")    fMaxXfromBeam    = atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="MaxYfromBeam")    fMaxYfromBeam    = atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="DetectorClosestX")fDetectorClosestX= atof(line.substr(strlen(keyword.c_str())).c_str());
        else if (keyword=="No-FilterHitMap") fFilterHitMap = false;
        else std::cout << "Unknown parameter: " << keyword << ". Ignoring it." << std::endl;
     }
}
void PPSSim::SmearVertexPosition(double& vtxX,double& vtxY, double& vtxZ)
{
     vtxX = fVtxMeanX;
     vtxY = fVtxMeanY;
     vtxZ = fVtxMeanZ;
     if (fSmearVertex) {
         vtxX=gRandom3->Gaus(fVtxMeanX,fVtxSigmaX); // in cm
         vtxY=gRandom3->Gaus(fVtxMeanY,fVtxSigmaY); // in cm
         vtxZ=gRandom3->Gaus(fVtxMeanZ,fVtxSigmaZ); // in cm
     }
}
void PPSSim::HitSmearing(double& x, double& y, double& z)
{
//
// X,Y in PPS is in mm, Z in m, but the hit resolution is given in mm. Then, to avoid smearing
// into a too narow distribution, converts to mm and then, converts back the z coordinats to m
//
     if (fSmearHit) {
        x = gRandom3->Gaus(x,fHitSigmaX);
        y = gRandom3->Gaus(y,fHitSigmaY);
        z = gRandom3->Gaus(z*m_to_mm,fHitSigmaZ)*mm_to_m;
     }
     return;
}
double PPSSim::Minimum_t(const double& xi)
{
     double partE = fBeamEnergy*(1.- xi);
     double partP = sqrt(partE*partE-ProtonMassSQ);
     return -2.*(fBeamMomentum*partP-fBeamEnergy*partE+ProtonMassSQ);
}

void PPSSim::PrintParameters()
{
     std::cout << "Running with:\n"
               << "TrackerPosition    = " <<  fTrackerPosition << "\n"
               << "TrackerLength      = " <<  fTrackerLength << "\n"
               << "TrackerPosition    = " <<  fTrackerPosition << "\n"
               << "TrackerLength      = " <<  fTrackerLength << "\n"
               << "ToFPosition        = " <<  fToFPosition << "\n"
               << "BeamLineLength     = " <<  fBeamLineLength << "\n"
               << "SmearVertex        = " <<  fSmearVertex << "\n"
               << "VtxMeanX           = " <<  fVtxMeanX << "\n"
               << "VtxMeanY           = " <<  fVtxMeanY << "\n"
               << "VtxMeanZ           = " <<  fVtxMeanZ << "\n"
               << "VtxSigmaX          = " <<  fVtxSigmaX << "\n"
               << "VtxSigmaY          = " <<  fVtxSigmaY << "\n"
               << "VtxSigmaZ          = " <<  fVtxSigmaZ << "\n"
               << "VtxMeanZ           = " <<  fVtxMeanZ << "\n"
               << "VtxSigmaX          = " <<  fVtxSigmaX << "\n"
               << "VtxSigmaY          = " <<  fVtxSigmaY << "\n"
               << "VtxSigmaZ          = " <<  fVtxSigmaZ << "\n"
               << "SmearHit           = " <<  fSmearHit << "\n"
               << "HitSigmaX          = " <<  fHitSigmaX << "\n"
               << "HitSigmaY          = " <<  fHitSigmaY << "\n"
               << "HitSigmaZ          = " <<  fHitSigmaZ << "\n"
               << "TimeSigma          = " <<  fTimeSigma << "\n"
               << "SimBeam            = " <<  fSimBeam   << "\n"
               << "PhiMin             = " <<  fPhiMin    << "\n"
               << "PhiMax             = " <<  fPhiMax    << "\n"
               << "EtaMin             = " <<  fEtaMin    << "\n"
               << "MomentumMin        = " <<  fMomentumMin << "\n"
               << "CrossAngleCorr     = " <<  fCrossAngleCorr << "\n"
               << "KickersOFF         = " <<  fKickersOFF << "\n"
               << "Central Mass       = " <<  fCentralMass << " +- " << fCentralMassErr << "\n"
               << "TrackImpactParameterCut = " << fTrackImpactParameterCut << "\n"
               << "MinThetaXatDet1    = " <<fMinThetaXatDet1 << "\n"
               << "MaxThetaXatDet1    = " <<fMaxThetaXatDet1 << "\n"
               << "MinThetaYatDet1    = " <<fMinThetaYatDet1 << "\n"
               << "MaxThetaYatDet1    = " <<fMaxThetaYatDet1 << "\n"
               << std::endl;
}

TH2F* PPSSim::GenBeamProfile(const double& z)
{
    float beamp_w = 20.0;//beam pipe width
    int  direction=int(z/fabs(z));
    int   nbins = 500;
    TH2F* beamprofile = (TH2F*)gDirectory->FindObject("BeamProfile");
    if (beamprofile) delete beamprofile;
    beamprofile = new TH2F("BeamProfile",Form("Beam Profile at z=%3.2f; X (mm); Y (mm)",z),nbins,-beamp_w,beamp_w,nbins,-beamp_w,beamp_w);
    for(int n=0;n<100000;n++) {
       H_BeamParticle p1; // beam particle generated in the ref. frame of Hector/LHC
       p1.smearPos();
       if (fCrossAngleCorr) CrossingAngleCorrection(p1,direction); // apply the crossing angle correction (boost)
       else { p1.smearAng();p1.smearE(); }   // if no correnction for crossing angle, apply just the smearing
//
// set the vertex, given in the CMS ref. frame (x-> -x; z-> -z)
       p1.setPosition(
                      //p1.getX()-fVtxMeanX*cm_to_um,p1.getY()+fVtxMeanY*cm_to_um,
                      p1.getX(),p1.getY(),
                      p1.getTX(),p1.getTY(),
                      //p1.getTX()-direction*fCrossingAngle,p1.getTY(),
                      p1.getS());
//
       if (z<0) p1.computePath(beamlineB);
       else     p1.computePath(beamlineF);
       p1.propagate(fabs(z));
       beamprofile->Fill(-p1.getX()*um_to_mm,p1.getY()*um_to_mm);
    }
    return beamprofile;
}
