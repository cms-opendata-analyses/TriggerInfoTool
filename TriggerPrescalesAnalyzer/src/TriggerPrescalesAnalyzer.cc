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

//classes to extract jet information
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h" 

//classes to extract trigger information
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

//classes to save data
#include "TTree.h"
#include "TFile.h"
#include "TH1.h"

//class to save the histograms
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

////classes to have fun with wildcards
//#include "HLTrigger/HLTcore/interface/TriggerExpressionData.h"
//#include "HLTrigger/HLTcore/interface/TriggerExpressionEvaluator.h"
//#include "HLTrigger/HLTcore/interface/TriggerExpressionParser.h"

#include<vector>

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


//
// class declaration
//

class TriggerPrescalesAnalyzer : public edm::EDAnalyzer {
	public:
	explicit TriggerPrescalesAnalyzer(const edm::ParameterSet&);
	~TriggerPrescalesAnalyzer();
	
	virtual void beginJob();
	virtual void beginRun(edm::Run const&, edm::EventSetup const&);
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void endRun(edm::Run const&, edm::EventSetup const&);
	virtual void endJob();
	
	//the follwing are not being used here
	virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
	virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
	//Until here
	
	private:
	
	//Methods used for the analysis
	//void analyzeJets(const edm::Event& iEvent, const edm::Handle<reco::PFJetCollection> &jets);
	void analyzeJets(const edm::Event& iEvent, const edm::EventSetup& iSetup, const edm::Handle<reco::CaloJetCollection> &jets);//Jet analysis
	bool checkTriggerPass(const edm::Event& iEvent, const std::string& triggerName);//Check if the trigger passed
	void analyzeTriggObject(const edm::Event& iEvent, const edm::EventSetup& iSetup, const edm::Handle<trigger::TriggerEvent> &trigEvent);//declare a function to do the trigger analysis
	
	//declare the variables for save data in the ROOT files
	TFile *myfile;
	TTree *mytree;
	
	int numjet;//number of jets in the event
	
	int numtrigobj; //number of trigger objects in the event

	//declare histograms and variables that will go into the root files
	TH1D *trighist_pt;
	TH1D *trig_vs_pt;
	
	std::vector<float> jet_pt;
	std::vector<float> trigobj_pt;
	
	edm::Service<TFileService> fs;//to access the TFileService object in a framework module
	
	////To have fun with wild cards
	//triggerExpression::Data m_triggerCache;
	//std::unique_ptr<triggerExpression::Evaluator> m_triggerSelector;
	
	// from HLTEventAnalyzerAOD.h
	/// module config parameters - Have to be in order
	edm::InputTag jetInput_;//declare the input tag for the jet collection
	std::string   processName_;
	std::string   triggerName_;
	edm::InputTag triggerResultsTag_;
	edm::InputTag triggerEventTag_;
	std::string   filterName_;//declare de filter (module) of the trigger  
	
	// these are actually the containers where we will store the trigger information
	edm::Handle<edm::TriggerResults>   triggerResultsHandle_;
	edm::Handle<trigger::TriggerEvent> triggerEventHandle_;
	
	HLTConfigProvider hltConfig_;
};

//This should match your configuration python file
TriggerPrescalesAnalyzer::TriggerPrescalesAnalyzer(const edm::ParameterSet& iConfig):
//To have fun with wild cards
//m_triggerCache(iConfig.getParameterSet("triggerConfiguration") ),
//m_triggerSelector(triggerExpression::parse( iConfig.getParameter<std::string>("triggerSelection") ) ),
jetInput_(iConfig.getParameter<edm::InputTag>("JetInputCollection")),
processName_(iConfig.getParameter<std::string>("processName")),
triggerName_("HLT_Jet150_v1"),
triggerResultsTag_(iConfig.getParameter<edm::InputTag>("triggerResults")),
triggerEventTag_(iConfig.getParameter<edm::InputTag>("triggerEvent")),
filterName_(iConfig.getParameter<std::string>("filterName"))
{
	using namespace std;
	using namespace edm;
	
	//this takes the type of object (input tag) specified in the configuration python file, corresponding to each container.
	//https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideRecoDataTable
	//this take the filter (module) of the trigger
	
	//Print the configuration just to check
	cout<<"Here is the information passed to the constructor:"<<endl;
	cout<<"HLTEventAnalyzerAOD configuration: "<< endl
		<<"		Jet Input Collection = "<< jetInput_.encode() << endl
		<<"		ProcessName = "<< processName_ << endl
		<<"		TriggerName = "<< triggerName_ << endl
		//<<"		TriggerName = "<< m_triggerSelector << endl//To have fun with wild cards
		<<"		TriggerResultsTag = "<< triggerResultsTag_.encode() << endl
		<<"		TriggerEventTag = "<< triggerEventTag_.encode() << endl
		<<"		FilterName = "<<filterName_<<endl;
}

