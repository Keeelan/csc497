# JUCE DX21
This repository was initially made for a final project of a bachelor's degree in Music and Computer Science, with the goal being to emulate a four-oscillator synthesizer, namely the Yamaha DX21, as accurately as possible.

The project was built in JUCE, and the layout and code structure took inspiration from [The Audio Programmer's JUCE Synth](https://github.com/TheAudioProgrammer/tapSynth), as it's highly componentized structure in the front and back end was very intuitive to work with.

## Features
As the project currently stands, the synth has four adjustable oscillators, each with amplitude and ratio knobs. Each oscillator can be used as a carrier oscillator, or in the case of oscillators 2 through 4, be used as modulation oscillators, by changing which FM algorithm is activated via a dropdown menu in the UI.
The algorithms correspond to the same algorithms used in the DX21, which can be seen below:
