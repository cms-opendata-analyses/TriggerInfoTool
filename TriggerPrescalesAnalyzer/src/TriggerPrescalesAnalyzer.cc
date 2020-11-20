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

// Guardar branches
// Buscar TFile service en la twiki
// Además de los histogramas, guardar las variables (branches)
// Guardar los prescale tambien

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

//From TriggMatchingAnalyzer
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

// From AOD2NanoAOD
#include "TFile.h"
#include "TTree.h"

using namespace std;
using namespace edm;
using namespace trigger;
using namespace reco;

//
// class declaration
//

class TriggerPrescalesAnalyzer : public edm::EDAnalyzer
{
public:
	explicit TriggerPrescalesAnalyzer(const edm::ParameterSet &);
	~TriggerPrescalesAnalyzer();

	virtual void beginJob();
	virtual void beginRun(edm::Run const &, edm::EventSetup const &);
	//virtual void beginRun(edm::Run const&, edm::EventSetup const&, const edm::TriggerNames& triggerNames, const edm::TriggerResults& result);
	virtual void analyze(const edm::Event &, const edm::EventSetup &);
	virtual void endRun(edm::Run const &, edm::EventSetup const &);
	virtual void endJob();

	//Methods used for the analysis
	bool checkTriggerPass(const edm::Event &iEvent, const std::string &triggerName); //Check if the trigger passed
	void analyzePrescales(const edm::Event &iEvent, const edm::EventSetup &iSetup, const std::string &tname, const edm::Handle<trigger::TriggerEvent> &trigEvent, const std::string &filterName, const edm::InputTag &trigEventTag, const edm::Handle<reco::PFJetCollection> &jets, TH1D *ht, TH1D *hj, const int &minPt);

	virtual void beginLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const &);
	virtual void endLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const &);
	static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
	TH1D *h1;
	TH1D *H1;
	TH1D *h2;
	TH1D *H2;
	TH1D *h3;
	TH1D *H3;
	TH1D *h4;
	TH1D *H4;

	edm::Service<TFileService> fs; //to access the TFileService object in a framework module

	TTree *tree;

	// Event information
	Int_t value_run;
	UInt_t value_lumi_block;
	ULong64_t value_event;

	// Jet
	const static int max_jet = 1000;
	UInt_t value_jet_n;
	float value_jet_pt[max_jet];

	//Prescale values
	const static int max_prescale = 1000;
	UInt_t value_prescale_n;
	float value_prescale[max_prescale];

	// Trigger
	const static int max_trigger = 1000;
	UInt_t value_trigger_n;
	float value_trigger_pt[max_trigger];

	string filterName1_; //declare de filter (module) of the trigger
	string filterName2_;
	string filterName3_;
	string filterName4_;

	bool isData;

	edm::InputTag trackInput;

	//declare a function to do the trigger analysis
	double dR(const trigger::TriggerObject &, reco::PFJetCollection::const_iterator itjet);

	Handle<TriggerResults> triggerResultsHandle_;
	HLTConfigProvider hltConfig_;
};

//This should match your configuration python file
// TriggerPrescalesAnalyzer::TriggerPrescalesAnalyzer(const edm::ParameterSet& iConfig):
TriggerPrescalesAnalyzer::TriggerPrescalesAnalyzer(const edm::ParameterSet &iConfig) : filterName1_(iConfig.getParameter<string>("filterName1")),
																					   filterName2_(iConfig.getParameter<string>("filterName2")),
																					   filterName3_(iConfig.getParameter<string>("filterName3")),
																					   filterName4_(iConfig.getParameter<string>("filterName4")),
																					   isData(iConfig.getParameter<bool>("isData"))
{
	edm::Service<TFileService> fs;

	tree = fs->make<TTree>("Events", "Events");

	// Event information
	tree->Branch("run", &value_run);
	tree->Branch("luminosityBlock", &value_lumi_block);
	tree->Branch("event", &value_event);

	// Jet
	tree->Branch("nJet", &value_jet_n, "nJet/i");
	tree->Branch("Jet_pt", value_jet_pt, "Jet_pt[nJet]/F");

	// Prescale values
	tree->Branch("nPrescale", &value_prescale_n, "nPrescale/i");
	tree->Branch("Prescale", value_prescale, "Prescale[nPrescale]/F");

	// Trigger
	tree->Branch("nTrigger", &value_trigger_n, "nTrigger/i");
	tree->Branch("Trigger_pt", value_trigger_pt, "Trigger_pt[nTrigger]/F");
}

