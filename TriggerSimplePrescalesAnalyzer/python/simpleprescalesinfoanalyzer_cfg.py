import FWCore.ParameterSet.Config as cms

process = cms.Process("TriggerInfo")

process.load("FWCore.MessageService.MessageLogger_cfi")
#if more events are activated, choose to print every 1000:
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0C5B7776-D23E-E311-ACB7-002481E0D1F2.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0C9638EB-CA3E-E311-9D83-0025B32441FA.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0CADD1C9-C33E-E311-892C-003048F01074.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0CCF68F1-E73E-E311-A1EE-003048FEAFA0.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0CD213C3-E03E-E311-88B4-002590494DE8.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0E00131B-D73E-E311-9FBF-0025901D6286.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0E38FEF9-103F-E311-81A2-0025901D5C7E.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0E53E0E6-EA3E-E311-8ACC-02163E008D96.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0E5E5D99-AB3E-E311-ACE7-003048F236DE.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0E721DD8-F23E-E311-96C9-02163E00C09C.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0EB3C1F1-C13E-E311-87F6-C860001BD89E.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0EBF2796-D83E-E311-A794-5404A63886EC.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0EC296FC-E63E-E311-83D0-003048F179C2.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0ED83F55-063F-E311-96B0-003048F24354.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/10426894-D33E-E311-BEEC-02163E008D96.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/107DEEF2-C73E-E311-8AE2-02163E008D83.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/107E130C-ED3E-E311-BE60-02163E00A1F2.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/10AE0E26-023F-E311-8861-02163E00C7E1.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/10B2D1EE-F43E-E311-B3BC-5404A63886B1.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/10C529FC-EA3E-E311-B0E1-003048F236DA.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/10D9B1AE-E23E-E311-92BE-0030486730E6.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/10EDF04B-A23E-E311-8D99-003048F02C5A.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/122CFC9F-063F-E311-8719-02163E00CACA.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/123F921D-B33E-E311-844D-C860001BD868.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/124D4CF9-0E3F-E311-B402-02163E00B1AA.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/125206EC-C63E-E311-B48E-002590496FE6.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/12C0AED0-B23E-E311-A9E7-003048F1C764.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/140CAFBD-033F-E311-B5B9-0030486730BE.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/140CFF35-CD3E-E311-93CD-02163E008DB1.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/14444AA8-DC3E-E311-9AEE-0025901AF344.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/1463F0BE-C93E-E311-BBB4-003048FEB9C2.root'
    )
)

#needed to get the actual prescale values used from the global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA.db')
process.GlobalTag.globaltag = 'FT_53_LV5_AN1::All'

#configure the analyzer
#inspired by https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_X/HLTrigger/HLTfilters/interface/HLTHighLevel.h
process.gettriggerinfo = cms.EDAnalyzer('TriggerSimplePrescalesAnalyzer',
                              processName = cms.string("HLT"),
                              triggerPatterns = cms.vstring("HLT_Jet110_v*","HLT_Jet150_v*","HLT_Jet190_v*","HLT_Jet240_v*"), #if left empty, all triggers will run        
                              triggerResults = cms.InputTag("TriggerResults","","HLT"),
                              triggerEvent   = cms.InputTag("hltTriggerSummaryAOD","","HLT")                             
                              )


process.triggerinfo = cms.Path(process.gettriggerinfo)
process.schedule = cms.Schedule(process.triggerinfo)
