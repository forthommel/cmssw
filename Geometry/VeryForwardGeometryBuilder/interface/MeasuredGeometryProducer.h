/****************************************************************************
*
* Authors:
*  Jan Kaspar (jan.kaspar@gmail.com) 
*  Dominik Mierzejewski <dmierzej@cern.ch>
*    
****************************************************************************/

#ifndef Geometry_VeryForwardGeometryBuilder_MeasuredGeometryProducer_h
#define Geometry_VeryForwardGeometryBuilder_MeasuredGeometryProducer_h

#include "FWCore/Framework/interface/ESHandle.h"
 
#include "DetectorDescription/Core/interface/graphwalker.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDMaterial.h"
#include "DetectorDescription/Core/interface/DDSolid.h"
#include "DetectorDescription/Core/interface/DDSpecifics.h"
#include "DetectorDescription/Base/interface/DDRotationMatrix.h"
#include "DetectorDescription/Core/src/LogicalPart.h"

#include "DataFormats/CTPPSAlignment/interface/RPAlignmentCorrectionsData.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"

#include "Geometry/VeryForwardGeometryBuilder/interface/DDDTotemRPCommon.h"

#include <TMatrixD.h>

// Copies ideal_ddcv to measured_ddcv, applying the alignments if any
// WARNING: (TODO?) does not handle the "old" geometry
class MeasuredGeometryProducer
{
  private:
    const DDCompactView &idealCV;
    const RPAlignmentCorrectionsData *const alignments;
    DDLogicalPart root;
    DDCompactView *measuredCV;

    // -- Expanded view utilits ------------
    // WTF WARNING
    // DDExpandView's constructor sets DDRotation::StoreT to readonly
    //
    // Any newExpandedView() call will set this value before calling the constructor
    // Any delExpandedView() call will the restore StoreT state to _the_last_ set value
    static bool evRotationStoreState;

    // Allocate new ExpandedView and set it to point at the LogicalPart
    //
    // The LogicalPart _must_ be in the CompactView
    // Only name and ns of the LogicalPart are taken into account
    static DDExpandedView *newExpandedView(const DDCompactView&, const DDLogicalPart&);

    // Deallocate the ExpandedView
    //
    // Returns NULL
    static DDExpandedView *delExpandedView(DDExpandedView*);

    // -- Transformation matrix utils ------

    // Standard 4x4 tranformation matrixes are used for the alignments:
    //
    //  | R R R x |
    //  | R R R y |
    //  | R R R z |
    //  | 0 0 0 1 |
    //
    // where R are parameters of rotation matrix and x,y,z are translation parametres.
    // (Rotation and translation must be applied in this very order, as is done in CMSSW).
    // Such matrixes can be easily used to compose the transformations, 
    // e.g to describe transformation C: "first do A, then do B", multiply
    //      C = B * A
    // All tranformation matrixes are invertible.

    // Creates transformation matrix according to rotation and translation (applied in this order)
    static void translRotToTransform(const DDTranslation&, const DDRotationMatrix&, TMatrixD&);

    // sets rotation and translation (applied in this order) from transformation matrix 
    static void translRotFromTransform(DDTranslation&, DDRotationMatrix&, const TMatrixD&);

    // Gets global transform of given LogicalPart in given CompactView (uses ExpandedView to calculate)
    static void getGlobalTransform(const DDLogicalPart&, const DDCompactView&, TMatrixD&);

    // -- Misc. utils ----------------------

    // true if part's name maches DDD_TOTEM_RP_PRIMARY_VACUUM_NAME
    static inline bool isRPBox(const DDLogicalPart &part) {
      return (! part.name().name().compare(DDD_TOTEM_RP_PRIMARY_VACUUM_NAME));
    }

    // true if part's name maches DDD_TOTEM_RP_DETECTOR_NAME
    static inline bool isDetector(const DDLogicalPart &part) {
      return (! part.name().name().compare(DDD_TOTEM_RP_DETECTOR_NAME));
    }

    // Extracts RP id from object namespace - object must be RP_Box, or RP_Hybrid (NOT RP_Silicon_Detector)
    static TotemRPDetId getRPIdFromNamespace(const DDLogicalPart&);

    // -------------------------------------

    // Applies alignment (translation and rotation) to transformation matrix
    //
    // translation = alignmentTranslation + translation
    // rotation    = alignmentRotation + rotation
    static void applyCorrectionToTransform(const RPAlignmentCorrectionData&, TMatrixD&);

    // Applies relative alignments to Detector rotation and translation
    void applyCorrection(const DDLogicalPart&, const DDLogicalPart&, const RPAlignmentCorrectionData&, DDTranslation&, DDRotationMatrix&, const bool useMeasuredParent = true);

    void positionEverythingButDetectors(void);
    void positionDetectors(void);

  public:
    MeasuredGeometryProducer(const edm::ESHandle<DDCompactView> &idealCV,
    const edm::ESHandle<RPAlignmentCorrectionsData> &alignments) :
      idealCV(*idealCV), alignments(alignments.isValid() ? &(*alignments) : NULL) {
      root = this->idealCV.root();
    }

    DDCompactView *&produce();
};

#endif
