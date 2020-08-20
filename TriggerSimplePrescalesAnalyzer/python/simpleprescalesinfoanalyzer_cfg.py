import FWCore.ParameterSet.Config as cms

process = cms.Process("TriggerInfo")

process.load("FWCore.MessageService.MessageLogger_cfi")
#if more events are activated, choose to print every 1000:
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/00052C5A-EF70-E011-B43F-00266CF32A00.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/00097BA6-F570-E011-905F-002481E94BFE.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/0017CF6D-EF70-E011-9682-003048C691DC.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/003D4F04-EF70-E011-A6B9-003048C65E48.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/0044CAF2-9171-E011-A9DF-002481E0D974.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/022B41E3-C371-E011-B46D-003048F0E5B4.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/024E4ED1-657A-E011-9E51-0025901D493C.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/02A33300-6071-E011-9E33-003048F0E426.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/02C5F94C-F470-E011-8070-0025901D493C.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/04501DDD-957A-E011-A6B0-003048CFCBB2.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/0461F1AB-F570-E011-B928-002481E0D714.root',
    )
)

#needed to get the actual prescale values used from the global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/FT_R_42_V10A.db')
process.GlobalTag.globaltag = 'FT_R_42_V10A::All'

#configure the analyzer
#inspired by https://github.com/cms-sw/cmssw/blob/CMSSW_5_3_X/HLTrigger/HLTfilters/interface/HLTHighLevel.h
process.gettriggerinfo = cms.EDAnalyzer('TriggerSimplePrescalesAnalyzer',
                              processName = cms.string("HLT"),
                              triggerPatterns = cms.vstring("HLT_Jet??U","HLT_Jet???U"), #if left empty, all triggers will run        
                              triggerResults = cms.InputTag("TriggerResults","","HLT"),
                              triggerEvent   = cms.InputTag("hltTriggerSummaryAOD","","HLT")                             
                              )


process.triggerinfo = cms.Path(process.gettriggerinfo)
process.schedule = cms.Schedule(process.triggerinfo)
