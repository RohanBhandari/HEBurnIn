#include <iostream>
#include "HEBurnIn/ntupler/interface/HcalTupleMaker_QIE11Digis.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"

// NEEDS UPDATING
double adc2fC_QIE11[256]={
  // - - - - - - - range 0 - - - - - - - -
  //subrange0
  1.58, 4.73, 7.88, 11.0, 14.2, 17.3, 20.5, 23.6,
  26.8, 29.9, 33.1, 36.2, 39.4, 42.5, 45.7, 48.8,
  //subrange1
  53.6, 60.1, 66.6, 73.0, 79.5, 86.0, 92.5, 98.9,
  105, 112, 118, 125, 131, 138, 144, 151,
  //subrange2
  157, 164, 170, 177, 186, 199, 212, 225,
  238, 251, 264, 277, 289, 302, 315, 328,
  //subrange3
  341, 354, 367, 380, 393, 406, 418, 431,
  444, 464, 490, 516, 542, 568, 594, 620,

  // - - - - - - - range 1 - - - - - - - -
  //subrange0
  569, 594, 619, 645, 670, 695, 720, 745,
  771, 796, 821, 846, 871, 897, 922, 947,
  //subrange1
  960, 1010, 1060, 1120, 1170, 1220, 1270, 1320,
  1370, 1430, 1480, 1530, 1580, 1630, 1690, 1740,
  //subrange2
  1790, 1840, 1890, 1940,  2020, 2120, 2230, 2330,
  2430, 2540, 2640, 2740, 2850, 2950, 3050, 3150,
  //subrange3
  3260, 3360, 3460, 3570, 3670, 3770, 3880, 3980,
  4080, 4240, 4450, 4650, 4860, 5070, 5280, 5490,

  // - - - - - - - range 2 - - - - - - - -
  //subrange0
  5080, 5280, 5480, 5680, 5880, 6080, 6280, 6480,
  6680, 6890, 7090, 7290, 7490, 7690, 7890, 8090,
  //subrange1
  8400, 8810, 9220, 9630, 10000, 10400, 10900, 11300,
  11700, 12100, 12500, 12900, 13300, 13700, 14100, 14500,
  //subrange2
  15000, 15400, 15800, 16200, 16800, 17600, 18400, 19300,
  20100, 20900, 21700, 22500, 23400, 24200, 25000, 25800,
  //subrange3
  26600, 27500, 28300, 29100, 29900, 30700, 31600, 32400,
  33200, 34400, 36100, 37700, 39400, 41000, 42700, 44300,

  // - - - - - - - range 3 - - - - - - - - -
  //subrange0
  41100, 42700, 44300, 45900, 47600, 49200, 50800, 52500,
  54100, 55700, 57400, 59000, 60600, 62200, 63900, 65500,
  //subrange1
  68000, 71300, 74700, 78000, 81400, 84700, 88000, 91400,
  94700, 98100, 101000, 105000, 108000, 111000, 115000, 118000,
  //subrange2
  121000, 125000, 128000, 131000, 137000, 145000, 152000, 160000,
  168000, 176000, 183000, 191000, 199000, 206000, 214000, 222000,
  //subrange3
  230000, 237000, 245000, 253000, 261000, 268000, 276000, 284000,
  291000, 302000, 316000, 329000, 343000, 356000, 370000, 384000

};

HcalTupleMaker_QIE11Digis::HcalTupleMaker_QIE11Digis(const edm::ParameterSet& iConfig):
  prefix          (iConfig.getUntrackedParameter<std::string>("Prefix")),
  suffix          (iConfig.getUntrackedParameter<std::string>("Suffix")),
  m_qie11DigisTag (iConfig.getUntrackedParameter<edm::InputTag>("tagQIE11", edm::InputTag("hcalDigis")))
{ 

  qie11digisToken_ = consumes<HcalDataFrameContainer<QIE11DataFrame> >(m_qie11DigisTag);
//consumes<QIE10DigiCollection>(m_qie10DigisTag);

    
  produces<std::vector<int>   >                  ( "rm"      );
  produces<std::vector<int>   >                  ( "fiber"   );
  produces<std::vector<int>   >                  ( "channel" );
  produces<std::vector<int>   >                  ( "linkErr" );
  produces<std::vector<std::vector<int>   > >    ( "soi"     );
  produces<std::vector<std::vector<int>   > >    ( "digiADC" );
  produces<std::vector<std::vector<double>   > > ( "digiFC"  );
  produces<std::vector<std::vector<int>   > >    ( "capID"   );
}

