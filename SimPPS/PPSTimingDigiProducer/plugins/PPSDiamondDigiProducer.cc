// -*- C++ -*-
//
// Package:    SimPPS/PPSTimingDigiProducer
// Class:      PPSDiamondDigiProducer
//
/**\class PPSDiamondDigiProducer PPSDiamondDigiProducer.cc SimPPS/PPSTimingDigiProducer/plugins/PPSDiamondDigiProducer.cc

 Description: A producer for simulated PPS diamond DIGIs

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme
//         Created:  Wed, 23 Nov 2022 15:21:39 GMT
//
//

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDigi/interface/CTPPSDiamondDigi.h"

class PPSDiamondDigiProducer : public edm::stream::EDProducer<> {
public:
  explicit PPSDiamondDigiProducer(const edm::ParameterSet&);

  static void fillDescriptions(edm::ConfigurationDescriptions&);

private:
  void beginStream(edm::StreamID) override;
  void produce(edm::Event&, const edm::EventSetup&) override;
  void endStream() override;

  edm::EDGetTokenT<CrossingFrame<PSimHit> > crossingFrameToken_;
  edm::EDPutTokenT<edm::DetSetVector<CTPPSDiamondDigi> > diamondDigiToken_;
};

PPSDiamondDigiProducer::PPSDiamondDigiProducer(const edm::ParameterSet& iConfig)
    : crossingFrameToken_(consumes<CrossingFrame<PSimHit> >(iConfig.getParameter<edm::InputTag>("inputs"))),
      diamondDigiToken_(produces<edm::DetSetVector<CTPPSDiamondDigi> >()) {}

void PPSDiamondDigiProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::DetSetVector<CTPPSDiamondDigi> digis;
  iEvent.emplace(diamondDigiToken_, std::move(digis));
}

void PPSDiamondDigiProducer::beginStream(edm::StreamID) {
  // please remove this method if not needed
}

void PPSDiamondDigiProducer::endStream() {
  // please remove this method if not needed
}

void PPSDiamondDigiProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("inputs", edm::InputTag("mix", "g4SimHitsCTPPSTimingHits"));
  descriptions.add("PPSDiamondDigitizer", desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PPSDiamondDigiProducer);