TriggerPrescalesAnalyzer::~TriggerPrescalesAnalyzer()
{
	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)
}

// -----method called once each job just before starting event loop-----
void TriggerPrescalesAnalyzer::beginJob()//Everything ROOT related
{
	using namespace std;
	using namespace edm;

	myfile = new TFile("TriggerPrescaleExample.root","RECREATE");
	mytree = new TTree("mytree","Trigger accept information");

	trig_vs_pt = fs->make <TH1D>("Trig vs pt", "Trig vs pt ", 100,0,2000);
	mytree->Branch("jet_pt",&jet_pt);
	
	trighist_pt = fs->make <TH1D>("trighist_pt", "obj pt ", 100,0,2000);
	mytree->Branch("trigobj_pt",&trigobj_pt);
}//-----------------------------------------------------------beginJob()

// ------------ method called when starting to processes a run  --------
void TriggerPrescalesAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
	using namespace std;
	using namespace edm;
	
	///   "init" return value indicates whether intitialisation has succeeded
	///   "changed" parameter indicates whether the config has actually changed
	
	bool changed(true);
	
	if (hltConfig_.init(iRun,iSetup,processName_,changed)) {
		if (changed) {
			// check if trigger name in (new) config
			if (triggerName_!="@") {
			//if (m_triggerSelector!="@") {//To have fun with wild cards
				const unsigned int n(hltConfig_.size());
				const unsigned int triggerIndex(hltConfig_.triggerIndex(triggerName_));
				//const unsigned int triggerIndex(hltConfig_.triggerIndex(m_triggerSelector));//To have fun with wild cards
				while (triggerIndex>=n) {
					cout<<"HLTEventAnalyzerAOD::analyze:"
						<<" TriggerName "<<triggerName_ 
						//<<" TriggerName "<<m_triggerSelector//To have fun with wild cards
						<<" not available in (new) config!"<<endl;
					triggerName_ = "HLT_Jet150_v1";
					if (hltConfig_.triggerIndex(triggerName_)<n) {
						cout<<"Now trying the trigger: "<<triggerName_<<endl;
						break;
					}
					triggerName_ = "HLT_Jet150_v2";
					if (hltConfig_.triggerIndex(triggerName_)<n) {
						cout<<"Now trying the trigger: "<<triggerName_<<endl;
						break;
					}
					triggerName_ = "HLT_Jet150_v3";
					if (hltConfig_.triggerIndex(triggerName_)<n) {
						cout<<"Now trying the trigger: "<<triggerName_<<endl;
						break;
					}
					triggerName_ = "HLT_Jet150_v4";
					if (hltConfig_.triggerIndex(triggerName_)<n) {
						cout<<"Now trying the trigger: "<<triggerName_<<endl;
						break;
					}
					triggerName_ = "HLT_Jet150_v5";
					if (hltConfig_.triggerIndex(triggerName_)<n) {
						cout<<"Now trying the trigger: "<<triggerName_<<endl;
						break;
					}
					triggerName_ = "HLT_Jet150_v6";
					if (hltConfig_.triggerIndex(triggerName_)<n) {
						cout<<"Now trying the trigger: "<<triggerName_<<endl;
						break;
					}
					triggerName_ = "HLT_Jet150_v9";
					if (hltConfig_.triggerIndex(triggerName_)<n) {
						cout<<"Now trying the trigger: "<<triggerName_<<endl;
						break;
					}
					if (hltConfig_.triggerIndex(triggerName_)>=n) {
						cout<<"Available TriggerNames are: "<<endl;
						hltConfig_.dump("Triggers");
						break;
					}
				}	
			}	
		}	
	}else{
		cout<<"HLTEventAnalyzerAOD::analyze:"
		<<" config extraction failure with process name "
		<<processName_ << endl;
	}     
	
	//hltConfig_.dump("Triggers");
	
}//-----------------------------------------------------------beginRun()

