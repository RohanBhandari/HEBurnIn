#ifndef HcalTupleMaker_QIE11Digis_h
#define HcalTupleMaker_QIE11Digis_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

class HcalTupleMaker_QIE11Digis : public edm::EDProducer {
public:
  explicit HcalTupleMaker_QIE11Digis(const edm::ParameterSet&);

private:
  void produce( edm::Event &, const edm::EventSetup & ); 
  std::string prefix,suffix; 
  const edm::InputTag m_qie11DigisTag;
  edm::EDGetTokenT<HcalDataFrameContainer<QIE11DataFrame> > qie11digisToken_;
};

#endif
