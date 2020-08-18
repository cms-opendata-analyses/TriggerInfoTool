// -*- C++ -*-
//
// Package:    TriggerSimplePrescalesAnalyzer
// Class:      TriggerSimplePrescalesAnalyzer
// 
/**\class TriggerSimplePrescalesAnalyzer TriggerSimplePrescalesAnalyzer.cc TriggerInfo/TriggerSimplePrescalesAnalyzer/src/TriggerSimplePrescalesAnalyzer.cc
 Description: [one line class summary]
 Implementation:
     [Notes on implementation]
*/
//
// Originally put together by Edgar Carrera from official CMSSW software
//         Created:  Mon Jul  3 15:59:18 CEST 2017
// $Id$
//
//


// system include files
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <boost/foreach.hpp>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "FWCore/Utilities/interface/RegexMatch.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Provenance/interface/ParameterSetID.h"
#include <cassert>


//
// class declaration
//

class TriggerSimplePrescalesAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TriggerSimplePrescalesAnalyzer(const edm::ParameterSet&);
      ~TriggerSimplePrescalesAnalyzer();

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void analyzeSimplePrescales(const edm::Event&, const edm::EventSetup&, const std::string& triggerName);
      virtual void initPattern(const edm::TriggerResults & result,
                        const edm::EventSetup& iSetup,
                        const edm::TriggerNames & triggerNames);
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
      std::string   processName_;
      edm::InputTag triggerResultsTag_;
      edm::InputTag triggerEventTag_;
    /// HLT trigger names
    edm::ParameterSetID triggerNamesID_;

      // additional class data memebers
      // these are actually the containers where we will store
      // the trigger information
      edm::Handle<edm::TriggerResults>   triggerResultsHandle_;
      edm::Handle<trigger::TriggerEvent> triggerEventHandle_;
      HLTConfigProvider hltConfig_;

      //inspired by https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_X/HLTrigger/HLTfilters/interface/HLTHighLevel.h
      // input patterns that will be expanded into trigger names
      std::vector<std::string>  HLTPatterns_;

    /// list of required HLT triggers by HLT name
      std::vector<std::string>  HLTPathsByName_;

    

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
// Notice that here, using the parameter set tool, 
// you need to point the code to
// the right branch (in the EDM root files) where the trigger information
// is stored.  

// Also, at configuration time
// you will need to point to the appropiate triggers
// you want to look at. Alternatively (is also shown below), you
// could select the trigger names dynamically; for example getting them
// from the HLTConfigProvider.

// To start out, you need to define a processName, which is the name of
// the CMSSW computing process that originally wrote the products in the root
// file. Originally, this is always "HLT", by default.  
// In triggerName, you can
// use wildcards, which will be described later.
// As for the InputTags, these shall match the name of the ROOT branches
// where the information is stored.  This was essentially fixed and will
// most likely be the same always. 

//To make the wildcards working we get inspired by
//https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_X/HLTrigger/HLTfilters/src/HLTHighLevel.cc

//This should match your configuration python file
TriggerSimplePrescalesAnalyzer::TriggerSimplePrescalesAnalyzer(const edm::ParameterSet& ps):
processName_(ps.getParameter<std::string>("processName")),
triggerResultsTag_(ps.getParameter<edm::InputTag>("triggerResults")),
triggerEventTag_(ps.getParameter<edm::InputTag>("triggerEvent")),
triggerNamesID_(),
HLTPatterns_(ps.getParameter<std::vector<std::string> >("triggerPatterns")),
HLTPathsByName_()
{
   //now do what ever initialization is needed
  using namespace std;
  using namespace edm;
  
}


TriggerSimplePrescalesAnalyzer::~TriggerSimplePrescalesAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//




// ------------ method called when starting to processes a run  ------------
void TriggerSimplePrescalesAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
//--------------------------------------------------------------------------
{
    using namespace std;
    using namespace edm;

    bool changed(true);
    hltConfig_.init(iRun,iSetup,processName_,changed);
    
    if (changed)
    {
        cout<<"HLTConfig has changed . . . "<<endl;
        
    }
    
      
        

}//------------------- beginRun()





// ------------ method called for each event  ------------------------------
// As with any EDAnalyzer, this method is the heart of the analysis
void TriggerSimplePrescalesAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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
 
   iEvent.getByLabel(triggerResultsTag_,triggerResultsHandle_);
   if (!triggerResultsHandle_.isValid()) {
     cout << "Error in getting TriggerResults product from Event!" << endl;
     return;
   }
   iEvent.getByLabel(triggerEventTag_,triggerEventHandle_);
   if (!triggerEventHandle_.isValid()) {
     cout << "Error in getting TriggerEvent product from Event!" << endl;
     return;
   }
   // sanity check
   assert(triggerResultsHandle_->size()==hltConfig_.size());

   //Inspired in https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_X/HLTrigger/HLTfilters/src/HLTHighLevel.cc
   // init the TriggerNames with the TriggerResults
  const edm::TriggerNames & triggerNames = iEvent.triggerNames(*triggerResultsHandle_);
  bool config_changed = false;
  if (triggerNamesID_ != triggerNames.parameterSetID()) {
    triggerNamesID_ = triggerNames.parameterSetID();
    config_changed = true;
  }
  // (re)run the initialization of the container with the trigger patterns 
  // - this is the first event 
  // - or the HLT table has changed 
  if (config_changed) {
      initPattern(*triggerResultsHandle_, iSetup, triggerNames);  
  }
  

   //Loop over all triggers in the pattern
  unsigned int n = HLTPathsByName_.size();
   for (unsigned int i=0; i!=n; ++i) {
       analyzeSimplePrescales(iEvent,iSetup,HLTPathsByName_[i]);
   }






  return;

}//---------------------------analyze()




