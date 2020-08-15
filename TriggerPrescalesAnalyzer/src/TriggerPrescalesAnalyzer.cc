// -*- C++ -*-
//
// Package:    TriggerPrescalesAnalyzer
// Class:      TriggerPrescalesAnalyzer
//
/**\class TriggerPrescalesAnalyzer TriggerPrescalesAnalyzer.cc TriggerInfoTool/TriggerPrescalesAnalyzer/src/TriggerPrescalesAnalyzer.cc
 Description: [one line class summary]
 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Edgar Carrera
//         Created:  Mon Jul  9 15:59:18 CEST 2019
// $Id$
//
//

//**

//classes to save data
#include "TH1.h"
#include "TCanvas.h"
#include "THistPainter.h"
#include "TAttMarker.h"
#include "TStyle.h"


//classes to extract trigger information
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"


//class to save the histograms
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//classes to extract PFJet information
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include <iosfwd>

//**

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

using namespace std;
using namespace edm;
using namespace trigger;
using namespace reco;

//
// class declaration
//

class TriggerPrescalesAnalyzer : public edm::EDAnalyzer {
	public:
	explicit TriggerPrescalesAnalyzer(const edm::ParameterSet&);
	~TriggerPrescalesAnalyzer();

	virtual void beginJob();
	virtual void beginRun(edm::Run const&, edm::EventSetup const&);
	//virtual void beginRun(edm::Run const&, edm::EventSetup const&, const edm::TriggerNames& triggerNames, const edm::TriggerResults& result);
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endRun(edm::Run const&, edm::EventSetup const&);
	virtual void endJob();
	void analyzePrescales(const edm::Event& iEvent
    , const edm::EventSetup& iSetup
	, const std::string& tname
	, const edm::Handle<trigger::TriggerEvent> &trigEvent
	, const std::string& filterName
	, const edm::InputTag &trigEventTag
	, const edm::Handle<reco::PFJetCollection> &jets
	, TH1D* ht
	, TH1D* hj
	, const int &minPt
	);

	//the follwing are not being used here
	virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
	virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
	//Until here

	private:

	//Methods used for the analysis
	bool checkTriggerPass(const edm::Event& iEvent, const std::string& triggerName);//Check if the trigger passed
	//declare histograms and variables that will go into the root files

    TH1D *h1;
	TH1D *H1;
	TH1D *h2;
	TH1D *H2;
	TH1D *h3;
	TH1D *H3;
	TH1D *h4;
	TH1D *H4;

	edm::Service<TFileService> fs;//to access the TFileService object in a framework module

	string filterName1_;//declare de filter (module) of the trigger
	string filterName2_;
	string filterName3_;
	string filterName4_;
	
	Handle<TriggerResults> triggerResultsHandle_;

	HLTConfigProvider hltConfig_;

};

//This should match your configuration python file
TriggerPrescalesAnalyzer::TriggerPrescalesAnalyzer(const edm::ParameterSet& iConfig):
filterName1_(iConfig.getParameter<string>("filterName1")),
filterName2_(iConfig.getParameter<string>("filterName2")),
filterName3_(iConfig.getParameter<string>("filterName3")),
filterName4_(iConfig.getParameter<string>("filterName4"))
{
}

TriggerPrescalesAnalyzer::~TriggerPrescalesAnalyzer()
{
	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)
}

// -----method called once each job just before starting event loop-----
void TriggerPrescalesAnalyzer::beginJob()//Everything ROOT related
{
  h1 = fs->make <TH1D>("h1","obj pt 60 ",100,0,300);
  H1 = fs->make <TH1D>("H1","obj pt 60 ",100,0,300);
  h2 = fs->make <TH1D>("h2","obj pt 110 ",100,0,300);
  H2 = fs->make <TH1D>("H2","obj pt 110 ",100,0,300);
  h3 = fs->make <TH1D>("h3","obj pt 150 ",100,0,300);
  H3 = fs->make <TH1D>("H3","obj pt 150 ",100,0,300);
  h4 = fs->make <TH1D>("h4","obj pt 190 ",100,0,300);
  H4 = fs->make <TH1D>("H4","obj pt 190 ",100,0,300);

}//-----------------------------------------------------------beginJob()

// ------------ method called when starting to processes a run  --------
//void TriggerPrescalesAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup, const edm::TriggerNames& triggerNames, const edm::TriggerResults& result)
void TriggerPrescalesAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
	///   "init" return value indicates whether intitialisation has succeeded
	///   "changed" parameter indicates whether the config has actually changed
		bool changed(true);
		hltConfig_.init(iRun,iSetup,"HLT",changed);

}//-----------------------------------------------------------beginRun()

// ------------ method called for each event  --------------------------
void TriggerPrescalesAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{ 
	InputTag triggerEventTag_("hltTriggerSummaryAOD","","HLT");
	InputTag triggerResultsTag_("TriggerResults","","HLT");
	//Declare the handle (container) to store trigger objects.
	//InputTag trigEventTag("hltTriggerSummaryAOD","","HLT"); //make sure have correct process on MC -> HLT
	//data process=HLT, MC depends, Spring11 is REDIGI311X
  Handle<TriggerEvent> triggerEventHandle_;
	iEvent.getByLabel(triggerEventTag_, triggerEventHandle_);
	iEvent.getByLabel(triggerResultsTag_,triggerResultsHandle_);

	Handle<reco::PFJetCollection> jets;
	iEvent.getByLabel("ak5PFJets", jets);

	if (!triggerResultsHandle_.isValid() || !triggerEventHandle_.isValid() || !jets.isValid()) {
		cout<<"HLTEventAnalyzerAOD::analyze: Error in getting product/s from Event!"<<endl;
		return;
	}
	
    
    assert(triggerResultsHandle_->size()==hltConfig_.size());

  // Get all trigger names associated with the "Jet" dataset.
  const vector<string> triggerNames = hltConfig_.datasetContent("Jet");

	for (unsigned i = 0; i < triggerNames.size(); i++) {
    string name = triggerNames[i];
    

	if (name.find("HLT_Jet60_v") != string::npos) {
	if (checkTriggerPass(iEvent, name)){
		
		analyzePrescales(iEvent,iSetup,name,triggerEventHandle_,filterName1_,triggerEventTag_,jets,h1,H1,60);

    }}//end filter size check


 	if (name.find("HLT_Jet80_v") != string::npos) {
	if (checkTriggerPass(iEvent, name)){
		
		analyzePrescales(iEvent,iSetup,name,triggerEventHandle_,filterName2_,triggerEventTag_,jets,h2,H2,80);

 	}}//end filter size check


 	if (name.find("HLT_Jet110_v") != string::npos) {
	if (checkTriggerPass(iEvent, name)){
		
		analyzePrescales(iEvent,iSetup,name,triggerEventHandle_,filterName3_,triggerEventTag_,jets,h3,H3,110);

 	}}//end filter size check


 	if (name.find("HLT_Jet150_v") != string::npos) {
	if (checkTriggerPass(iEvent, name)){
		
		analyzePrescales(iEvent,iSetup,name,triggerEventHandle_,filterName4_,triggerEventTag_,jets,h4,H4,150);

 	}}//end filter size check

}
	return;
}//------------------------------------------------------------analyze()

bool TriggerPrescalesAnalyzer::checkTriggerPass(const edm::Event& iEvent, const std::string& triggerName)
{
	const unsigned int triggerIndex(hltConfig_.triggerIndex(triggerName));//Get the trigger index for the current trigger
	assert(triggerIndex==iEvent.triggerNames(*triggerResultsHandle_).triggerIndex(triggerName));//check that the trigger in the event and in the configuration agree
	bool acceptedTrigger = triggerResultsHandle_->accept(triggerIndex);//Was the trigger accepted? 1 == True, 0 == False.
	cout<<"Currently analyzing trigger: "<<triggerName<<endl;//Prints trigger name
	return acceptedTrigger;
}//---------------------------------------------------checkTriggerPass()



void TriggerPrescalesAnalyzer::analyzePrescales(const edm::Event& iEvent
, const edm::EventSetup& iSetup
, const std::string& tname
, const edm::Handle<trigger::TriggerEvent> &trigEvent
, const std::string& filterName
, const edm::InputTag &trigEventTag
, const edm::Handle<reco::PFJetCollection> &myjets
,  TH1D* ht
,  TH1D* hj
, const int &minPt
){

const  pair<int,int> prescales(hltConfig_.prescaleValues(iEvent,iSetup,tname));

 	cout << "The trigger: "<< tname<<" has prescale values L1T,HLT: "
 	<< prescales.first << "," << prescales.second<< endl
 	<<"The total prescale value is: "<<prescales.first*prescales.second<<endl;

 	size_type filterIndex = trigEvent->filterIndex(InputTag(filterName,"",trigEventTag.process()));
    double TriggPt = -999;
 	if(filterIndex < trigEvent->sizeFilters()) {
 	   const Keys& trigKeys = trigEvent->filterKeys(filterIndex);
 	   const TriggerObjectCollection & trigObjColl(trigEvent->getObjects());
 		//now loop of the trigger objects passing filter
 	  	for(trigger::Keys::const_iterator keyIt=trigKeys.begin();keyIt!=trigKeys.end();++keyIt) {
 			  const trigger::TriggerObject trigobj = trigObjColl[*keyIt];
 			  if (TriggPt <= trigobj.pt()){TriggPt=trigobj.pt();}
 		}
 		ht->Fill(TriggPt, prescales.first*prescales.second);
 		cout<<"Fired trigger Pt value: "<<TriggPt<<endl;
 	}
    double JetPt = -999;
	for (reco::PFJetCollection::const_iterator itjet=myjets->begin(); itjet!=myjets->end(); ++itjet) {
		
		if (JetPt <= itjet->pt()){JetPt=itjet->pt();}
	    
	}
	if (JetPt >= minPt){
	hj->Fill(JetPt, prescales.first*prescales.second);
    }
	
}


// ------------ method called when ending the processing of a run  ------------
void TriggerPrescalesAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{

}

// ------------ method called once each job just after ending the event loop  ------------
void TriggerPrescalesAnalyzer::endJob()
{

}//-------------------------------------------------------------endJob()

// ------------ method called when starting to processes a luminosity block  ------------
void
TriggerPrescalesAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{

}//-----------------------------------------------beginLuminosityBlock()

// ------------ method called when ending the processing of a luminosity block  ------------
void
TriggerPrescalesAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{

}//---------------------------------------------------endLuminosityBlock

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TriggerPrescalesAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions){
	//The following says we do not know what parameters are allowed so do no validation
	//Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}//---------------------------------------------------fillDescriptions()

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerPrescalesAnalyzer);
