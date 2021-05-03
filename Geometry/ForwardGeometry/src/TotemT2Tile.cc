/****************************************************************************
*
* This is a part of TOTEM offline software.
* Author:
*   Laurent Forthomme
*
****************************************************************************/

#include "Geometry/ForwardGeometry/interface/TotemT2Tile.h"
#include "DataFormats/Math/interface/AlgebraicROOTObjects.h"

TotemT2Tile::TotemT2Tile() : CaloCellGeometry() {}

TotemT2Tile::TotemT2Tile(const GlobalPoint& centre, CornersMgr* mgr, const DetGeomDesc* dgd)
    : CaloCellGeometry(centre, mgr, dgd_ = computeParamsFromDetGeomDesc(dgd)) {
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  for (const auto& param : dgd->params())
    std::cout << ">>> " << param << std::endl;
}

TotemT2Tile::~TotemT2Tile() {
  if (dgd_)
    delete dgd_;
}

CaloCellGeometry::CCGFloat* TotemT2Tile::computeParamsFromDetGeomDesc(const DetGeomDesc* dgd) {
  auto* params = new CCGFloat[P_NUM_PARAMS];
  params[P_RAD] = dgd->params()[0];
  params[P_LEN_MIN] = dgd->params()[4];
  params[P_LEN_MAX] = dgd->params()[8];
  params[P_THICK] = dgd->params()[3];
  AlgebraicMatrix33 mat;
  dgd->rotation().GetRotationMatrix(mat);
  params[P_ANG] = acos(mat[0][0]);
  return params;
}

void TotemT2Tile::initCorners(CornersVec& corners) {
  //if (!corners.uninitialized())
  //  return;
  const auto* params = param();
  std::cout << ">>> " << params[P_ANG] << std::endl;
}
