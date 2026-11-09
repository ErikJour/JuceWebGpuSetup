# WebGPU (Dawn) + JUCE

This project demonstrates setting up WebGPU inside of a JUCE audio plugin. Follow the steps
below to get this running:

<p align="center">
  <img src="images/Basic_render.png" alt="Basic render on the main branch" width="600">
</p>

## Getting Started:

1) Install [CMake](https://cmake.org) if you don't already have it

2) Clone the latest version of JUCE inside the root directory

3) Configure CMake project with cmake -B build -DCMAKE_BUILD_TYPE=Debug

4) Run cmake --build build

5) Explore the various branches for different examples of rendering graphics using WebGPU inside a JUCE plugin. Current:

    A) Main -> basic cross-platform renderer

    B) Example_02_2D_Button_and_Viz -> example project with on / off button and simple visualization via uniform to shader.

    C) Coming soon -> Example_03_2D_Sequencer_with_Oscillator

    D) Coming soon -> Example 04_3D_Slider_with_Oscillator

Thank you! Check out my 3D synthesizer plugin, Animated East at animatedinstruments.com

