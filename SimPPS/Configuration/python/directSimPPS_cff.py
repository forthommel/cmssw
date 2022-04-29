import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
from SimPPS.DirectSimProducer.ppsDirectProtonSimulation_cff import *

directSimPPSTask = cms.Task(
    beamDivergenceVtxGenerator,
    ppsDirectProtonSimulation
)

directSimPPS = cms.Sequence(directSimPPSTask)

def unshiftVertex(process, smearingParams):
    """Undo vertex smearing using the parameters used for the sample production"""
    if not hasattr(process, 'ctppsBeamParametersFromLHCInfoESSource'):
        return
    from importlib import import_module
    _params = import_module('IOMC.EventVertexGenerators.VtxSmearedParameters_cfi')
    if not hasattr(_params, smearingParams):
        raise ImportError('Failed to import {} from vertex smearing parameters!'.format(smearingParams))
    _params = getattr(_params, smearingParams)
    process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetX45 = cms.double(-_params.X0.value())
    process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetY45 = cms.double(-_params.Y0.value())
    process.ctppsBeamParametersFromLHCInfoESSource.vtxOffsetZ45 = cms.double(-_params.Z0.value())