TriggerPrescalesAnalyzer::~TriggerPrescalesAnalyzer()
{
	// do anything here that needs to be done at desctruction time
	// (e.g. close files, deallocate resources etc.)
}

// -----method called once each job just before starting event loop-----
void TriggerPrescalesAnalyzer::beginJob() //Everything ROOT related
{
	h1 = fs->make<TH1D>("h1", "obj pt 60 ", 100, 0, 300);
	H1 = fs->make<TH1D>("H1", "obj pt 60 ", 100, 0, 300);
	h2 = fs->make<TH1D>("h2", "obj pt 110 ", 100, 0, 300);
	H2 = fs->make<TH1D>("H2", "obj pt 110 ", 100, 0, 300);
	h3 = fs->make<TH1D>("h3", "obj pt 150 ", 100, 0, 300);
	H3 = fs->make<TH1D>("H3", "obj pt 150 ", 100, 0, 300);
	h4 = fs->make<TH1D>("h4", "obj pt 190 ", 100, 0, 300);
	H4 = fs->make<TH1D>("H4", "obj pt 190 ", 100, 0, 300);
} //-----------------------------------------------------------beginJob()

// ------------ method called when starting to processes a run  --------
//void TriggerPrescalesAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup, const edm::TriggerNames& triggerNames, const edm::TriggerResults& result)
void TriggerPrescalesAnalyzer::beginRun(edm::Run const &iRun,
										edm::EventSetup const &iSetup)
{
	///   "init" return value indicates whether intitialisation has succeeded
	///   "changed" parameter indicates whether the config has actually changed
	bool changed(true);
	hltConfig_.init(iRun, iSetup, "HLT", changed);

} //-----------------------------------------------------------beginRun()

// ------------ method called for each event  --------------------------
void TriggerPrescalesAnalyzer::analyze(const edm::Event &iEvent,
									   const edm::EventSetup &iSetup)
{
	// Event information
	value_run = iEvent.run();
	value_lumi_block = iEvent.luminosityBlock();
	value_event = iEvent.id().event();

	InputTag triggerEventTag_("hltTriggerSummaryAOD", "", "HLT");
	InputTag triggerResultsTag_("TriggerResults", "", "HLT");
	//Declare the handle (container) to store trigger objects.
	//InputTag trigEventTag("hltTriggerSummaryAOD","","HLT"); //make sure have correct process on MC -> HLT
	//data process=HLT, MC depends, Spring11 is REDIGI311X
	Handle<TriggerEvent> triggerEventHandle_;
	iEvent.getByLabel(triggerEventTag_, triggerEventHandle_);
	iEvent.getByLabel(triggerResultsTag_, triggerResultsHandle_);

	Handle<reco::PFJetCollection> jets;
	iEvent.getByLabel("ak5PFJets", jets);

	Handle<reco::TrackCollection> mytracks;
	iEvent.getByLabel(trackInput, mytracks);

	if (!triggerResultsHandle_.isValid() || !triggerEventHandle_.isValid() || !jets.isValid())
	{
		cout << "HLTEventAnalyzerAOD::analyze: Error in getting product/s from Event!" << endl;
		return;
	}

	assert(triggerResultsHandle_->size() == hltConfig_.size());

	// Get all trigger names associated with the "Jet" dataset.
	const vector<string> triggerNames = hltConfig_.datasetContent("Jet");

	for (unsigned i = 0; i < triggerNames.size(); i++)
	{
		string name = triggerNames[i];

		// string triggerPath;
		// string triggers[4] = ["60", "80", "110", "150"];

		// for (unsigned i = 0; i < triggers.size(); i++)
		// {
		//     if (name.find("HTL_Jet"+triggers[i]+"_v") != string::npos and checkTriggerPass(iEvent, name))
		//     {
		// 		analyzePrescales(iEvent,iSetup,name,triggerEventHandle_,filterName1_,triggerEventTag_,jets,h1,H1,triggers[i].stoi());
		//     }
		// } //////Wild cards de manera compacta

		if (name.find("HLT_Jet60_v") != string::npos)
		{
			if (checkTriggerPass(iEvent, name))
			{
				analyzePrescales(iEvent, iSetup, name, triggerEventHandle_, filterName1_, triggerEventTag_, jets, h1, H1, 60);
			}
		} //end filter size check

		if (name.find("HLT_Jet80_v") != string::npos)
		{
			if (checkTriggerPass(iEvent, name))
			{
				analyzePrescales(iEvent, iSetup, name, triggerEventHandle_, filterName2_, triggerEventTag_, jets, h2, H2, 80);
			}
		} //end filter size check

		if (name.find("HLT_Jet110_v") != string::npos)
		{
			if (checkTriggerPass(iEvent, name))
			{
				analyzePrescales(iEvent, iSetup, name, triggerEventHandle_, filterName3_, triggerEventTag_, jets, h3, H3, 110);
			}
		} //end filter size check

		if (name.find("HLT_Jet150_v") != string::npos)
		{
			if (checkTriggerPass(iEvent, name))
			{
				analyzePrescales(iEvent, iSetup, name, triggerEventHandle_, filterName4_, triggerEventTag_, jets, h4, H4, 150);
			}
		} //end filter size check
	}

	tree->Fill();

	return;
} //------------------------------------------------------------analyze()

