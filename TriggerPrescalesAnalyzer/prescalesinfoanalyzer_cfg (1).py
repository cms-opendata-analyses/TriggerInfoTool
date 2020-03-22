import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList 
import FWCore.ParameterSet.Types as CfgTypes

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

#process.options = cms.untracked.PSet(
    #wantSummary = cms.untracked.bool(True)
#)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( *(

'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/06E23025-BC3E-E311-BCD0-003048F01100.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/06EA1332-D83E-E311-84BD-02163E00BA2B.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0811F6D0-F63E-E311-8119-003048F0E3E2.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0821EF2C-F13E-E311-82A6-0025901AF58A.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/083338D7-C83E-E311-A6BA-BCAEC518FF52.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/083FB350-E13E-E311-BA07-003048F1B904.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0870B2A1-0F3F-E311-8D3A-0025901D6284.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0875B156-063F-E311-99FE-003048D462BA.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/087BD0EE-073F-E311-84CD-02163E00ADFE.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/08CD351F-DF3E-E311-B4AD-003048F2377E.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/08F80CDC-FC3E-E311-9275-003048FEAF14.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0A00A5BE-D73E-E311-AE52-02163E00A313.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0A0408EF-EA3E-E311-8501-003048FEAECC.root',


#    'file:00082EAF-C03D-E311-8E53-003048F00B1C.root' 
    ) )
)

#goodJSON = 'Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
goodJSON = 'JsonRun163337.json'
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',')

process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
process.source.lumisToProcess.extend(myLumis)

#needed to get the actual prescale values used from the global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA.db')
process.GlobalTag.globaltag = 'FT_53_LV5_AN1::All'


#configure the analyzer
process.demo = cms.EDAnalyzer('TriggerPrescalesAnalyzer'
    #,filterName = cms.vstring("hltSingleJet60Regional","hltSingleJet100Regional","hltSingleJet140Regional","hltSingleJet200Regional","hltSingleJet260Regional")
    ,filterName1 = cms.string("hltSingleJet60Regional")
    ,filterName2 = cms.string("hltSingleJet100Regional")
    ,filterName3 = cms.string("hltSingleJet140Regional")
    ,filterName4 = cms.string("hltSingleJet200Regional")
)
                              
#configure the TFileservice, in order to save histograms.
process.TFileService = cms.Service("TFileService",
              fileName = cms.string('histo.root')
                                   )
                                   
process.triggerinfo = cms.Path(process.demo)
process.schedule = cms.Schedule(process.triggerinfo)
