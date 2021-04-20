def customizeTotemT2Digi(process):
    process.mix.mixObjects.mixSH.crossingFrames.append('TotemT2Hits')
    process.mix.mixObjects.mixSH.input.append(cms.InputTag('g4SimHits', 'TotemT2Hits'))
    process.mix.mixObjects.mixSH.subdets.append('TotemT2Hits')
    return process
