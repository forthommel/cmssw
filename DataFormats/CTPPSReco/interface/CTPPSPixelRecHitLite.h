/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef DataFormats_CTPPSReco_CTPPSPixelRecHitLite_h
#define DataFormats_CTPPSReco_CTPPSPixelRecHitLite_h

#include <cassert>

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/GeometrySurface/interface/LocalError.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "FWCore/Utilities/interface/isFinite.h"

namespace io_v1 {
  /// Lightweight version of reconstructed hits in CTPPS Pixel detector
  class CTPPSPixelRecHitLite {
  public:
    explicit CTPPSPixelRecHitLite(const Local2DPoint& position = Local2DPoint(0., 0.),
                                  const LocalError& uncertainty = LocalError(0., 0., 0.),
                                  int minrow = 0,
                                  int mincol = 0,
                                  bool edge = false,
                                  bool bad = false,
                                  bool rocs = false,
                                  unsigned char size = 0,
                                  unsigned char rowsize = 0,
                                  unsigned char colsize = 0)
        : position_(position),
          position_unc_(uncertainty),
          min_pixel_(minrow, mincol),
          flags_(edge | (bad << 1) | (rocs << 2)),
          cluster_size_(size),
          cluster_size_row_(rowsize),
          cluster_size_col_(colsize) {}
    /// Full-to-lite rechit conversion
    explicit CTPPSPixelRecHitLite(const CTPPSPixelRecHit& rechit)
        : CTPPSPixelRecHitLite({rechit.point().x(), rechit.point().y()},
                               rechit.error(),
                               rechit.minPixelRow(),
                               rechit.minPixelCol(),
                               rechit.isOnEdge(),
                               rechit.hasBadPixels(),
                               rechit.spanTwoRocs(),
                               rechit.clusterSize(),
                               rechit.clusterSizeRow(),
                               rechit.clusterSizeCol()) {
      setDetId(rechit.detId());
    }

    inline void setDetId(CTPPSPixelDetId detid) { detid_ = detid; }
    inline CTPPSPixelDetId detId() const { return detid_; }

    const Local2DPoint& point() const { return position_; }
    const LocalError& error() const { return position_unc_; }

    class PixelPosition {
    public:
      explicit PixelPosition(unsigned char pixel_row, unsigned char pixel_col)
          : pixel_id_(pixel_row * kNumCols + pixel_col) {}

      unsigned char row() const { return pixel_id_ / kNumCols; }
      unsigned char column() const { return pixel_id_ % kNumCols; }

    private:
      static constexpr unsigned char kNumCols = 3 * 52;
      unsigned short pixel_id_{0};
    };
    class StatusFlags {
    public:
      explicit StatusFlags(unsigned char flags) : flags_{flags} {}
      bool isOnEdge() const { return flags_ & 0x1; }
      bool hasBadPixels() const { return (flags_ >> 1) & 0x1; }
      bool spanTwoRocs() const { return (flags_ >> 2) & 0x1; }

    private:
      unsigned char flags_{0};
    };

    StatusFlags flags() const { return flags_; }
    PixelPosition minPixel() const { return min_pixel_; }

    unsigned char clusterSize() const { return cluster_size_; }
    unsigned char clusterSizeRow() const { return cluster_size_row_; }
    unsigned char clusterSizeCol() const { return cluster_size_col_; }

    float sort_key() const { return position_.mag2(); }

  private:
    CTPPSPixelDetId detid_{CTPPSPixelDetId(0, 0)};
    const Local2DPoint position_{0., 0.};
    const LocalError position_unc_{0., 0., 0.};

    const PixelPosition min_pixel_{0, 0};  // unsigned short
    const StatusFlags flags_{0};           // unsigned char

    const unsigned char cluster_size_{0};
    const unsigned char cluster_size_row_{0};
    const unsigned char cluster_size_col_{0};
  };

  inline bool operator<(const CTPPSPixelRecHitLite& lhs, const CTPPSPixelRecHitLite& rhs) {
    const auto lhs_key = lhs.sort_key(), rhs_key = rhs.sort_key();
    assert(edm::isFinite(lhs_key));
    assert(edm::isFinite(rhs_key));
    return lhs_key < rhs_key;
  }
}  // namespace io_v1
using CTPPSPixelRecHitLite = io_v1::CTPPSPixelRecHitLite;

#endif  // DataFormats_CTPPSReco_CTPPSPixelRecHitLite_h
