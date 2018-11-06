/****************************************************************************
 *
 * This is a part of CTPPS offline software.
 * Authors:
 *   Jan Kašpar
 *   Laurent Forthomme
 *
 ****************************************************************************/

#include "RecoCTPPS/ProtonReconstruction/interface/ProtonReconstructionAlgorithm.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSDetId.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"

#include "TF1.h"

using namespace std;
using namespace edm;

//#define DEBUG 1

//----------------------------------------------------------------------------------------------------

ProtonReconstructionAlgorithm::ProtonReconstructionAlgorithm(const std::string &optics_file_beam1, const std::string &optics_file_beam2,
    const edm::ParameterSet &beam_conditions, const std::vector<edm::ParameterSet> &detector_packages, bool _fit_vtx_y, unsigned int _verbosity) :

  verbosity(_verbosity),
  beamConditions_(beam_conditions),
  halfCrossingAngleSector45_(beamConditions_.getParameter<double>("halfCrossingAngleSector45" )),
  halfCrossingAngleSector56_(beamConditions_.getParameter<double>("halfCrossingAngleSector56" )),
  yOffsetSector45_(beamConditions_.getParameter<double>("yOffsetSector45")),
  yOffsetSector56_(beamConditions_.getParameter<double>("yOffsetSector56")),
  fitVtxY_(_fit_vtx_y),

  fitter_(std::make_unique<ROOT::Fit::Fitter>()),
  chiSquareCalculator_(std::make_unique<ChiSquareCalculator>(beamConditions_))
{
  // open files with optics
  TFile *f_in_optics_beam1 = TFile::Open(optics_file_beam1.c_str());
  if (f_in_optics_beam1 == NULL)
    throw cms::Exception("ProtonReconstructionAlgorithm") << "Can't open file '" << optics_file_beam1 << "'.";

  TFile *f_in_optics_beam2 = TFile::Open(optics_file_beam2.c_str());
  if (f_in_optics_beam2 == NULL)
    throw cms::Exception("ProtonReconstructionAlgorithm") << "Can't open file '" << optics_file_beam2 << "'.";

  // build RP id, optics object name association
  std::map<unsigned int, std::string> idNameMap;
  for (const auto &p : detector_packages)
  {
    const unsigned int &rpId = p.getParameter<unsigned int>("potId");
    const string &approximatorName = p.getParameter<string>("interpolatorName");
    idNameMap.emplace(rpId, approximatorName);
  }

#ifdef DEBUG
  TFile *f_debug = new TFile("debug.root", "recreate");
#endif

  TF1 *ff = new TF1("ff", "[0] + [1]*x");

  // build optics data for each object
  for (const auto &it : idNameMap)
  {
    const CTPPSDetId rpId(it.first);
    const std::string &ofName = it.second;

    // load optics approximation
    TFile *f_in_optics = (rpId.arm() == 1) ? f_in_optics_beam1 : f_in_optics_beam2;
    LHCOpticsApproximator *of_orig = (LHCOpticsApproximator *) f_in_optics->Get(ofName.c_str());
    if (of_orig == NULL)
      throw cms::Exception("ProtonReconstructionAlgorithm") << "Can't load object '" << ofName << "'.";

    // copy approximator object not to loose it when the input ROOT file is closed
    RPOpticsData rpod;
    rpod.optics = make_shared<LHCOpticsApproximator>(LHCOpticsApproximator(* of_orig));

    // build auxiliary optical functions
    double crossing_angle = 0.;
    double vtx0_y = 0.;

    if (rpId.arm() == 0)
    {
      crossing_angle = halfCrossingAngleSector45_;
      vtx0_y = yOffsetSector45_;
    } else {
      crossing_angle = halfCrossingAngleSector56_;
      vtx0_y = yOffsetSector56_;
    }

    const bool check_apertures = false;
    const bool invert_beam_coord_systems = true;

    TGraph *g_xi_vs_x = new TGraph();
    TGraph *g_x0_vs_xi = new TGraph();
    TGraph *g_L_x_vs_xi = new TGraph();

    TGraph *g_y0_vs_xi = new TGraph();
    TGraph *g_v_y_vs_xi = new TGraph();
    TGraph *g_L_y_vs_xi = new TGraph();

    // input: all zero
    double kin_in_zero[5] = { 0., crossing_angle, vtx0_y, 0., 0. };
    double kin_out_zero[5] = { 0., 0., 0., 0., 0. };
    rpod.optics->Transport(kin_in_zero, kin_out_zero, check_apertures, invert_beam_coord_systems);

    rpod.x0 = kin_out_zero[0];
    rpod.y0 = kin_out_zero[2];

    for (double xi = 0.; xi <= 0.251; xi += 0.005)
    {
      // input: only xi
      double kin_in_xi[5] = { 0., crossing_angle * (1. - xi), vtx0_y, 0., -xi };
      double kin_out_xi[5];
        rpod.optics->Transport(kin_in_xi, kin_out_xi, check_apertures, invert_beam_coord_systems);

      // input: xi and th_x
      const double th_x = 20E-6;  // rad
      double kin_in_xi_th_x[5] = { 0., (crossing_angle + th_x) * (1. - xi), vtx0_y, 0., -xi };
      double kin_out_xi_th_x[5];
        rpod.optics->Transport(kin_in_xi_th_x, kin_out_xi_th_x, check_apertures, invert_beam_coord_systems);

      // input: xi and vtx_y
      const double vtx_y = 10E-6;  // m
      double kin_in_xi_vtx_y[5] = { 0., crossing_angle * (1. - xi), vtx0_y + vtx_y, 0., -xi };
      double kin_out_xi_vtx_y[5];
        rpod.optics->Transport(kin_in_xi_vtx_y, kin_out_xi_vtx_y, check_apertures, invert_beam_coord_systems);

      // input: xi and th_y
      const double th_y = 20E-6;  // rad
      double kin_in_xi_th_y[5] = { 0., crossing_angle * (1. - xi), vtx0_y, th_y * (1. - xi), -xi };
      double kin_out_xi_th_y[5];
        rpod.optics->Transport(kin_in_xi_th_y, kin_out_xi_th_y, check_apertures, invert_beam_coord_systems);

      // fill graphs
      int idx = g_xi_vs_x->GetN();
      g_xi_vs_x->SetPoint(idx, kin_out_xi[0] - kin_out_zero[0], xi);
      g_x0_vs_xi->SetPoint(idx, xi, kin_out_xi[0] - kin_out_zero[0]);
      g_L_x_vs_xi->SetPoint(idx, xi, (kin_out_xi_th_x[0] - kin_out_xi[0]) / th_x);

      g_y0_vs_xi->SetPoint(idx, xi, kin_out_xi[2] - kin_out_zero[2]);
      g_v_y_vs_xi->SetPoint(idx, xi, (kin_out_xi_vtx_y[2] - kin_out_xi[2]) / vtx_y);
      g_L_y_vs_xi->SetPoint(idx, xi, (kin_out_xi_th_y[2] - kin_out_xi[2]) / th_y);
    }

#ifdef DEBUG
    char buf[100];
    unsigned int decRPId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();
    sprintf(buf, "%u", decRPId);
    gDirectory = f_debug->mkdir(buf);
    g_xi_vs_x->Write("g_xi_vs_x");
    g_y0_vs_xi->Write("g_y0_vs_xi");
    g_v_y_vs_xi->Write("g_v_y_vs_xi");
    g_L_y_vs_xi->Write("g_L_y_vs_xi");
#endif

    // make splines
    rpod.s_xi_vs_x = make_shared<TSpline3>("", g_xi_vs_x->GetX(), g_xi_vs_x->GetY(), g_xi_vs_x->GetN());
    rpod.s_y0_vs_xi = make_shared<TSpline3>("", g_y0_vs_xi->GetX(), g_y0_vs_xi->GetY(), g_y0_vs_xi->GetN());
    rpod.s_v_y_vs_xi = make_shared<TSpline3>("", g_v_y_vs_xi->GetX(), g_v_y_vs_xi->GetY(), g_v_y_vs_xi->GetN());
    rpod.s_L_y_vs_xi = make_shared<TSpline3>("", g_L_y_vs_xi->GetX(), g_L_y_vs_xi->GetY(), g_L_y_vs_xi->GetN());

    // get linear approximation
    ff->SetParameters(0., 0.);
    g_x0_vs_xi->Fit(ff, "Q");
    g_x0_vs_xi->Fit(ff, "Q");
    g_x0_vs_xi->Fit(ff, "Q");
    rpod.ch0 = ff->GetParameter(0);
    rpod.ch1 = ff->GetParameter(1);

    ff->SetParameters(0., 0.);
    g_L_x_vs_xi->Fit(ff, "Q");
    g_L_x_vs_xi->Fit(ff, "Q");
    g_L_x_vs_xi->Fit(ff, "Q");
    rpod.la0 = ff->GetParameter(0);
    rpod.la1 = ff->GetParameter(1);

    //printf("ch0 = %.3f, ch1 = %.3f, la0 = %.3f, la1 = %.3f\n", rpod.ch0, rpod.ch1, rpod.la0, rpod.la1);

    // free memory
    delete g_xi_vs_x;
    delete g_x0_vs_xi;
    delete g_L_x_vs_xi;

    delete g_y0_vs_xi;
    delete g_v_y_vs_xi;
    delete g_L_y_vs_xi;

    // insert optics data
    m_rp_optics_[rpId] = rpod;
  }

  // initialise fitter
  double pStart[] = { 0, 0, 0, 0 };
  fitter_->SetFCN( 4, *chiSquareCalculator_, pStart, 0, true );

  // clean up
  delete ff;

  // TODO: debug only
#ifdef DEBUG
  delete f_debug;
#endif
}

