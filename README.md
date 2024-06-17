# JUCE DX21
This repository was initially made for a final project of a bachelor's degree in Music and Computer Science, with the goal being to emulate a four-oscillator synthesizer, namely the Yamaha DX21, as accurately as possible.

The project was built in JUCE and can be compiled as a VST3 plugin, and Audio Component plugin, or a standalone plugin. The layout and code structure took inspiration from [The Audio Programmer's JUCE Synth](https://github.com/TheAudioProgrammer/tapSynth), as it's highly componentized structure in the front and back end was very intuitive to understand and work with.

<a href="#">
    <img src="https://github.com/Keeelan/csc497/blob/main/ui.jpg">
  </a>

## Features
As the project currently stands, the synth has four adjustable oscillators, each with amplitude and ratio knobs. Each oscillator can be used as a carrier oscillator, or in the case of oscillators 2 through 4, be used as modulation oscillators, by changing which FM algorithm is activated via a dropdown menu in the UI.
The algorithms correspond to the same algorithms used in the DX21, which can be seen below:

<a href="#">
    <img src="https://github.com/Keeelan/csc497/blob/main/algorithms.jpeg">
  </a>

The synth also features a global gain slider, along with global adsr sliders that affect all carrier oscillators.
The LFO component features an LFO frequency dial, which adheres to the bounds of the DX21's LFO: from 0 being 0.06Hz and 99 being 50Hz. Below are two additional dials, the first controlling the range of pitch modulation the LFO will fluctuate between and the second being the same but for amplitude modulation.

## Planned Features
-  feedback modulation support for OSC 4
-  individual ADSR controls for each oscillator
-  chorus effect
-  saving and loading parameter presets
