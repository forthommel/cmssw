#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/Common/interface/Ref.h"

#include "DataFormats/CTPPSReco/interface/TotemRPCluster.h"
#include "DataFormats/CTPPSReco/interface/TotemRPRecHit.h"
#include "DataFormats/CTPPSReco/interface/TotemRPUVPattern.h"
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"
#include "DataFormats/CTPPSReco/interface/ProtonTrack.h"
#include "DataFormats/CTPPSReco/interface/Proton.h"

#include <vector>

namespace DataFormats_CTPPSReco {
  struct dictionary {
	TotemRPRecHit rp_reco_hit;
    edm::DetSet<TotemRPRecHit> ds_rp_reco_hit;
    edm::DetSetVector<TotemRPRecHit> dsv_rp_reco_hit;
    std::vector<edm::DetSet<TotemRPRecHit> > sv_dsw_rp_reco_hit;
    edm::Wrapper<edm::DetSetVector<TotemRPRecHit> > w_dsv_rp_reco_hit;
	std::vector<TotemRPRecHit> sv_rp_reco_hit;
	std::vector<const TotemRPRecHit*> sv_cp_rp_reco_hit;
    
    TotemRPCluster dc;
    edm::DetSet<TotemRPCluster> dsdc;
    std::vector<TotemRPCluster> svdc;
    std::vector<edm::DetSet<TotemRPCluster> > svdsdc;
    edm::DetSetVector<TotemRPCluster> dsvdc;
    edm::Wrapper<edm::DetSetVector<TotemRPCluster> > wdsvdc;

    TotemRPUVPattern pat;
    edm::DetSetVector<TotemRPUVPattern> dsv_pat;
    edm::Wrapper<edm::DetSetVector<TotemRPUVPattern>> w_dsv_pat;

    TotemRPLocalTrack ft;
    edm::DetSetVector<TotemRPLocalTrack> dsv_ft;
    edm::Wrapper<edm::DetSetVector<TotemRPLocalTrack> > w_dsv_ft;
    edm::DetSetVector<TotemRPLocalTrack::FittedRecHit> dsv_ft_frh;
    edm::Wrapper<edm::DetSetVector<TotemRPLocalTrack::FittedRecHit> > w_dsv_ft_frh;

    reco::ProtonTrack pt;
    std::vector<reco::ProtonTrack> vec_pt;
    edm::Wrapper<reco::ProtonTrack> wrp_pt;
    edm::Ref<std::vector<reco::ProtonTrack> > ref_vec_pt;
    edm::RefProd<std::vector<reco::ProtonTrack> > refprod_vec_pt;
    edm::Ptr<reco::ProtonTrack> ptr_pt;
    edm::Wrapper<edm::Ptr<reco::ProtonTrack> > w_ptr_pt;
    edm::PtrVector<reco::ProtonTrack> ptrvec_pt;
    edm::Wrapper<edm::PtrVector<reco::ProtonTrack> > w_ptrvec_pt;

    reco::Proton pr;
    std::vector<reco::Proton> vec_pr;
    edm::Wrapper<reco::Proton> wrp_pr;
    edm::Ref<std::vector<reco::Proton> > ref_vec_pr;
    edm::RefProd<std::vector<reco::Proton> > refprod_vec_pr;
    edm::Ptr<reco::Proton> ptr_pr;
    edm::Wrapper<edm::Ptr<reco::Proton> > w_ptr_pr;
    edm::PtrVector<reco::Proton> ptrvec_pr;
    edm::Wrapper<edm::PtrVector<reco::Proton> > w_ptrvec_pr;
  };
}
