from sys import argv
import ROOT
from DataFormats.FWLite import Events, Handle

ROOT.gROOT.SetBatch()

def raw_id(detid):
    arm = (detid >> 24)&1
    pot = (detid >> 19)&7
    return arm*100+pot

def main(argv):
    events = Events('ctppsSim.root')
    lt_handle = Handle('std::vector<CTPPSLocalTrackLite>')
    lt_label = ('ctppsOpticsParameterisation')
    gen_handle = Handle('std::vector<reco::GenParticle>')
    gen_label = ('prunedGenParticles')

    #----- histograms booking

    pots = [2, 3, 102, 103]
    #--- book the 2d plots for hitmaps
    plots = {}
    for p in pots:
        plots[p] = ROOT.TH2D('pot_%i' % p, 'RP %i;x;y' % p, 100, -1.e-1, 1.e-1, 100, -1.e-1, 1.e-1)

    #----- main events loop

    for ev in events:
        ev.getByLabel(lt_label, lt_handle)
        tracks = lt_handle.product()
        for trk in tracks:
            detid = raw_id(trk.getRPId())
            plots[detid].Fill(trk.getX(), trk.getY())

    #----- end of events loop

    #----- plotting part

    c = ROOT.TCanvas('hitmaps', '', 800, 800)
    c.Divide(len(pots)/2,2)
    i = 0
    for pot in pots:
        c.cd(i+1)
        plots[pot].Draw('colz')
        i += 1
    c.SaveAs('hitmaps.png')

    #----- end of plotting part

if __name__=='__main__':
    main(argv)
