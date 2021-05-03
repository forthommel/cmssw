#ifndef Geometry_ForwardGeometry_TotemT2Tile_h
#define Geometry_ForwardGeometry_TotemT2Tile_h

#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/DetGeomDesc.h"

class TotemT2Tile : public CaloCellGeometry {
public:
  TotemT2Tile();
  explicit TotemT2Tile(const GlobalPoint&, CornersMgr*, const DetGeomDesc*);
  ~TotemT2Tile() override;

  static CCGFloat* computeParamsFromDetGeomDesc(const DetGeomDesc*);

  enum Parameters { P_RAD = 0, P_LEN_MIN, P_LEN_MAX, P_THICK, P_ANG, P_NUM_PARAMS };

private:
  void initCorners(CornersVec&) override;
  void vocalCorners(CaloCellGeometry::Pt3DVec&, const CCGFloat*, CaloCellGeometry::Pt3D&) const override {}
  CCGFloat* dgd_;
};

#endif