//----------------------------------------------------------------------------------------------------

ProtonReconstructionAlgorithm::~ProtonReconstructionAlgorithm()
{
}

//----------------------------------------------------------------------------------------------------

double ProtonReconstructionAlgorithm::ChiSquareCalculator::operator() (const double* parameters) const
{
  // extract proton parameters
  const double& xi = parameters[0];
  const double& th_x = parameters[1];
  const double& th_y = parameters[2];
  const double vtx_x = 0;
  const double& vtx_y = parameters[3];

  // calculate chi^2 by looping over hits
  double S2 = 0.0;

  for (const auto &track : *tracks)
  {
    const CTPPSDetId rpId(track->getRPId());

    // determine beam beam parameters
    double crossing_angle = 0., vtx0_y = 0.;

    if (rpId.arm() == 0)
    {
      crossing_angle = halfCrossingAngleSector45_;
      vtx0_y = yOffsetSector45_;
    } else {
      crossing_angle = halfCrossingAngleSector56_;
      vtx0_y = yOffsetSector56_;
    }

    const bool check_apertures = false;
    const bool invert_beam_coord_systems = true;

    // transport proton to the RP
    auto oit = m_rp_optics->find(rpId);
    double kin_in[5] = { vtx_x, (th_x + crossing_angle) * (1. - xi), vtx0_y + vtx_y, th_y * (1. - xi), -xi };
    double kin_out[5];
    oit->second.optics->Transport(kin_in, kin_out, check_apertures, invert_beam_coord_systems);

    // proton position wrt. beam
    const double& x = kin_out[0] - oit->second.x0;
    const double& y = kin_out[2] - oit->second.y0;

    // calculate chi^2 contributions, convert track data mm --> m
    double x_unc = track->getXUnc();
    if (x_unc < 1E-3)
      x_unc = 1E-3;

    double y_unc = track->getYUnc();
    if (y_unc < 1E-3)
      y_unc = 1E-3;

    const double x_diff_norm = (x - track->getX()*1E-3) / (x_unc*1E-3);
    const double y_diff_norm = (y - track->getY()*1E-3) / (y_unc*1E-3);

    // increase chi^2
    S2 += x_diff_norm*x_diff_norm + y_diff_norm*y_diff_norm;
  }

  /*
  edm::LogInfo("ChiSquareCalculator")
    << "xi = " << xi << ", "
    << "th_x = " << th_x << ", "
    << "th_y = " << th_y << ", "
    << "vtx_y = " << vtx_y << " | S2 = " << S2 << "\n";
  */

  return S2;
}

