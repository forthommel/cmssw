/****************************************************************************
*
* This is a part of TOTEM offline software.
* Author:
*   Laurent Forthomme
*
****************************************************************************/

#include "Geometry/ForwardGeometry/interface/TotemT2Tile.h"
#include "DataFormats/Math/interface/AlgebraicROOTObjects.h"

TotemT2Tile::TotemT2Tile() {}

TotemT2Tile::TotemT2Tile(const DetGeomDesc* dgd) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  for (const auto& param : dgd->params())
    std::cout << ">>> " << param << std::endl;
  centre_ = GlobalPoint{(float)dgd->translation().x(), (float)dgd->translation().y(), (float)dgd->parentZPosition()};  // retrieve the plane position for z coordinate
  const dd4hep::Volume box;//(0, 0, 0);
  const dd4hep::Trapezoid trap(dgd->params()[4], dgd->params()[8], dgd->params()[3], dgd->params()[3], dgd->params()[0]);
  const dd4hep::Transform3D tile_transform(dgd->rotation(), dgd->translation());
  //vol_ = box.placeVolume(trap, tile_transform);
}

TotemT2Tile::~TotemT2Tile() {}

/*CaloCellGeometry::CCGFloat* TotemT2Tile::computeParamsFromDetGeomDesc(const DetGeomDesc* dgd) {
  auto* params = new CCGFloat[P_NUM_PARAMS];
  params[P_RAD] = dgd->params()[0];
  params[P_LEN_MIN] = dgd->params()[4];
  params[P_LEN_MAX] = dgd->params()[8];
  params[P_THICK] = dgd->params()[3];
  { // get azimutal component of tile rotation
    AlgebraicMatrix33 mat;
    dgd->rotation().GetRotationMatrix(mat);
    params[P_ANG] = acos(mat[0][0]);
  }
  return params;
}*/
