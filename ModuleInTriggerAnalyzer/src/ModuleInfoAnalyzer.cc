// -*- C++ -*-
//
// Package:    ModuleInfoAnalyzer
// Class:      ModuleInfoAnalyzer
// 
/**\class ModuleInfoAnalyzer ModuleInfoAnalyzer.cc 
 Description: [one line class summary]
 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Edgar Carrera
//         Created:  Mon Jul  3 15:59:18 CEST 2017
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//Following the HLTEventAnalyzerAOD.h, 
//include the following headers:
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

//Also include headers from  HLTEventAnalyzerAOD.cc
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include <cassert>


//
// class declaration
//

class ModuleInfoAnalyzer : public edm::EDAnalyzer {
   public:
      explicit ModuleInfoAnalyzer(const edm::ParameterSet&);
      ~ModuleInfoAnalyzer();

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void analyzeTrigger(const edm::Event&, const edm::EventSetup&, const std::string& triggerName);
      //the follwing are not being used here
      virtual void beginJob() ;
      virtual void endJob() ;
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
     
      // from HLTEventAnalyzerAOD.h
      /// module config parameters
      std::string   triggerName_;

      // additional class data memebers
      // these are actually the containers where we will store
      // the trigger information
      edm::Handle<edm::TriggerResults>   triggerResultsHandle_;
      edm::Handle<trigger::TriggerEvent> triggerEventHandle_;
      HLTConfigProvider hltConfig_;

      // ----------member data ---------------------------
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

//This should match your configuration python file
ModuleInfoAnalyzer::ModuleInfoAnalyzer(const edm::ParameterSet& ps):
triggerName_(ps.getParameter<std::string>("triggerName"))
{
   //now do what ever initialization is needed

}


ModuleInfoAnalyzer::~ModuleInfoAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called when starting to processes a run  ------------
void ModuleInfoAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
//--------------------------------------------------------------------------
{
  using namespace std;
  using namespace edm;

	bool changed(true);
	hltConfig_.init(iRun,iSetup,"HLT",changed);
}//------------------- beginRun()


// ------------ method called for each event  ------------------------------
// As with any EDAnalyzer, this method is the heart of the analysis
void ModuleInfoAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
//--------------------------------------------------------------------------
{
   using namespace edm;
   using namespace std;

   // Get event products: 
   // In the following, the code is trying to access the information 
   // from the ROOT files and point the containers (that we created), 
   // namely triggerResultsHandle_ and triggerEVentHandle_, 
   // to the correct "address", given at configuration time 
   // and assigned to triggerResultsTag_ and triggerEventTag_
 
   // After that, a simple sanity check is done.
   InputTag triggerEventTag_("hltTriggerSummaryAOD","","HLT");
   InputTag triggerResultsTag_("TriggerResults","","HLT");
   iEvent.getByLabel(triggerResultsTag_,triggerResultsHandle_);
   iEvent.getByLabel(triggerEventTag_,triggerEventHandle_);
   
   if (triggerName_=="@") {
     const unsigned int n(hltConfig_.size());
     for (unsigned int i=0; i!=n; ++i) {
       analyzeTrigger(iEvent,iSetup,hltConfig_.triggerName(i));
     }
   } else {
     analyzeTrigger(iEvent,iSetup,triggerName_);
   }
   
  return;

}//---------------------------analyze()


//---------------------------Actual trigger analysis-------------
void ModuleInfoAnalyzer::analyzeTrigger(const edm::Event& iEvent, const edm::EventSetup& iSetup, const std::string& triggerName) 
//-----------------------------------------------------------------
{

  using namespace std;
  using namespace edm;
  using namespace reco;
  using namespace trigger;

  cout<<"Currently analyzing trigger "<<triggerName<<endl;

  //Get the trigger index for the current trigger
  const unsigned int triggerIndex(hltConfig_.triggerIndex(triggerName));
 

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // EXAMPLE: Which module was last run within the trigger path
  // Additionally, one can find which module was last run within the trigger,
  // i.e., which module was responsible for stopping the trigger path.
  // One could find the list of modules in a given trigger from the
  // HLTConfigProvider as explained somewhere else in this code.
  // Get index (slot position) of module giving the decision of the path
  // as described in "DataFormats/Common/interface/HLTGlobalStatus.h"
  //Uncomment the lines below
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  const unsigned int m(hltConfig_.size(triggerIndex));
  const vector<string>& moduleLabels(hltConfig_.moduleLabels(triggerIndex));
  const unsigned int moduleIndex(triggerResultsHandle_->index(triggerIndex));
  cout << " Last active module - label/type: "
       << moduleLabels[moduleIndex] << "/" << hltConfig_.moduleType(moduleLabels[moduleIndex])
       << " [" << moduleIndex << " out of 0-" << (m-1) << " on this path]"
       << endl;
  assert (moduleIndex<m);
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  return;
}//--------------------------analyzeTrigger()



// ------------ method called once each job just before starting event loop  ------------
void 
ModuleInfoAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ModuleInfoAnalyzer::endJob() 
{
}


// ------------ method called when ending the processing of a run  ------------
void ModuleInfoAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}


// ------------ method called when starting to processes a luminosity block  ------------
void 
ModuleInfoAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
ModuleInfoAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ModuleInfoAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ModuleInfoAnalyzer);
