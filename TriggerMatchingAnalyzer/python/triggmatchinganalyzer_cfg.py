import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
#if more events are activated, choose to print every 100:
#process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/00052C5A-EF70-E011-B43F-00266CF32A00.root'
	
    )
)
#needed to get the actual prescale values used from the global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/FT_R_42_V10A.db')
process.GlobalTag.globaltag = 'FT_R_42_V10A::All'

#This is inspired by https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHLTAnalysis
process.demo = cms.EDAnalyzer('TriggMatchingAnalyzer',
                 filterName = cms.string("hlt1jet100U"),
                 TrackCollection = cms.InputTag("generalTracks")
                             )




process.p = cms.Path(process.demo)
