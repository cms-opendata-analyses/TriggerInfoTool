import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList 

goodJSON = 'Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',')

process = cms.Process("TriggerInfo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/10000/06E59C16-0A40-E311-B387-02163E008D9F.root', #Not good quality
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/000D4260-D23E-E311-A850-02163E008D77.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/00E41BFD-D93E-E311-A91D-0025901D5DEC.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/022B6AEB-173F-E311-B17D-0025904A90CA.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/02484AF2-E63E-E311-A3F6-003048D2BEE6.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/024E0F96-B93E-E311-8A3C-02163E008DA1.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/027308C4-C73E-E311-8A0C-003048F23FA8.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/02751E29-D73E-E311-B249-02163E00CD78.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0278353F-F23E-E311-AEED-02163E00C482.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/02C290BF-E03E-E311-913B-C860001BD86A.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/02D19806-DD3E-E311-BC8A-002481E0D1F2.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/02E16C94-0B3F-E311-9F2F-02163E00C4FA.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/02E91997-EF3E-E311-8520-0025901AF660.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0420D5E4-B23E-E311-9F88-02163E00BF97.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0422E58A-A53E-E311-9A3E-02163E00ADFE.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/04446C4B-183F-E311-AB6F-02163E008D93.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0444C885-EB3E-E311-AD67-C860001BD834.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0444F8BF-DB3E-E311-9729-003048F02C66.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0471E862-D03E-E311-B57D-02163E00A38A.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/04A4EED5-053F-E311-A2AF-003048F1B904.root',
#'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/04CD866D-EE3E-E311-B148-003048F23676.root'
#    'file:00082EAF-C03D-E311-8E53-003048F00B1C.root' 
    )
)

process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
process.source.lumisToProcess.extend(myLumis)

#needed to get the actual prescale values used from the global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA.db')
process.GlobalTag.globaltag = 'FT_53_LV5_AN1::All'

#configure the analyzer
process.gettriggerinfo = cms.EDAnalyzer('TriggerPrescalesAnalyzer'
	#To have fun with wildcards
	#triggerSelection = cms.vstring('(HLT_Jet190_v*)'), #string 
	#triggerConfiguration =  cms.PSet(
		#hltResults = cms.InputTag('TriggerResults','','HLT'),
		#l1tResults = cms.InputTag('gtDigis'),
		#daqPartitions = cms.uint32(1),
		#l1tIgnoreMask = cms.bool( False ),
		#l1techIgnorePrescales = cms.bool( False ),
		#throw  = cms.bool( True )
	#)
    #,JetInputCollection = cms.InputTag("ak5PFJets")
    ,JetInputCollection = cms.InputTag("ak5CaloJets")
    ,processName = cms.string("HLT")
    #,triggerName = cms.string("HLT_Jet190_v1")
    ,triggerResults = cms.InputTag("TriggerResults","","HLT")
    ,triggerEvent = cms.InputTag("hltTriggerSummaryAOD","","HLT")
    ,filterName = cms.string("hltSingleJet190Regional")
)
                              
#configure the TFileservice, in order to save histograms.
process.TFileService = cms.Service("TFileService",
              fileName = cms.string('histo.root')
                                   )
                                   
process.triggerinfo = cms.Path(process.gettriggerinfo)
process.schedule = cms.Schedule(process.triggerinfo)
