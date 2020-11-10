import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils ###
import FWCore.PythonUtilities.LumiList as LumiList 
import FWCore.ParameterSet.Types as CfgTypes

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

# process.options = cms.untracked.PSet(
#     wantSummary = cms.untracked.bool(True)
# )

# Set the maximum number of events to be processed (-1 processes all events)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000))

files = FileUtils.loadListFromFile("data/CMS_Run2011A_Jet_AOD_12Oct2013-v1_10000_file_index.txt")
files.extend(FileUtils.loadListFromFile("data/CMS_Run2011A_Jet_AOD_12Oct2013-v1_20000_file_index.txt"))
files.extend(FileUtils.loadListFromFile("data/CMS_Run2011A_Jet_AOD_12Oct2013-v1_20001_file_index.txt"))
files.extend(FileUtils.loadListFromFile("data/CMS_Run2011B_Jet_AOD_12Oct2013-v1_00000_file_index.txt"))
files.extend(FileUtils.loadListFromFile("data/CMS_Run2011B_Jet_AOD_12Oct2013-v1_20000_file_index.txt"))

process.source = cms.Source(
    "PoolSource", fileNames=cms.untracked.vstring(*files))

# process.source = cms.Source("PoolSource",
#     fileNames = cms.untracked.vstring( *(   
# 'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0C5B7776-D23E-E311-ACB7-002481E0D1F2.root',
#     ) )
# )

#needed to get the actual prescale values used from the global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
# process.GlobalTag.connect = cms.string('sqlite_file:/cvmfs/cms-opendata-conddb.cern.ch/FT_53_LV5_AN1_RUNA.db') #I can comment this because I do not need the soft links (Working on lxplus)
process.GlobalTag.globaltag = 'FT_53_LV5_AN1::All'

# Apply JSON file with lumi mask (needs to be done after the process.source definition)
goodJSON = 'data/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt'
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',')
# process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
process.source.lumisToProcess = CfgTypes.untracked(
    CfgTypes.VLuminosityBlockRange())
process.source.lumisToProcess.extend(myLumis)

# Number of events to be skipped (0 by default)
# process.source.skipEvents = cms.untracked.uint32(0)

#configure the analyzer
process.demo = cms.EDAnalyzer('TriggerPrescalesAnalyzer'
    #,filterName = cms.vstring("hltSingleJet60Regional","hltSingleJet100Regional","hltSingleJet140Regional","hltSingleJet200Regional","hltSingleJet260Regional")
    # ,filterName4 = cms.string("hltSingleJet30Regional")
    ,filterName1 = cms.string("hltSingleJet60Regional")
    ,filterName2 = cms.string("hltSingleJet80Regional")
    ,filterName3 = cms.string("hltSingleJet110Regional")
    ,filterName4 = cms.string("hltSingleJet150Regional")
    # ,filterName4 = cms.string("hltSingleJet190Regional")
    # ,filterName4 = cms.string("hltSingleJet240Regional")
    # ,filterName4 = cms.string("hltSingleJet300Regional")
    # ,filterName4 = cms.string("hltSingleJet190Regional")
    # ,filterName4 = cms.string("hltSingleJet370Regional")
    # ,filterName4 = cms.string("hltSingleJet800Regional")
    ,isData = cms.bool(True)
    ,TrackCollection = cms.InputTag("generalTracks")
)
                              
#configure the TFileservice, in order to save histograms.
process.TFileService = cms.Service(
    "TFileService", fileName = cms.string('output.root'))
                                   
process.triggerinfo = cms.Path(process.demo)
process.schedule = cms.Schedule(process.triggerinfo)