void HcalTupleMaker_QIE11Digis::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<int> >                    _rm      ( new std::vector<int>   ());
  std::auto_ptr<std::vector<int> >                    _fiber   ( new std::vector<int>   ());
  std::auto_ptr<std::vector<int> >                    _channel ( new std::vector<int>   ());
  std::auto_ptr<std::vector<int> >                    _linkEr  ( new std::vector<int>   ());
  std::auto_ptr<std::vector<std::vector<int  > > >    _soi     ( new std::vector<std::vector<int  > >    ());
  std::auto_ptr<std::vector<std::vector<int  > > >    _adc     ( new std::vector<std::vector<int  > >    ());
  std::auto_ptr<std::vector<std::vector<double  > > > _fc      ( new std::vector<std::vector<double  > > ());
  std::auto_ptr<std::vector<std::vector<int  > > >    _capid   ( new std::vector<std::vector<int  > >    ());
    
  //
  edm::Handle<HcalDataFrameContainer<QIE11DataFrame> >  qie11Digis;
  iEvent.getByToken(qie11digisToken_, qie11Digis);
    
  //
  for (uint32_t i=0; i<qie11Digis->size(); i++){

    // From: https://github.com/awhitbeck/HFcommissioningAnalysis/blob/b3456c9fe66ef9bcc6c54773d60f768c269a5c74/src/HFanalyzer.cc#L429
    QIE11DataFrame qie11df = (*qie11Digis)[i];
    //QIE10 structure: static_cast<QIE11DataFrame>((*qie11Digis)[i]);

    //Extract info on detector location
    DetId detid = qie11df.detid();

    HcalCalibDetId hcalcalib = HcalCalibDetId(detid);

    _rm      -> push_back ( hcalcalib.rm()      );
    _fiber   -> push_back ( hcalcalib.fiber()   );
    _channel -> push_back ( hcalcalib.channel() );
    _linkEr  -> push_back ( qie11df.linkError() );
    
    if (0){
      std::cout << "Printing raw dataframe" << std::endl;
      std::cout << qie11df << std::endl;

      std::cout << "Printing content of samples() method" << std::endl;
      std::cout << qie11df.samples() << std::endl;
    }

    _soi             -> push_back ( std::vector<int  >   () ) ;
    _adc             -> push_back ( std::vector<int  >   () ) ;
    _fc              -> push_back ( std::vector<double  >() ) ;
    _capid           -> push_back ( std::vector<int  >   () ) ;
    size_t last_entry = _adc -> size() - 1;

    // TS
    int nTS = qie11df.samples();

    for(int its=0; its<nTS; ++its){ 
      //int adc = qie10df[its].adc();
      //int capid = qie10df[its].capid();

      (*_soi      )[last_entry].push_back ( qie11df[its].soi()               ); // soi is a bool, but stored as an int
      (*_adc      )[last_entry].push_back ( qie11df[its].adc()               );
      (*_fc       )[last_entry].push_back ( adc2fC_QIE11[qie11df[its].adc()] );
      (*_capid    )[last_entry].push_back ( qie11df[its].capid()             );
	  
    }

  }

  //  
  iEvent.put( _rm      , "rm"      ); 
  iEvent.put( _fiber   , "fiber"   ); 
  iEvent.put( _channel , "channel" ); 
  iEvent.put( _linkEr  , "linkErr" );
  iEvent.put( _soi     , "soi"     );
  iEvent.put( _adc     , "digiADC" );
  iEvent.put( _fc      , "digiFC"  );
  iEvent.put( _capid   , "capID"   ); 
}
