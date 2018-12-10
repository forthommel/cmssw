#ifndef DataFormats_CTPPSReco_CTPPSLocalTrackLiteFwd_h
#define DataFormats_CTPPSReco_CTPPSLocalTrackLiteFwd_h

#include <vector>
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/RefVector.h"

//--- start with a forward definition of the base object
class CTPPSLocalTrackLite;

/// collection of PPS local tracks
typedef std::vector<CTPPSLocalTrackLite> CTPPSLocalTrackLiteCollection;
/// persistent reference to a PPS local track
typedef edm::Ref<CTPPSLocalTrackLiteCollection> CTPPSLocalTrackLiteRef;
/// persistent reference to a PPS local tracks collection
typedef edm::RefProd<CTPPSLocalTrackLiteCollection> CTPPSLocalTrackLiteRefProd;
/// vector of reference to a PPS local track in the same collection
typedef edm::RefVector<CTPPSLocalTrackLiteCollection> CTPPSLocalTrackLiteRefVector;
/// iterator over a vector of reference to a PPS local track in the same collection
typedef CTPPSLocalTrackLiteRefVector::iterator CTPPSLocalTrackLite_iterator;

#endif