//----------------------------------------------------------------------------------------------------

void ProtonReconstructionAlgorithm::reconstructFromMultiRP(const vector<const CTPPSLocalTrackLite*> &tracks, vector<reco::ProtonTrack> &out) const
{
  // need at least two tracks
  if (tracks.size() < 2)
    return;

  // make sure optics is available for all tracks
  for (const auto &it : tracks)
  {
    auto oit = m_rp_optics_.find(it->getRPId());
    if (oit == m_rp_optics_.end())
      throw cms::Exception("") << "Optics data not available for RP " << it->getRPId() << ".";
  }

  // initial estimate of xi and th_x
  double xi_init = 0., th_x_init = 0.;

  const bool use_improved_estimate = true;

  if (use_improved_estimate)
  {
    double x_N = 0., x_F = 0.;
    const RPOpticsData *i_N = NULL, *i_F = NULL;
    unsigned int idx = 0;
    for (const auto &track : tracks)
    {
      auto oit = m_rp_optics_.find(track->getRPId());

      if (idx == 0) { x_N = track->getX() * 1E-3; i_N = &oit->second; }
      if (idx == 1) { x_F = track->getX() * 1E-3; i_F = &oit->second; }
      if (idx == 2) break;

      idx++;
    }

    const double a = i_F->ch1*i_N->la1 - i_N->ch1*i_F->la1;
    const double b = i_F->ch0*i_N->la1 - i_N->ch0*i_F->la1 + i_F->ch1*i_N->la0 - i_N->ch1*i_F->la0 + x_N*i_F->la1 - x_F*i_N->la1;
    const double c = x_N*i_F->la0 - x_F*i_N->la0 + i_F->ch0*i_N->la0 - i_N->ch0*i_F->la0;
    const double D = b*b - 4.*a*c;

    xi_init = (-b + sqrt(D)) / 2. / a;
    th_x_init = (x_N - i_N->ch0 - i_N->ch1 * xi_init) / (i_N->la0 + i_N->la1 * xi_init);
  } else {
    double S_xi0 = 0., S_1 = 0.;
    for (const auto &track : tracks)
    {
      auto oit = m_rp_optics_.find(track->getRPId());
      double xi = oit->second.s_xi_vs_x->Eval(track->getX() * 1E-3);  // mm --> m

      S_1 += 1.;
      S_xi0 += xi;
    }

    xi_init = S_xi0 / S_1;
  }

  // initial estimate of th_y and vtx_y
  double y[2], v_y[2], L_y[2];
  unsigned int y_idx = 0;
  for (const auto &track : tracks)
  {
    if (y_idx >= 2)
      continue;

    auto oit = m_rp_optics_.find(track->getRPId());

    y[y_idx] = track->getY()*1E-3 - oit->second.s_y0_vs_xi->Eval(xi_init); // track y: mm --> m
    v_y[y_idx] = oit->second.s_v_y_vs_xi->Eval(xi_init);
    L_y[y_idx] = oit->second.s_L_y_vs_xi->Eval(xi_init);

    y_idx++;
  }

  double vtx_y_init = 0.;
  double th_y_init = 0.;

  if (fitVtxY_)
  {
    const double det_y = v_y[0] * L_y[1] - L_y[0] * v_y[1];
    vtx_y_init = (L_y[1] * y[0] - L_y[0] * y[1]) / det_y;
    th_y_init = (v_y[0] * y[1] - v_y[1] * y[0]) / det_y;
  } else {
    vtx_y_init = 0.;
    th_y_init = (y[1]/L_y[1] + y[0]/L_y[0]) / 2.;
  }

  if (verbosity)
  {
    unsigned int armId = CTPPSDetId((*tracks.begin())->getRPId()).arm();
    printf("* ProtonReconstructionAlgorithm::reconstructFromMultiRP(%u)\n", armId);
    printf("    initial estimate: xi_init = %f, th_x_init = %E, th_y_init = %E, vtx_y_init = %E\n", xi_init, th_x_init, th_y_init, vtx_y_init);
  }

  // minimisation
  fitter_->Config().ParSettings(0).Set("xi", xi_init, 0.005);
  fitter_->Config().ParSettings(1).Set("th_x", th_x_init, 2E-6);
  fitter_->Config().ParSettings(2).Set("th_y", th_y_init, 2E-6);
  fitter_->Config().ParSettings(3).Set("vtx_y", vtx_y_init, 10E-6);

  if (!fitVtxY_)
    fitter_->Config().ParSettings(3).Fix();

  chiSquareCalculator_->tracks = &tracks;
  chiSquareCalculator_->m_rp_optics = &m_rp_optics_;

  fitter_->FitFCN();

  // extract proton parameters
  const ROOT::Fit::FitResult& result = fitter_->Result();
  const double *params = result.GetParams();

  if (verbosity)
    edm::LogInfo("ProtonReconstructionAlgorithm")
      << "at reconstructed level: "
      << "xi=" << params[0] << ", "
      << "theta_x=" << params[1] << ", "
      << "theta_y=" << params[2] << ", "
      << "vertex_y=" << params[3] << "\n";

  if (verbosity)
    printf("    fit: xi = %f, th_x = %E, th_y = %E, vtx_y = %E, chiSq = %.0f\n", params[0], params[1], params[2], params[3], result.Chi2());

  reco::ProtonTrack pt;
  pt.method = reco::ProtonTrack::rmMultiRP;
  pt.halfCrossingAngleSector45 = halfCrossingAngleSector45_;
  pt.halfCrossingAngleSector56 = halfCrossingAngleSector56_;

  pt.fitChiSq = result.Chi2();
  pt.fitNDF = 2.*tracks.size() - 4;
  pt.lhcSector = (CTPPSDetId(tracks[0]->getRPId()).arm() == 0) ? reco::ProtonTrack::sector45 : reco::ProtonTrack::sector56;

  pt.setVertex(Local3DPoint(0., params[3]*1E3, 0.));  // vertext in mm

  const double p_nom = 6500.;  // GeV
  const double p = p_nom *  (1. - params[0]);
  const double th_x = params[1];
  const double th_y = params[2];
  const double cos_th = sqrt(1. - th_x*th_x - th_y*th_y);
  const double sign_z_lhc = (pt.lhcSector == reco::ProtonTrack::sector45) ? -1. : +1.;

  pt.setXi(params[0]);

  pt.setDirection(Local3DVector(
    - p * th_x,   // the signs reflect change LHC --> CMS convention
    + p * th_y,
    - sign_z_lhc * p * cos_th
  ));

  for (const auto &track : tracks)
    pt.contributingRPIds.insert(track->getRPId());

  const double max_chi_sq = 1. + 3. * pt.fitNDF;

  pt.setValid(result.IsValid() && pt.fitChiSq < max_chi_sq);

  out.push_back(move(pt));
}

