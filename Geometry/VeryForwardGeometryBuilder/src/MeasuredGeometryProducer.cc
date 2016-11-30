/****************************************************************************
 *
 * Authors:
 *  Jan Kaspar (jan.kaspar@gmail.com) 
 *  Dominik Mierzejewski <dmierzej@cern.ch>
 *    
 ****************************************************************************/

#include "Geometry/VeryForwardGeometryBuilder/interface/MeasuredGeometryProducer.h"

bool MeasuredGeometryProducer::evRotationStoreState;

DDExpandedView*
MeasuredGeometryProducer::newExpandedView(const DDCompactView &compactView, const DDLogicalPart &part)
{
  evRotationStoreState = DDRotation::StoreT::instance().readOnly();
  DDExpandedView *expandedView = new DDExpandedView(compactView);
  // traverse the tree until name and ns are mached
  const std::string& name = part.name().name();
  const std::string& ns = part.name().ns();
  bool noMatch = true;

  noMatch = false;
  noMatch |= expandedView->logicalPart().name().name().compare(name);
  noMatch |= expandedView->logicalPart().name().ns().compare(ns);
  while (noMatch) {
    expandedView->next();
    noMatch = false;
    noMatch |= expandedView->logicalPart().name().name().compare(name);
    noMatch |= expandedView->logicalPart().name().ns().compare(ns);
  }
  return expandedView;
}

DDExpandedView*
MeasuredGeometryProducer::delExpandedView(DDExpandedView *expandedView)
{
  delete expandedView;
  DDRotation::StoreT::instance().setReadOnly(evRotationStoreState);
  return NULL;
}

void
MeasuredGeometryProducer::translRotToTransform(const DDTranslation &translation, const DDRotationMatrix &rotation, TMatrixD &transform)
{
  // set rotation
  double values[9];
  rotation.GetComponents(values);
  for (int i = 0; i < 9; ++i) {
    transform[i / 3][i % 3] = values[i];
  }
  // set translation
  transform[0][3] = translation.X();
  transform[1][3] = translation.Y();
  transform[2][3] = translation.Z();
  transform[3][3] = 1.;
}

void
MeasuredGeometryProducer::translRotFromTransform(DDTranslation &translation, DDRotationMatrix &rotation, const TMatrixD &transform)
{
  // set rotation
  double values[9];
  for (int i = 0; i < 9; ++i) {
    values[i] = transform[i / 3][i % 3];
  }
  rotation.SetComponents(values, values + 9);
  // set translation
  translation.SetXYZ(transform[0][3], transform[1][3], transform[2][3]);
}

void
MeasuredGeometryProducer::getGlobalTransform(const DDLogicalPart &part, const DDCompactView &compactView, TMatrixD &transform)
{
  DDExpandedView *expandedView = newExpandedView(compactView, part);
  translRotToTransform(expandedView->translation(), expandedView->rotation(), transform);
  delExpandedView(expandedView);
}

TotemRPDetId
MeasuredGeometryProducer::getRPIdFromNamespace(const DDLogicalPart &part)
{
  const int nsLength = part.name().ns().length();
  const unsigned int rpDecId = atoi(part.name().ns().substr(nsLength - 3, nsLength).c_str());

  const unsigned int armIdx = rpDecId / 100;
  const unsigned int stIdx = (rpDecId / 10) % 10;
  const unsigned int rpIdx = rpDecId % 10;

  return TotemRPDetId(armIdx, stIdx, rpIdx);
}

void
MeasuredGeometryProducer::applyCorrectionToTransform(const RPAlignmentCorrectionData &correction, TMatrixD &transform)
{
  DDTranslation translation;
  DDRotationMatrix rotation;

  translRotFromTransform(translation, rotation, transform);

  translation = correction.getTranslation() + translation;
  rotation    = correction.getRotationMatrix() * rotation;

  translRotToTransform(translation, rotation, transform);
}

void
MeasuredGeometryProducer::applyCorrection(const DDLogicalPart &parent, const DDLogicalPart &child, const RPAlignmentCorrectionData &correction, DDTranslation &translation, DDRotationMatrix &rotation, const bool useMeasuredParent)
{
  TMatrixD C(4,4);    // child relative transform
  TMatrixD iP(4,4);   // ideal parent global transform
  TMatrixD mP(4,4);   // measured parent global transform
  TMatrixD F(4,4);    // final child transform

  translRotToTransform(translation, rotation, C);

  if (useMeasuredParent) 
    getGlobalTransform(parent, *measuredCV, mP);
  else 
    getGlobalTransform(parent, idealCV, mP);
  getGlobalTransform(parent, idealCV, iP);

  // global final transform
  F = iP * C;
  applyCorrectionToTransform(correction, F);
  // relative final transform
  mP.Invert();
  F = mP * F;

  translRotFromTransform(translation, rotation, F);
}

void
MeasuredGeometryProducer::positionEverythingButDetectors(void)
{
  DDCompactView::graph_type::const_iterator it = idealCV.graph().begin_iter();
  DDCompactView::graph_type::const_iterator itEnd = idealCV.graph().end_iter();
  for (; it != itEnd; ++it) {
    if (!isDetector(it->to())) {
      const DDLogicalPart from    = it->from();
      const DDLogicalPart to      = it->to();
      const int           copyNo      = it->edge()->copyno_;
      const DDDivision    &division   = it->edge()->division();
      DDTranslation       translation(it->edge()->trans());
      DDRotationMatrix    &rotationMatrix = *(new DDRotationMatrix(it->edge()->rot()));

      if (isRPBox(to)) {
        if (alignments != NULL) {
          TotemRPDetId rpId = getRPIdFromNamespace(to);

          const RPAlignmentCorrectionData correction = alignments->GetRPCorrection(rpId);
          applyCorrection(from, to, correction, translation, rotationMatrix, false);
        }
      }

      const DDRotation rotation = DDanonymousRot(&rotationMatrix);
      measuredCV->position(to, from, copyNo, translation, rotation, &division);
    }
  }
}

void
MeasuredGeometryProducer::positionDetectors(void)
{
  DDCompactView::graph_type::const_iterator it = idealCV.graph().begin_iter();
  DDCompactView::graph_type::const_iterator itEnd = idealCV.graph().end_iter();
  for (; it != itEnd; ++it) {
    if (isDetector(it->to())) {
      const DDLogicalPart from    = it->from();
      const DDLogicalPart to      = it->to();
      const int           copyNo      = it->edge()->copyno_;
      const DDDivision    &division   = it->edge()->division();
      DDTranslation       translation(it->edge()->trans());
      DDRotationMatrix    &rotationMatrix = *(new DDRotationMatrix(it->edge()->rot()));

      if (alignments != NULL) {
        TotemRPDetId detId = getRPIdFromNamespace(from);
        detId.setPlane(copyNo);

        const RPAlignmentCorrectionData correction = alignments->GetFullSensorCorrection(detId);
        applyCorrection(from, to, correction, translation, rotationMatrix);
      }

      const DDRotation rotation = DDanonymousRot(&rotationMatrix);
      measuredCV->position(to, from, copyNo, translation, rotation, &division);
    }
  }
}

DDCompactView *&
MeasuredGeometryProducer::produce()
{
  // create DDCompactView for measured geometry
  // notice that this class is not responsible for deleting this object
  measuredCV = new DDCompactView(root);
  // CMSSW/DetectorDescription graph interface sucks, so instead of doing a one bfs
  // we go over the tree twice (this is needed, as final detector postions are
  // dependent on new positions of RP units).
  positionEverythingButDetectors();
  positionDetectors();
  return measuredCV;
}
