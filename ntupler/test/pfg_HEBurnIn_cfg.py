#------------------------------------------------------------------------------------
# Imports
#------------------------------------------------------------------------------------
import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
import FWCore.ParameterSet.VarParsing as VarParsing


#------------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------------

options = VarParsing.VarParsing()

options.register('skipEvents',
                 0, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to skip")

options.register('processEvents',
                 100000, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Number of events to process")

options.register('inputFiles',
                 "file:/afs/cern.ch/user/r/rbhandar/test/CMSSW_8_0_16/src/HEBurnIn/ntupler/B904_Integration_1000023284.root", #default value
                 VarParsing.VarParsing.multiplicity.list,
                 VarParsing.VarParsing.varType.string,
                 "Input files")

options.register('outputFile',
                 "outputFile_"+options.inputFiles[0].split('ntupler/')[1], #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Output file")

options.parseArguments()

print "Skip events =", options.skipEvents
print "Process events =", options.processEvents
print "inputFiles =", options.inputFiles
print "outputFile =", options.outputFile


#------------------------------------------------------------------------------------
# Declare the process and input variables
#------------------------------------------------------------------------------------
process = cms.Process('PFG')

#------------------------------------------------------------------------------------
# Get and parse the command line arguments
#------------------------------------------------------------------------------------
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.processEvents) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1001) )
process.source = cms.Source("HcalTBSource",
    fileNames  = cms.untracked.vstring(options.inputFiles[0]),
    skipEvents = cms.untracked.uint32(options.skipEvents),
)

process.TFileService = cms.Service("TFileService",
     fileName = cms.string(options.outputFile)
)

#------------------------------------------------------------------------------------
# import of standard configurations
#------------------------------------------------------------------------------------
process.load('Configuration.Geometry.GeometryIdeal_cff')
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("CondCore.DBCommon.CondDBSetup_cfi")

#------------------------------------------------------------------------------------
# Set up our analyzer
#------------------------------------------------------------------------------------
process.load("HEBurnIn.ntupler.HcalTupleMaker_Tree_cfi")
process.load("HEBurnIn.ntupler.HcalTupleMaker_QIE11Digis_cfi")

#------------------------------------------------------------------------------------
# FED numbers
#------------------------------------------------------------------------------------
process.qie11Digis = cms.EDProducer("HcalRawToDigi",
#       UnpackHF = cms.untracked.bool(True),
        ### Flag to enable unpacking of TTP channels(default = false)
        ### UnpackTTP = cms.untracked.bool(True),
        FilterDataQuality = cms.bool(False),
        InputLabel = cms.InputTag('source'),
        HcalFirstFED = cms.untracked.int32(700),
        ComplainEmptyData = cms.untracked.bool(False),
#       UnpackCalib = cms.untracked.bool(True),
        firstSample = cms.int32(0),
        lastSample = cms.int32(9) #use 9 for 10 TS
)

#------------------------------------------------------------------------------------
# Specify Global Tag
#------------------------------------------------------------------------------------
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = '80X_dataRun2_Prompt_v10'


#   EMAP
process.es_ascii = cms.ESSource('HcalTextCalibrations',
        input = cms.VPSet(
               cms.PSet(
                object = cms.string('ElectronicsMap'),
                file = cms.FileInPath('HEBurnIn/ntupler/data/2016-nov-07/HE1-18_904_emap.txt')  # EMAP here!
               )
        )
)
process.es_prefer = cms.ESPrefer('HcalTextCalibrations', 'es_ascii')

#------------------------------------------------------------------------------------
# HcalTupleMaker sequence definition
#------------------------------------------------------------------------------------
process.tuple_step = cms.Sequence(
    # Make HCAL tuples: digi info
    process.hcalTupleQIE11Digis*
    # Package everything into a tree
    process.hcalTupleTree
)


#-----------------------------------------------------------------------------------
# Path and EndPath definitions
#-----------------------------------------------------------------------------------
process.preparation = cms.Path(
    # Unpack digis from RAW
    process.qie11Digis*
    # Make the ntuples
    process.tuple_step
)
