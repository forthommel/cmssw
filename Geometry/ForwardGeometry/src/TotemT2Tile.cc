/****************************************************************************
*
* This is a part of TOTEM offline software.
* Author:
*   Laurent Forthomme
*
****************************************************************************/

#include "Geometry/ForwardGeometry/interface/TotemT2Tile.h"
#include "DataFormats/Math/interface/AlgebraicROOTObjects.h"
#include <TGeoManager.h>

TotemT2Tile::TotemT2Tile() {}

TotemT2Tile::TotemT2Tile(const DetGeomDesc* dgd) {
  centre_ = GlobalPoint{(float)dgd->translation().x(),
                        (float)dgd->translation().y(),
                        (float)dgd->parentZPosition()};  // retrieve the plane position for z coordinate
  //const TGeoTrd1 tile(dgd->params()[4], dgd->params()[8], dgd->params()[3], dgd->params()[0]);
  //tile.InspectShape();
  //const dd4hep::Transform3D tile_transform(dgd->rotation(), dgd->translation());
  //vol_ = box.placeVolume(trap, tile_transform);
  double angle = 0.;
  {  // get azimutal component of tile rotation
    AlgebraicMatrix33 mat;
    dgd->rotation().GetRotationMatrix(mat);
    angle = acos(mat[0][0]);
  }
  TGeoCombiTrans place(centre_.x(), centre_.y(), centre_.z(), new TGeoRotation("tile_rot", angle, 0., 0.));
  TGeoManager mgr;
  auto* box = mgr.MakeBox("top", nullptr, 0., 0., 0.);
  mgr.SetTopVolume(box);
  auto* vol = mgr.MakeTrd1("tile", nullptr, dgd->params()[4], dgd->params()[8], dgd->params()[3], dgd->params()[0]);
  box->AddNode(vol, 1, &place);
  box->PrintNodes();
  vol->PrintVoxels();
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