bool TriggerPrescalesAnalyzer::checkTriggerPass(const edm::Event &iEvent,
												const std::string &triggerName)
{
	const unsigned int triggerIndex(hltConfig_.triggerIndex(triggerName));						   //Get the trigger index for the current trigger
	assert(triggerIndex == iEvent.triggerNames(*triggerResultsHandle_).triggerIndex(triggerName)); //check that the trigger in the event and in the configuration agree
	bool acceptedTrigger = triggerResultsHandle_->accept(triggerIndex);							   //Was the trigger accepted? 1 == True, 0 == False.
	cout << "Currently analyzing trigger: " << triggerName << endl;								   //Prints trigger name
	return acceptedTrigger;
} //---------------------------------------------------checkTriggerPass()

void TriggerPrescalesAnalyzer::analyzePrescales(const edm::Event &iEvent,
												const edm::EventSetup &iSetup,
												const std::string &tname,
												const edm::Handle<trigger::TriggerEvent> &trigEvent,
												const std::string &filterName,
												const edm::InputTag &trigEventTag,
												const edm::Handle<reco::PFJetCollection> &myjets,
												TH1D *ht,
												TH1D *hj,
												const int &minPt)
{

	const pair<int, int> prescales(hltConfig_.prescaleValues(iEvent, iSetup, tname));

	cout << "The trigger: " << tname << " has prescale values L1T,HLT: "
		 << prescales.first << "," << prescales.second << endl
		 << "The total prescale value is: " << prescales.first * prescales.second << endl;

	size_type filterIndex = trigEvent->filterIndex(InputTag(filterName, "", trigEventTag.process()));

	value_trigger_n = 0;
	float TriggPt = -9999;

	value_jet_n = 0;
	float JetPt = -9999;

	double minDR = 9999;
	trigger::TriggerObject bestTriggerObj;
	reco::PFJetCollection::const_iterator bestJet;

	if (filterIndex < trigEvent->sizeFilters())
	{
		const Keys &trigKeys = trigEvent->filterKeys(filterIndex);
		const TriggerObjectCollection &trigObjColl(trigEvent->getObjects());
		//now loop of the trigger objects passing filter
		for (trigger::Keys::const_iterator keyIt = trigKeys.begin(); keyIt != trigKeys.end(); ++keyIt)
		{
			const trigger::TriggerObject trigobj = trigObjColl[*keyIt];
			for (reco::PFJetCollection::const_iterator itjet = myjets->begin(); itjet != myjets->end(); ++itjet)
			{
				double thisDR = dR(trigobj, itjet);
				if(thisDR < minDR){
					minDR = thisDR;
					bestJet = itjet;
					bestTriggerObj = trigobj;
				}
			}
		}
		if(minDR < 0.1){
			TriggPt = bestTriggerObj.pt();
			JetPt = bestJet->pt();
			value_trigger_pt[value_trigger_n] = TriggPt;
			value_trigger_n++;
			value_jet_pt[value_jet_n] = JetPt;
			value_jet_n++;
			value_prescale[value_jet_n] = prescales.first * prescales.second;
			ht->Fill(TriggPt, prescales.first * prescales.second);
			hj->Fill(JetPt, prescales.first * prescales.second);
		}			
		cout << "Fired trigger Pt value: " << TriggPt << " with weight: " << prescales.first * prescales.second << endl;
		cout << "Jet Pt value: " << JetPt << " with weight: " << prescales.first * prescales.second << endl;
		cout << "Found a possible match in this event with DeltaR = " << minDR << endl;
	}

	
	// float TriggPt = -999;
	// value_trigger_n = 0;
	// if (filterIndex < trigEvent->sizeFilters())
	// {
	// 	const Keys &trigKeys = trigEvent->filterKeys(filterIndex);
	// 	const TriggerObjectCollection &trigObjColl(trigEvent->getObjects());
	// 	//now loop of the trigger objects passing filter
	// 	for (trigger::Keys::const_iterator keyIt = trigKeys.begin(); keyIt != trigKeys.end(); ++keyIt)
	// 	{
	// 		const trigger::TriggerObject trigobj = trigObjColl[*keyIt];
	// 		if (trigobj.pt() >= TriggPt)
	// 		{
	// 			TriggPt = trigobj.pt();
	// 		}
	// 	}
	// 	value_trigger_pt[value_trigger_n] = TriggPt;
	// 	value_trigger_n++;
	// 	ht->Fill(TriggPt, prescales.first * prescales.second);
	// 	cout << "Fired trigger Pt value: " << TriggPt << " with weight: " << prescales.first * prescales.second << endl;
	// 	// cout << "Found a possible match in this event with DeltaR = " << mydeltaR << endl;
	// }

	// float JetPt = -999;
	// value_jet_n = 0;

	// for (reco::PFJetCollection::const_iterator itjet = myjets->begin(); itjet != myjets->end(); ++itjet)
	// {
	// 	if (itjet->pt() > JetPt)
	// 	{
	// 		JetPt = itjet->pt();
	// 	}
	// }
	// if (JetPt >= minPt)
	// {
	// 	hj->Fill(JetPt, prescales.first * prescales.second);
	// 	value_jet_pt[value_jet_n] = JetPt;
	// 	value_prescale[value_jet_n] = prescales.first * prescales.second;
	// 	value_jet_n++;
	// }
	// cout << "Jet Pt value: " << JetPt << " with weight: " << prescales.first * prescales.second << endl;
}

double TriggerPrescalesAnalyzer::dR(const trigger::TriggerObject &obj, reco::PFJetCollection::const_iterator itjet)
{
	double dEta2 = pow(itjet->eta() - obj.eta(), 2);
	double dPhi2 = pow(itjet->phi() - obj.phi(), 2);
	double dR = sqrt(dPhi2 + dEta2);
	return dR;
}

// ------------ method called when ending the processing of a run  ------------
void TriggerPrescalesAnalyzer::endRun(edm::Run const &, edm::EventSetup const &)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void TriggerPrescalesAnalyzer::endJob()
{

} //-------------------------------------------------------------endJob()

// ------------ method called when starting to processes a luminosity block  ------------
void TriggerPrescalesAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const &)
{

} //-----------------------------------------------beginLuminosityBlock()

// ------------ method called when ending the processing of a luminosity block  ------------
void TriggerPrescalesAnalyzer::endLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const &)
{

} //---------------------------------------------------endLuminosityBlock

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void TriggerPrescalesAnalyzer::fillDescriptions(edm::ConfigurationDescriptions &descriptions)
{
	//The following says we do not know what parameters are allowed so do no validation
	//Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
} //---------------------------------------------------fillDescriptions()

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerPrescalesAnalyzer);