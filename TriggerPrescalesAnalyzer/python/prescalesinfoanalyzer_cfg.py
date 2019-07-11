import FWCore.ParameterSet.Config as cms

process = cms.Process("TriggerInfo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
' root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/10000/06E59C16-0A40-E311-B387-02163E008D9F.root' 
#    'file:00082EAF-C03D-E311-8E53-003048F00B1C.root' 
    )
)

#needed to get the actual prescale values used from the global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA.db')
process.GlobalTag.globaltag = 'FT_53_LV5_AN1::All'

#configure the analyzer
process.gettriggerinfo = cms.EDAnalyzer('TriggerPrescalesAnalyzer'
                              #,JetInputCollection = cms.InputTag("ak5PFJets")
                              ,JetInputCollection = cms.InputTag("ak5CaloJets")
                              ,processName = cms.string("HLT")
                              ,triggerName = cms.string("HLT_Jet110_v3")         
                              ,triggerResults = cms.InputTag("TriggerResults","","HLT")
                              ,triggerEvent = cms.InputTag("hltTriggerSummaryAOD","","HLT")
                              )

#configure the TFileservice, in order to save histograms.
process.TFileService = cms.Service("TFileService",
              fileName = cms.string('histo.root')
                                   )
                                   
                                                              
process.triggerinfo = cms.Path(process.gettriggerinfo)
process.schedule = cms.Schedule(process.triggerinfo)