// ------------ method called for each event  --------------------------
void TriggerPrescalesAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace std;
	
	//Declare the handle (container) to store trigger objects.    
	//InputTag trigEventTag("hltTriggerSummaryAOD","","HLT"); //make sure have correct process on MC -> HLT
	//data process=HLT, MC depends, Spring11 is REDIGI311X
	
	//iEvent.getByLabel(triggerEventTag_, triggerEventHandle_);
	iEvent.getByLabel(triggerResultsTag_,triggerResultsHandle_);
	
	if (!triggerResultsHandle_.isValid()) {
		cout<<"HLTEventAnalyzerAOD::analyze: Error in getting TriggerResults product from Event!"<<endl;
		return;
	}
	
	assert(triggerResultsHandle_->size()==hltConfig_.size());
	
	//if (m_triggerSelector and m_triggerCache.setEvent(event, setup)) {//To have fun with wild cards
		//// if the L1 or HLT configurations have changed, (re)initialize the filters (including during the first event)
		//if (m_triggerCache.configurationUpdated())
		//m_triggerSelector ->init(m_triggerCache);
		//bool result = (*m_triggerSelector)(m_triggerCache);
	//}
	
	if (!checkTriggerPass(iEvent, triggerName_)){
	//if (!checkTriggerPass(iEvent, m_triggerSelector)){//To have fun with wild cards
		return;
	}
	
	cout<<"********************************************************************************************************************Hey I passed the trigger"<<endl;
	
	//Continue to analyze the data
			
	//Handle<reco::PFJetCollection> myjets;
	Handle<reco::CaloJetCollection> myjets;//Declare the handle (container) to store jets.	
	iEvent.getByLabel(jetInput_, myjets);
	analyzeJets(iEvent, iSetup, myjets);
	
	Handle<trigger::TriggerEvent> mytrigEvent;//Declare the handle (container) to store trigger objects.
	iEvent.getByLabel(triggerEventTag_,mytrigEvent);
	analyzeTriggObject(iEvent,iSetup,mytrigEvent);
	
	mytree->Fill(); //Now, the information is stored.
	
	return;
}//------------------------------------------------------------analyze()

bool TriggerPrescalesAnalyzer::checkTriggerPass(const edm::Event& iEvent, const std::string& triggerName)
{
	using namespace std;
	
	const unsigned int triggerIndex(hltConfig_.triggerIndex(triggerName));//Get the trigger index for the current trigger
	
	assert(triggerIndex==iEvent.triggerNames(*triggerResultsHandle_).triggerIndex(triggerName));//check that the trigger in the event and in the configuration agree
	
	bool acceptedTrigger = triggerResultsHandle_->accept(triggerIndex);//Was the trigger accepted? 1 == True, 0 == False.
	//bool runTrigger = triggerResultsHandle_->wasrun(triggerIndex);//Was the trigger run? 1 == True, 0 == False.
	//bool errorTrigger = triggerResultsHandle_->error(triggerIndex);//Was the trigger with an error? 1 == True, 0 == False.
	
	cout<<"Currently analyzing trigger: "<<triggerName<<endl;//Prints trigger name
	
	//cout<<"The trigger was accepted: "<<acceptedTrigger<<endl
		//<<" The trigger was run: "<<runTrigger<<endl
		//<<" The trigger had an error: "<<errorTrigger<<endl;
	
	return acceptedTrigger;	
}//---------------------------------------------------checkTriggerPass()

void TriggerPrescalesAnalyzer::analyzeJets(const edm::Event& iEvent, const edm::EventSetup& iSetup, const edm::Handle<reco::CaloJetCollection> &jets)
{
	using namespace std;
	
	numjet = 0;
	jet_pt.clear();
		
	const std::pair<int,int> prescales(hltConfig_.prescaleValues(iEvent,iSetup,triggerName_));
	
	if(jets.isValid()) {
		numjet=(*jets).size();// get the number of jets in the event
		
		for (reco::CaloJetCollection::const_iterator itjet=jets->begin(); itjet!=jets->end(); ++itjet) {
			jet_pt.push_back(itjet->pt());//Fill the vector
			
			cout<<"Jet Pt: "<<itjet->pt()
				<<" -- Px: "<<itjet->px()
				<<" -- Py: "<<itjet->py()
				<<" -- Pz: "<<itjet->pz()
				<<" -- Energy: "<<itjet->energy()<<endl;			
		}
		
		//std::cout<<"Maximum value: "<<*std::max_element(jet_pt.begin(), jet_pt.end())<<"First value: "<<jet_pt.front()<<std::endl; //Shows the max and first value of the vector
		
		//trig_vs_pt->Fill(*std::max_element(jet_pt.begin(), jet_pt.end()),prescales.first*prescales.second);//Fill(*std::max_element(jet_pt.begin(), jet_pt.end()), weight)
		//trig_vs_pt->Fill(*std::max_element(jet_pt.begin(), jet_pt.end()));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		trig_vs_pt->Fill(*std::max_element(jet_pt.begin(), jet_pt.end()), prescales.first*prescales.second);
		
		if (*std::max_element(jet_pt.begin(), jet_pt.end()) < 40) {
			cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$Look at me"<<endl;
		}
	}
}//--------------------------------------------------------analyzeJets()

