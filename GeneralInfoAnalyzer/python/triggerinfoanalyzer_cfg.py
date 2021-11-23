import FWCore.ParameterSet.Config as cms

process = cms.Process("TriggerInfo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#' root://eospublic.cern.ch//eos/opendata/cms/Run2015D/DoubleMuon/AOD/16Dec2015-v1/10000/FEFC93DA-7AA7-E511-AFA7-0025905A6076.root'
' root://eospublic.cern.ch//eos/opendata/cms/Run2015D/JetHT/AOD/16Dec2015-v1/00000/FEF28D68-A4AF-E511-A3FD-0025904C63F8.root' 
 

    )
)

#needed to get the actual prescale values used from the global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA.db')
#process.GlobalTag.globaltag = 'FT_53_LV5_AN1::All'
process.GlobalTag.globaltag = '76X_dataRun2_16Dec2015_v0'

#configure the analyzer
process.gettriggerinfo = cms.EDAnalyzer('TriggerInfoAnalyzer',
                              processName = cms.string("HLT"),
                              triggerName = cms.string("@"), #@ means all triggers        
                              #datasetName = cms.string("SingleMu"), #specific dataset example (for dumping info)          
                              datasetName = cms.string("JetHT"), #specific dataset example (for dumping info)          
                              triggerResults = cms.InputTag("TriggerResults","","HLT"),
                              triggerEvent   = cms.InputTag("hltTriggerSummaryAOD","","HLT")                             
                              )


process.triggerinfo = cms.Path(process.gettriggerinfo)
process.schedule = cms.Schedule(process.triggerinfo)
