#include "EventFilter/DateRawToDigi/interface/AliRawReader.h"
#include "EventFilter/DateRawToDigi/interface/AliRawReaderDate.h"

#include "EventFilter/DateRawToDigi/src/SRSAPVEvent.h"
#include "EventFilter/DateRawToDigi/src/SRSCluster.h"
#include "EventFilter/DateRawToDigi/src/SRSRawPedestal.h"
#include "EventFilter/DateRawToDigi/src/SRSPedestal.h"
#include "EventFilter/DateRawToDigi/src/SRSHit.h"
#include "EventFilter/DateRawToDigi/src/SRSTrack.h"

#include "EventFilter/DateRawToDigi/src/SRSPositionCorrection.h"
#include "EventFilter/DateRawToDigi/src/SRSTrackFit.h"
#include "EventFilter/DateRawToDigi/src/SRSAPVSignalFit.h"
#include "EventFilter/DateRawToDigi/src/SRSDoubleGaussianFit.h"
#include "EventFilter/DateRawToDigi/src/SRSConfiguration.h"
#include "EventFilter/DateRawToDigi/src/SRSAlignment.h"
#include "EventFilter/DateRawToDigi/src/SRSEventBuilder.h"
#include "EventFilter/DateRawToDigi/src/SRSFECPedestalDecoder.h"
#include "EventFilter/DateRawToDigi/src/SRSFECEventDecoder.h"
#include "EventFilter/DateRawToDigi/src/SRSOutputROOT.h"
#include "EventFilter/DateRawToDigi/src/SRSHistoManager.h"
#include "EventFilter/DateRawToDigi/src/SRSMapping.h"

#include "Rtypes.h"
#include <map>
#include <vector>

namespace EventFilter_DateRawToDigi
{
  struct dictionary
  {
    //AliRawReader a_rr;
    AliRawReaderDate a_rrd;
    SRSAPVEvent s_ae;
    SRSCluster s_clus;
    SRSRawPedestal s_rped;
    SRSPedestal s_ped;
    SRSHit s_hit;
    SRSTrack s_trk;
    SRSPositionCorrection s_pc;
    SRSTrackFit s_trkf;
    SRSAPVSignalFit s_asf;
    SRSDoubleGaussianFit s_dgf;
    SRSConfiguration s_cfg;
    SRSAlignment s_aln;
    SRSEventBuilder s_eb;
    SRSFECPedestalDecoder s_fpd;
    SRSFECEventDecoder s_fed;
    SRSOutputROOT s_or;
    SRSHistoManager s_hm;
    SRSMapping s_map;
    std::map<TString,std::vector<int> > m_s_vi;
    std::map<TString,std::vector<float> > m_s_vf;
  };
}