//Commented because we are using Calo instead of PF (Particle Flow)
//void TriggerPrescalesAnalyzer::analyzeJets(const edm::Event& iEvent, const edm::Handle<reco::PFJetCollection> &jets)
//{
	//using namespace std;
	
	//numjet = 0;
	//jet_pt.clear();
	
	//if(jets.isValid()) {
		//numjet=(*jets).size();// get the number of jets in the event
		////int myidx = 0;
		
		//for (reco::PFJetCollection::const_iterator itjet=jets->begin(); itjet!=jets->end(); ++itjet) {
			//jet_pt.push_back(itjet->pt());//Fill the vector
			
			//cout<<"Jet Pt: "<<itjet->pt()
				//<<" -- Px: "<<itjet->px()
				//<<" -- Py: "<<itjet->py()
				//<<" -- Pz: "<<itjet->pz()
				//<<" -- Energy: "<<itjet->energy()<<endl;
			
			////if (myidx==0) {trig_vs_pt->Fill(itjet->pt());}
			////myidx++;    	    
		//}
		
		////std::cout<<"Maximum value: "<<*std::max_element(jet_pt.begin(), jet_pt.end())<<"First value: "<<jet_pt.front()<<std::endl; //Shows the max and first value of the vector
		
		//trig_vs_pt->Fill(*std::max_element(jet_pt.begin(), jet_pt.end()));
	//}
//}//--------------------------------------------------------analyzeJets()

void TriggerPrescalesAnalyzer::analyzeTriggObject(const edm::Event& iEvent, const edm::EventSetup& iSetup, const edm::Handle<trigger::TriggerEvent> &trigEvent)
{
	using namespace std;
	
	numtrigobj = 0;
	trigobj_pt.clear();
	
	const std::pair<int,int> prescales(hltConfig_.prescaleValues(iEvent,iSetup,triggerName_));
	
	cout << "The trigger: "<< triggerName_<<" has prescale values L1T,HLT: " 
	<< prescales.first << "," << prescales.second<< endl
	<<"The total prescale value is: "<<prescales.first*prescales.second<<endl;

	trigger::size_type filterIndex = trigEvent->filterIndex(edm::InputTag(filterName_,"",triggerEventTag_.process()));
	
	if(filterIndex < trigEvent->sizeFilters()) {
		const trigger::Keys& trigKeys = trigEvent->filterKeys(filterIndex); 
		const trigger::TriggerObjectCollection & trigObjColl(trigEvent->getObjects());
		
		//now loop of the trigger objects passing filter
		for(trigger::Keys::const_iterator keyIt=trigKeys.begin();keyIt!=trigKeys.end();++keyIt) {
			const trigger::TriggerObject trigobj = trigObjColl[*keyIt];
			trigobj_pt.push_back(trigobj.pt());

			//trighist_pt->Fill(trigobj.pt());
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			trighist_pt->Fill(trigobj.pt(), prescales.first*prescales.second);
			cout<<"Fired trigger Pt value: "<<trigobj.pt()<<endl;
			numtrigobj=numtrigobj+1;
		}
	}//end filter size check
}//-------------------------------------------------analyzeTriggObject()

// ------------ method called when ending the processing of a run  ------------
void TriggerPrescalesAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
	
}

// ------------ method called once each job just after ending the event loop  ------------
void TriggerPrescalesAnalyzer::endJob()
{
	myfile->Write();
}//-------------------------------------------------------------endJob()


















//NOT USED HERE 

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
