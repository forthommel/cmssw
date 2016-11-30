/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Author:
 *   Laurent Forthomme (laurent.forthomme@cern.ch)
 *
 ****************************************************************************/

#ifndef Geometry_VeryForwardGeometry_CTPPSDiamondGeometry_h
#define Geometry_VeryForwardGeometry_CTPPSDiamondGeometry_h

#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"

#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondPlane.h"
#include "Geometry/VeryForwardGeometry/interface/CTPPSDiamondChannel.h"

#include "DataFormats/DetId/interface/DetId.h"

#include "Geometry/VeryForwardGeometryBuilder/interface/DetGeomDesc.h" //FIXME temp

//----------------------------------------------------------------------------------------------------

class GeomDetType;

class CTPPSDiamondGeometry : public TrackingGeometry
{
  public:
    CTPPSDiamondGeometry();
    CTPPSDiamondGeometry( const DetGeomDesc* ) {}
    virtual ~CTPPSDiamondGeometry();

    //------------------------------------------------------------------------------------------------

    const DetTypeContainer& detTypes() const override { return channels_types_; }
    const DetUnitContainer& detUnits() const override { return channels_; }
    const DetContainer& dets() const override { return dets_; }
    const DetIdContainer& detUnitIds() const override { return channels_ids_; }
    const DetIdContainer& detIds() const override { return dets_ids_; }
    const GeomDetUnit* idToDetUnit( DetId ) const override;
    const GeomDet* idToDet( DetId ) const override;

    //------------------------------------------------------------------------------------------------

    void add( CTPPSDiamondPlane* );
    const std::vector<const CTPPSDiamondPlane*>& planes() const { return all_planes_; }
    const CTPPSDiamondPlane* plane( const CTPPSDiamondDetId& ) const;

    //------------------------------------------------------------------------------------------------

    void add( CTPPSDiamondChannel* );
    const std::vector<const CTPPSDiamondChannel*>& channels() const { return all_channels_; }
    const CTPPSDiamondChannel* channel( const CTPPSDiamondDetId& ) const;

  private:
    DetUnitContainer channels_;
    DetTypeContainer channels_types_;
    DetContainer dets_;
    DetIdContainer channels_ids_, dets_ids_;

    mapIdToDet mapping_;

    std::vector<const CTPPSDiamondPlane*> all_planes_; // owned
    std::vector<const CTPPSDiamondChannel*> all_channels_; // NOT owned (channels belong to their plane)
};

#endif
