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
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/02E16C94-0B3F-E311-9F2F-02163E00C4FA.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/02E91997-EF3E-E311-8520-0025901AF660.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0420D5E4-B23E-E311-9F88-02163E00BF97.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0422E58A-A53E-E311-9A3E-02163E00ADFE.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/04446C4B-183F-E311-AB6F-02163E008D93.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0444C885-EB3E-E311-AD67-C860001BD834.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0444F8BF-DB3E-E311-9729-003048F02C66.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0471E862-D03E-E311-B57D-02163E00A38A.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/04A4EED5-053F-E311-A2AF-003048F1B904.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/04CD866D-EE3E-E311-B148-003048F23676.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/04D9EECE-F03E-E311-910B-02163E0079DA.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/04DE8625-BC3E-E311-8E8B-003048F1C9AC.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/04FC376C-053F-E311-9BAC-02163E0079D5.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/06318261-F83E-E311-B60D-003048CF92B8.root',
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
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0A046A87-043F-E311-B19F-02163E00CE1C.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0A07557B-043F-E311-A05E-02163E008D9E.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0A41D176-F33E-E311-9E3C-02163E00CAC5.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0A4C2BB0-EB3E-E311-9238-02163E00AD22.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0A6E33A9-E63E-E311-8E0E-C860001BD8BC.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0A73DA1F-FB3E-E311-9D96-02163E00CDF6.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0A817C2C-E43E-E311-823C-003048F1C1A0.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0A909CE8-E43E-E311-A769-02163E00AE41.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0A9FFC83-DA3E-E311-AD1C-003048FEAF90.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0AC9521D-AD3E-E311-BC7B-003048F1182C.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/0AD989E0-E33E-E311-AEF9-02163E00CC4F.root',
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
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/1463F0BE-C93E-E311-BBB4-003048FEB9C2.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/1475CD2D-113F-E311-A514-00237DDBE8CC.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/147929CD-DA3E-E311-ACDD-003048F01146.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/14792B5E-CA3E-E311-B877-003048F1C9A6.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/1488213F-D03E-E311-B09B-003048FEAEB0.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/14997B2C-AF3E-E311-B1A0-0025904B2C5A.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/14A72041-143F-E311-A4C3-002481E0D1F2.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/14AE996A-FC3E-E311-8ACE-02163E00C010.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/14E46207-D93E-E311-9321-003048CF9AE0.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/14F5FBD5-F63E-E311-8480-3C4A9279B99C.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/14F720D9-EA3E-E311-8540-02163E00BFED.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/16A08478-BF3E-E311-9C70-BCAEC5329720.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/18146DB6-D73E-E311-828B-003048F1C570.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/187A3C72-043F-E311-BBE7-003048F1B952.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/1880FB04-0C3F-E311-A832-003048F02D3A.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/189297B5-163F-E311-83D3-003048FE4C1E.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/18ABE119-FF3E-E311-935B-003048F1C41C.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/18B5A08D-CC3E-E311-86A0-02163E00A109.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/18C60B4C-FB3E-E311-AD4A-003048F23848.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/18DFFB68-EE3E-E311-AC75-003048F1DB5C.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/18F089E7-1B4E-E311-812F-003048CFB3AE.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/18FE9C62-F83E-E311-80A2-003048FEB9C2.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/1A0A9D6C-EF3E-E311-B855-02163E00ACFF.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/1A18B43E-D03E-E311-BF21-003048CF9E54.root',
'root://eospublic.cern.ch//eos/opendata/cms/Run2011A/Jet/AOD/12Oct2013-v1/20000/1A2C8D87-173F-E311-9F16-02163E00C7A8.root',


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
    ,filterName2 = cms.string("hltSingleJet90Regional")
    ,filterName3 = cms.string("hltSingleJet110Regional")
    ,filterName4 = cms.string("hltSingleJet190Regional")
)
                              
#configure the TFileservice, in order to save histograms.
process.TFileService = cms.Service("TFileService",
              fileName = cms.string('histo.root')
                                   )
                                   
process.triggerinfo = cms.Path(process.demo)
process.schedule = cms.Schedule(process.triggerinfo)