//---------------------------Actual trigger analysis-------------
void TriggerSimplePrescalesAnalyzer::analyzeSimplePrescales(const edm::Event& iEvent, const edm::EventSetup& iSetup, const std::string& triggerName) 
//-----------------------------------------------------------------
{

  using namespace std;
  using namespace edm;
  using namespace reco;
  using namespace trigger;

  cout<<"Currently analyzing trigger "<<triggerName<<endl;

  //Check the current configuration to see how many total triggers there are
  const unsigned int n(hltConfig_.size());
  //Get the trigger index for the current trigger
  const unsigned int triggerIndex(hltConfig_.triggerIndex(triggerName));
  //check that the trigger in the event and in the configuration agree
  assert(triggerIndex==iEvent.triggerNames(*triggerResultsHandle_).triggerIndex(triggerName));
  // abort on invalid trigger name
  if (triggerIndex>=n) {
    cout << "Trigger path"<< triggerName << " not found!" << endl;
    return;
  }

  
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // EXAMPLE: L1 and HLT prescale values via (L1) EventSetup
  // Current (default) prescale set index - to be taken from L1GtUtil via Event.
  // Try to get the L1 and HLT prescale values that were actually used 
  // for this event.
  // This example needs the conditions stored in the Global Tag,
  // which is some sort of snapshot of the by-then current detector
  // conditions.  They need to be extracted from the database
  // and for that the "Frontier Conditions" lines need to
  // be added in the python configuration file along with the 
  // name for the global tag.
  // This can make the job very slow at the very begining....
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  const std::pair<int,int> prescales(hltConfig_.prescaleValues(iEvent,iSetup,triggerName));
  cout << "analyzeSimplePrescales: path "
      << triggerName << " [" << triggerIndex << "] "
      << "prescales L1T,HLT: " << prescales.first << "," << prescales.second
      << endl;
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // EXAMPLE: Find out if the trigger was active, accepted, or in error.
  // We could also find out whether the trigger was active (wasrun), 
  // if it accepted the event (accept) or if it gave an error (error).
  // Results from TriggerResults product
  //Uncomment the lines below
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  cout << " Trigger path status:"
       << " WasRun=" << triggerResultsHandle_->wasrun(triggerIndex)
       << " Accept=" << triggerResultsHandle_->accept(triggerIndex)
       << " Error =" << triggerResultsHandle_->error(triggerIndex)
       << endl;
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  return;
}//--------------------------analyzeSimplePrescales()

// ------------ ------------
void TriggerSimplePrescalesAnalyzer::initPattern(const edm::TriggerResults & result,
                        const edm::EventSetup& iSetup,
                        const edm::TriggerNames & triggerNames)
//--------------------------------------------------------------------------
{
    unsigned int n;
    
    // clean up old data
    HLTPathsByName_.clear();

    if (HLTPatterns_.empty()) {
        // for empty input vector, default to all HLT trigger paths
        n = result.size();
        HLTPathsByName_.resize(n);
        for (unsigned int i = 0; i < n; ++i) {
            HLTPathsByName_[i] = triggerNames.triggerName(i);
        }
    } else {
        // otherwise, expand wildcards in trigger names...
        BOOST_FOREACH(const std::string & pattern, HLTPatterns_) {
            if (edm::is_glob(pattern)) {
                // found a glob pattern, expand it
                std::vector< std::vector<std::string>::const_iterator > matches = edm::regexMatch(triggerNames.triggerNames(), pattern);
                if (matches.empty()) {
                    // pattern does not match any trigger paths
                    std::cout<<"No patterns found.  Please check quality file... "<<std::endl;
                    exit(0);
                } else {
                    // store the matching patterns
                    BOOST_FOREACH(std::vector<std::string>::const_iterator match, matches)
                        HLTPathsByName_.push_back(*match);
                }
            } else {
                // found a trigger name, just copy it
                HLTPathsByName_.push_back(pattern);
            }
        }
        
    }
}



// ------------ method called once each job just before starting event loop  ------------
void 
TriggerSimplePrescalesAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TriggerSimplePrescalesAnalyzer::endJob() 
{
}


// ------------ method called when ending the processing of a run  ------------
void TriggerSimplePrescalesAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}


// ------------ method called when starting to processes a luminosity block  ------------
void 
TriggerSimplePrescalesAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
TriggerSimplePrescalesAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TriggerSimplePrescalesAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerSimplePrescalesAnalyzer);
