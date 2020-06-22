/****************************************************************************
*
* This is a part of TOTEM offline software.
* Authors:
* 	Hubert Niewiadomski
*   Jan Kašpar (jan.kaspar@gmail.com)
*
****************************************************************************/

#include "RecoPPS/Local/interface/TotemRPLocalTrackFitterAlgorithm.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TMatrixD.h"

//----------------------------------------------------------------------------------------------------

using namespace std;
using namespace edm;

//----------------------------------------------------------------------------------------------------

TotemRPLocalTrackFitterAlgorithm::TotemRPLocalTrackFitterAlgorithm(const edm::ParameterSet &) {}

//----------------------------------------------------------------------------------------------------

void TotemRPLocalTrackFitterAlgorithm::reset() { det_data_map_.clear(); }

//----------------------------------------------------------------------------------------------------

TotemRPLocalTrackFitterAlgorithm::RPDetCoordinateAlgebraObjs
TotemRPLocalTrackFitterAlgorithm::prepareReconstAlgebraData(unsigned int det_id, const CTPPSGeometry &tot_rp_geom) {
  RPDetCoordinateAlgebraObjs det_algebra_obj;

  det_algebra_obj.centre_of_det_global_position_ = convert3vector(tot_rp_geom.sensorTranslation(det_id));

  TVector3 rd_dir = convert3vector(tot_rp_geom.localToGlobalDirection(det_id, rp_topology_.GetStripReadoutAxisDir()));

  TVector2 v(rd_dir.X(), rd_dir.Y());
  det_algebra_obj.readout_direction_ = v.Unit();
  det_algebra_obj.rec_u_0_ = 0.0;
  det_algebra_obj.available_ = true;
  det_algebra_obj.rec_u_0_ =
      -(det_algebra_obj.readout_direction_ * det_algebra_obj.centre_of_det_global_position_.XYvector());

  return det_algebra_obj;
}

//----------------------------------------------------------------------------------------------------

TotemRPLocalTrackFitterAlgorithm::RPDetCoordinateAlgebraObjs *TotemRPLocalTrackFitterAlgorithm::getDetAlgebraData(
    unsigned int det_id, const CTPPSGeometry &tot_rp_geom) {
  auto it = det_data_map_.find(det_id);
  if (it != det_data_map_.end()) {
    return &(it->second);
  } else {
    det_data_map_[det_id] = prepareReconstAlgebraData(det_id, tot_rp_geom);
    return &det_data_map_[det_id];
  }
}

//----------------------------------------------------------------------------------------------------

bool TotemRPLocalTrackFitterAlgorithm::fitTrack(const edm::DetSetVector<TotemRPRecHit> &hits,
                                                double z_0,
                                                const CTPPSGeometry &tot_geom,
                                                TotemRPLocalTrack &fitted_track) {
  fitted_track.setValid(false);

  // bind hits with their algebra objects
  struct HitWithAlg {
    unsigned int detId;
    const TotemRPRecHit *hit;
    const RPDetCoordinateAlgebraObjs *alg;
  };

  vector<HitWithAlg> applicable_hits;

  for (auto &ds : hits) {
    unsigned int detId = ds.detId();

    for (auto &h : ds) {
      const auto& alg = getDetAlgebraData(detId, tot_geom);
      if (alg->available_)
        applicable_hits.emplace_back(HitWithAlg{detId, &h, alg});
    }
  }

  if (applicable_hits.size() < 5)
    return false;

  TMatrixD horiz(applicable_hits.size(), 4);
  TVectorD vert(applicable_hits.size());
  TVectorD vert_inv(applicable_hits.size());
  TVectorD unit(applicable_hits.size());

  size_t id_hit = 0;
  for (const auto& hit : applicable_hits) {
    const auto& alg_obj = hit.alg;

    horiz(id_hit, 0) = alg_obj->readout_direction_.X();
    horiz(id_hit, 1) = alg_obj->readout_direction_.Y();
    double delta_z = alg_obj->centre_of_det_global_position_.Z() - z_0;
    horiz(id_hit, 2) = alg_obj->readout_direction_.X() * delta_z;
    horiz(id_hit, 3) = alg_obj->readout_direction_.Y() * delta_z;
    double var = hit.hit->sigma();
    var *= var;
    vert[id_hit] = var;
    vert_inv[id_hit] = 1.0 / var;
    unit[id_hit] = hit.hit->position() - alg_obj->rec_u_0_;
    ++id_hit;
  }

  TMatrixD h_t_v_inv(TMatrixD::kTransposed, horiz);
  multiplyByDiagonalInPlace(h_t_v_inv, vert_inv);
  TMatrixD v_a(h_t_v_inv);
  TMatrixD v_a_mult(v_a, TMatrixD::kMult, horiz);
  try {
    v_a_mult.Invert();
  } catch (cms::Exception &e) {
    LogError("TotemRPLocalTrackFitterAlgorithm") << "Error in TotemRPLocalTrackFitterAlgorithm::fitTrack > "
                                                 << "Fit matrix is singular. Skipping.";
    return false;
  }

  TMatrixD u_to_a(v_a_mult, TMatrixD::kMult, h_t_v_inv);
  TVectorD a(unit);
  a *= u_to_a;

  fitted_track.setZ0(z_0);
  fitted_track.setParameterVector(a);
  fitted_track.setCovarianceMatrix(v_a_mult);

  double chi_2 = 0;
  for (const auto& hit : applicable_hits) {
    const auto& alg_obj = hit.alg;
    TVector2 readout_dir = alg_obj->readout_direction_;
    double det_z = alg_obj->centre_of_det_global_position_.Z();
    double sigma_str = hit.hit->sigma();
    double sigma_str_2 = sigma_str * sigma_str;
    TVector2 fited_det_xy_point = fitted_track.trackPoint(det_z);
    double unit_readout = hit.hit->position() - alg_obj->rec_u_0_;
    double unit_fited = (readout_dir *= fited_det_xy_point);
    double residual = unit_fited - unit_readout;
    TMatrixD v_t_cov_x_y(1, 2);
    v_t_cov_x_y(0, 0) = readout_dir.X();
    v_t_cov_x_y(0, 1) = readout_dir.Y();
    TMatrixD v_t_cov_x_y_mult(v_t_cov_x_y, TMatrixD::kMult, fitted_track.trackPointInterpolationCovariance(det_z));
    double fit_strip_var = v_t_cov_x_y_mult(0, 0) * readout_dir.X() + v_t_cov_x_y_mult(0, 1) * readout_dir.Y();
    double pull_normalization = sqrt(sigma_str_2 - fit_strip_var);
    double pull = residual / pull_normalization;

    chi_2 += residual * residual / sigma_str_2;

    const TotemRPLocalTrack::FittedRecHit hit_point(
        *(hit.hit), TVector3(fited_det_xy_point.X(), fited_det_xy_point.Y(), det_z), residual, pull);
    fitted_track.addHit(hit.detId, hit_point);
  }

  fitted_track.setChiSquared(chi_2);
  fitted_track.setValid(true);
  return true;
}

//----------------------------------------------------------------------------------------------------

void TotemRPLocalTrackFitterAlgorithm::multiplyByDiagonalInPlace(TMatrixD &mt, const TVectorD &diag) {
  for (int i = 0; i < mt.GetNrows(); ++i) {
    for (int j = 0; j < mt.GetNcols(); ++j) {
      mt[i][j] *= diag[j];
    }
  }
}
