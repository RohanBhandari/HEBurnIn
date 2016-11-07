#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "HEBurnIn/ntupler/interface/HcalTupleMaker_Tree.h"
#include "HEBurnIn/ntupler/interface/HcalTupleMaker_QIE11Digis.h"

DEFINE_FWK_MODULE(HcalTupleMaker_Tree);
DEFINE_FWK_MODULE(HcalTupleMaker_QIE11Digis);
