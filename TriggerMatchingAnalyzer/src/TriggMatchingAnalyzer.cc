// -*- C++ -*-
//
// Package:    TriggMatchingAnalyzer
// Class:      TriggMatchingAnalyzer
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


//classes to extract trigger information
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
//#include "DataFormats/Common/interface/TriggerResults.h"
//#include "FWCore/Common/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"


#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
//#include "DataFormats/Candidate/interface/Particle.h"

//
// class declaration
//

class TriggMatchingAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TriggMatchingAnalyzer(const edm::ParameterSet&);
      ~TriggMatchingAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);

      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
//declare a function to do the trigger analysis
      void analyzeMatchingObject(const edm::Event& iEvent, const edm::Handle<trigger::TriggerEvent> &trigEvent, const edm::InputTag &trigEventTag_,const edm::Handle<reco::TrackCollection> &tracks);
      
      double dR(const trigger::TriggerObject& ,  const reco::Track &  );


//declare de filter (module) of the trigger              
      std::string   filterName_;
      edm::InputTag trackInput;

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

TriggMatchingAnalyzer::TriggMatchingAnalyzer(const edm::ParameterSet& iConfig)

{
	//now do what ever initialization is needed
	//we take the filtername of interest and the reconstructed track collection
	//from the configuration
	filterName_ = iConfig.getParameter<std::string>("filterName");
    	trackInput = iConfig.getParameter<edm::InputTag>("TrackCollection");
}


TriggMatchingAnalyzer::~TriggMatchingAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void 
TriggMatchingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
   	using namespace std;
    
	//containers for the trigger stuff
	InputTag trigEventTag("hltTriggerSummaryAOD","","HLT"); 
	Handle<trigger::TriggerEvent> mytrigEvent; 
	iEvent.getByLabel(trigEventTag,mytrigEvent);
	//container for the tracks
	Handle<reco::TrackCollection> mytracks;
	iEvent.getByLabel(trackInput,mytracks);
	//launch the matching code
	analyzeMatchingObject(iEvent,mytrigEvent,trigEventTag,mytracks);
   
return;
}


void 
TriggMatchingAnalyzer::analyzeMatchingObject(const edm::Event& iEvent, const edm::Handle<trigger::TriggerEvent> &trigEvent, const edm::InputTag &trigEventTag_,const edm::Handle<reco::TrackCollection> &tracks)
{
	using namespace std;		
	//This is inspired on https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHLTAnalysis
	trigger::size_type filterIndex = trigEvent->filterIndex(edm::InputTag(filterName_,"",trigEventTag_.process())); 
      	int match=0;  
      	if(filterIndex<trigEvent->sizeFilters()){
        	const trigger::Keys& trigKeys = trigEvent->filterKeys(filterIndex); 
        	const trigger::TriggerObjectCollection & trigObjColl(trigEvent->getObjects());
        	//using reco::TrackCollection;
		if(tracks.isValid()){
			for(size_t i = 0; i < tracks->size(); ++ i){
				const reco::Track & p = (*tracks)[i];
     				// Trigger object loop starts
				for(trigger::Keys::const_iterator keyIt=trigKeys.begin();keyIt!=trigKeys.end();++keyIt){ 
					const trigger::TriggerObject& obj = trigObjColl[*keyIt];
					//calculate Delta R between the track and the trigger object
					double mydeltaR = dR(obj,p);
					if((mydeltaR<0.1)){
						match++;
						cout<<"Found a possible match in this event with DeltaR = "<<mydeltaR<<" between track with pT = "<<p.pt()<<" trigger object with pT = "<<obj.pt()<<endl;
					}
				}
			}   
		}
     	}

}


// ------------ method called once each job just before starting event loop  ------------
void
TriggMatchingAnalyzer::beginJob()
{
}

double TriggMatchingAnalyzer::dR(const trigger::TriggerObject& obj,  const reco::Track & p )
{
	        double dEta2 =pow( p.eta()-obj.eta(),2); 
			double dPhi2 =pow( p.phi()-obj.phi(),2);
			double dR = sqrt(dPhi2+dEta2);
			return dR;
}

// ------------ method called once each job just after ending the event loop  ------------
void
TriggMatchingAnalyzer::endJob()
{
}

// ------------ method called when starting to processes a run  ------------
void
TriggMatchingAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void
TriggMatchingAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
// ------------ method called when starting to processes a luminosity block  ------------
void
TriggMatchingAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void
TriggMatchingAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TriggMatchingAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
 //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggMatchingAnalyzer);
