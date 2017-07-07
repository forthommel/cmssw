import ROOT
from DataFormats.FWLite import Events, Handle

ROOT.gROOT.SetBatch()

events = Events('root://eoscms.cern.ch//store/group/phys_pps/diphoton/GammaGammaToGammaGamma_13TeV_fpmc/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1_MINIAODSIM/170324_062301/0000/GammaGammaGammaToGammaGamma-RunIISummer16MiniAODv2_1.root')
bs_handle = Handle('reco::BeamSpot')
bs_label = ('offlineBeamSpot')
gen_handle = Handle('std::vector<reco::GenParticle>')
gen_label = ('prunedGenParticles')

for ev in events:
    ev.getByLabel(bs_label, bs_handle)
    bs = bs_handle.product()
    print 'bs:', bs.x0(), bs.y0(), bs.z0()
    bs_pos = bs.position()

    ev.getByLabel(gen_label, gen_handle)
    gen = gen_handle.product()
    for p in gen:
        part_pos = p.vertex()
        part_rel_pos = part_pos-bs_pos
        print p.pdgId(), p.numberOfDaughters(), part_rel_pos.x(), part_rel_pos.y(), part_rel_pos.z()
