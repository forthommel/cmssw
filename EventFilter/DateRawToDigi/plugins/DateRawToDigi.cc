// -*- C++ -*-
//
// Package:    EventFilter/DateRawToDigi
// Class:      DateRawToDigi
//
/**\class DateRawToDigi DateRawToDigi.cc EventFilter/DateRawToDigi/plugins/DateRawToDigi.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Laurent Forthomme
//         Created:  Thu, 22 Nov 2018 12:52:00 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

class DateRawToDigi : public edm::stream::EDProducer<>
{
  public:
    explicit DateRawToDigi( const edm::ParameterSet& );
    ~DateRawToDigi() = default;

    static void fillDescriptions( edm::ConfigurationDescriptions& descriptions );

  private:
    virtual void beginStream( edm::StreamID ) override {}
    virtual void produce( edm::Event&, const edm::EventSetup& ) override;
    virtual void endStream() override {}

    //virtual void beginRun( const edm::Run&, const edm::EventSetup& ) override {}
    //virtual void endRun( const edm::Run&, const edm::EventSetup& ) override {}
    //virtual void beginLuminosityBlock( const edm::LuminosityBlock&, edm::EventSetup const& ) override {}
    //virtual void endLuminosityBlock( const edm::LuminosityBlock&, edm::EventSetup const& ) override {}
};

DateRawToDigi::DateRawToDigi( const edm::ParameterSet& iConfig )
{
   //register your products
/* Examples
   produces<ExampleData2>();

   //if do put with a label
   produces<ExampleData2>("label");

   //if you want to put into the Run
   produces<ExampleData2,InRun>();
*/
   //now do what ever other initialization is needed

}

void
DateRawToDigi::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which
   // is put into the Event
   iEvent.put(std::make_unique<ExampleData2>(*pIn));
*/

/* this is an EventSetup example
   //Read SetupData from the SetupRecord in the EventSetup
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
*/

}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DateRawToDigi::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE( DateRawToDigi );