//----------------------------------------------------------------------------------------------------

void ProtonReconstructionAlgorithm::reconstructFromSingleRP(const vector<const CTPPSLocalTrackLite*> &tracks,
  vector<reco::ProtonTrack> &out) const
{
  // make sure optics is available for all tracks
  for (const auto &it : tracks)
  {
    auto oit = m_rp_optics_.find(it->getRPId());
    if (oit == m_rp_optics_.end())
      throw cms::Exception("") << "Optics data not available for RP " << it->getRPId() << ".";
  }

  // rough estimate of xi and th_y from each track
  for (const auto &track : tracks)
  {
    CTPPSDetId rpId(track->getRPId());
    unsigned int decRPId = rpId.arm()*100 + rpId.station()*10 + rpId.rp();

    if (verbosity)
      printf("* reconstructFromSingleRP(%u)\n", decRPId);

    auto oit = m_rp_optics_.find(track->getRPId());
    const double xi = oit->second.s_xi_vs_x->Eval(track->getX() * 1E-3); // mm --> m
    const double L_y = oit->second.s_L_y_vs_xi->Eval(xi);
    const double th_y = track->getY() * 1E-3 / L_y;

    if (verbosity)
      printf("    xi = %f, th_y = %E\n", xi, th_y);

    reco::ProtonTrack pt;
    pt.method = reco::ProtonTrack::rmSingleRP;
    pt.lhcSector = (CTPPSDetId(track->getRPId()).arm() == 0) ? reco::ProtonTrack::sector45 : reco::ProtonTrack::sector56;
    pt.contributingRPIds.insert(track->getRPId());
    pt.halfCrossingAngleSector45 = halfCrossingAngleSector45_;
    pt.halfCrossingAngleSector56 = halfCrossingAngleSector56_;

    const double p_nom = 6500.;  // GeV
    const double p = p_nom *  (1. - xi);

    const double sign_z_lhc = (pt.lhcSector == reco::ProtonTrack::sector45) ? -1. : +1.;

    pt.setValid(true);
    pt.setVertex(Local3DPoint(0., 0., 0.));
    pt.setDirection(Local3DVector(0., + p * th_y, - sign_z_lhc * p));
    pt.setXi(xi);
    pt.fitNDF = 0;
    pt.fitChiSq = 0.;

    out.push_back(move(pt));
  }
}
