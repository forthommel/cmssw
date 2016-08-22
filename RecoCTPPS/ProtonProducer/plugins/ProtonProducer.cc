// -*- C++ -*-
//
// Package:    RecoCTPPS/ProtonProducer
// Class:      ProtonProducer
//
/**\class ProtonProducer ProtonProducer.cc RecoCTPPS/ProtonProducer/plugins/ProtonProducer.cc

 Description: Forward proton reconstruction

 Implementation:
     This implementation only relies on the information collected from
     CT-PPS strips detectors (Summer 2016 data taking)
*/
//
// Original Author:  Laurent Forthomme, Jan Kaspar
//         Created:  Mon, 22 Aug 2016 12:03:55 GMT
//
//
//

// system include files

#include <memory>

// user include files

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"
#include "DataFormats/CTPPSReco/interface/Proton.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoCTPPS/ProtonProducer/interface/ProtonKinematicsUtils.h"

#include <iostream>

class ProtonProducer : public edm::EDProducer
{
    public:
        ProtonProducer( const edm::ParameterSet & );
        ~ProtonProducer();

    private:
        void produce( edm::Event &, const edm::EventSetup & );
        void reconstructOneArm( const std::vector<reco::ProtonTrack>&, const std::vector<reco::ProtonTrack>&, const reco::ProtonTrack::Side&, std::vector<reco::Proton>& );

        edm::EDGetTokenT< edm::DetSetVector<TotemRPLocalTrack> > localTracksToken_;

        bool useXiInterp_;
        ProtonUtils::XiInterpolator* xiInterp_;

};

ProtonProducer::ProtonProducer( const edm::ParameterSet &iConfig ):
    localTracksToken_( consumes< edm::DetSetVector<TotemRPLocalTrack> >( iConfig.getParameter<edm::InputTag>( "protonTag" ) ) ),
    useXiInterp_( iConfig.getParameter<bool>( "useXiInterpolation" ) ), xiInterp_( 0 )
{
    produces< std::vector<reco::Proton> >();
    xiInterp_ = new ProtonUtils::XiInterpolator;
    if ( useXiInterp_ ) { xiInterp_->loadInterpolationGraphs( iConfig.getParameter<edm::FileInPath>( "xiInterpolationFile" ).fullPath().c_str() ); }
}

ProtonProducer::~ProtonProducer() {
    if ( xiInterp_ ) delete xiInterp_;
}

void
ProtonProducer::produce( edm::Event &evt, const edm::EventSetup & )
{
    edm::Handle< edm::DetSetVector<TotemRPLocalTrack> >  prptracks;
    evt.getByToken( localTracksToken_, prptracks );

    std::auto_ptr< std::vector<reco::Proton> > protonColl( new std::vector<reco::Proton> );

    xiInterp_->setAlignmentConstants( evt.id().run() ); // run-based alignment corrections

    std::vector<reco::ProtonTrack> fl_tracks, fr_tracks, nl_tracks, nr_tracks;
    for ( edm::DetSetVector<TotemRPLocalTrack>::const_iterator rp = prptracks->begin(); rp != prptracks->end(); rp++ ) {
        const unsigned int det_id = rp->detId();
        for ( edm::DetSet<TotemRPLocalTrack>::const_iterator proton = rp->begin(); proton != rp->end(); proton++ ) {
            if (!proton->isValid()) continue;

            reco::ProtonTrack frpp = reco::ProtonTrack( det_id, *proton );

std::cout << ">>> " << det_id << " ---> station:" << frpp.station() << " side:" << frpp.side() << std::endl;
std::cout << "     (" << frpp.decDetId() << ") >>>>>>>> " << frpp.detId() << std::endl;

            const reco::ProtonTrack::Station st = frpp.station();
            const reco::ProtonTrack::Side side = frpp.side();

            if      ( st==reco::ProtonTrack::FarStation  && side==reco::ProtonTrack::LeftSide )  fl_tracks.push_back( frpp );
            else if ( st==reco::ProtonTrack::FarStation  && side==reco::ProtonTrack::RightSide ) fr_tracks.push_back( frpp );
            else if ( st==reco::ProtonTrack::NearStation && side==reco::ProtonTrack::LeftSide )  nl_tracks.push_back( frpp );
            else if ( st==reco::ProtonTrack::NearStation && side==reco::ProtonTrack::RightSide ) nr_tracks.push_back( frpp );
        }
    }
    /*cerr << "number of tracks reconstructed:" << endl
         << "  left side:  near pot:" << nl_tracks.size() << ", far pot: " << fl_tracks.size() << endl
         << "  right side: near pot:" << nr_tracks.size() << ", far pot: " << fr_tracks.size() << endl;*/

    reconstructOneArm( nl_tracks, fl_tracks, reco::ProtonTrack::LeftSide, *protonColl );
    reconstructOneArm( nr_tracks, fr_tracks, reco::ProtonTrack::RightSide, *protonColl );

    evt.put( protonColl );
}

void
ProtonProducer::reconstructOneArm( const std::vector<reco::ProtonTrack>& near_coll,
                                   const std::vector<reco::ProtonTrack>& far_coll,
                                   const reco::ProtonTrack::Side& side,
                                   std::vector<reco::Proton>& out_coll )
{
    float min_distance = 999., xi = 0., err_xi = 0.;
    reco::Proton proton;

    if ( near_coll.size()==0 ) {
        for ( std::vector<reco::ProtonTrack>::const_iterator trk_f=far_coll.begin(); trk_f!=far_coll.end(); trk_f++ ) {
            proton = reco::Proton( *trk_f, side );

            if ( useXiInterp_ ) { xiInterp_->computeXiSpline( proton, &xi, &err_xi ); }
            else                { xiInterp_->computeXiLinear( proton, &xi, &err_xi ); }
            proton.setXi( xi, err_xi );

            if ( proton.isValid() ) out_coll.push_back( proton );
        }

        return;
    }

    for ( std::vector<reco::ProtonTrack>::const_iterator trk_n = near_coll.begin(); trk_n != near_coll.end(); trk_n++ ) {
        // checks if far pot tracks are reconstructed
        if ( far_coll.size()==0 ) {
            proton = reco::Proton( *trk_n, side );

            if ( useXiInterp_ ) { xiInterp_->computeXiSpline( proton, &xi, &err_xi ); }
            else                { xiInterp_->computeXiLinear( proton, &xi, &err_xi ); }
            proton.setXi( xi, err_xi );

            if ( proton.isValid() ) out_coll.push_back( proton );
            continue;
        }

        // associate a minimum-distance far pot track to this near pot track
        reco::ProtonTrack ft_sel;
        min_distance = 999.;
        for ( std::vector<reco::ProtonTrack>::const_iterator trk_f = far_coll.begin(); trk_f != far_coll.end(); trk_f++ ) {
            float dist = ProtonUtils::tracksDistance( *trk_n, *trk_f );
            if ( dist<min_distance ) {
                ft_sel = *trk_f;
                min_distance = dist;
            }
        }
        if ( !ft_sel.isValid() ) continue;

        proton = reco::Proton( *trk_n, ft_sel, side );

        if ( useXiInterp_ ) { xiInterp_->computeXiSpline( proton, &xi, &err_xi ); }
        else                { xiInterp_->computeXiLinear( proton, &xi, &err_xi ); }
        proton.setXi( xi, err_xi );

        if ( proton.isValid() ) out_coll.push_back( proton );
    }
}

DEFINE_FWK_MODULE( ProtonProducer );
