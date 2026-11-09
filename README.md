# WebGPU (Dawn) + JUCE

This project demonstrates setting up WebGPU inside of a JUCE audio plugin. Follow the steps
below to get this running:

<p align="center">
  <img src="images/Basic_render.png" alt="Basic render on the main branch" width="300">
</p>

## Getting Started:

1) Install [CMake](https://cmake.org) if you don't already have it.

2) Clone the latest version of [JUCE](https://github.com/juce-framework/JUCE) inside the root directory.

3) Configure CMake project with cmake -B build -DCMAKE_BUILD_TYPE=Debug

4) Run cmake --build build

## Examples

# Each branch shows a different way of rendering graphics with WebGPU inside a JUCE plugin:

| Branch | Description                                                         |
|---|---------------------------------------------------------------------|
| `main` | Basic cross-platform renderer                                       |
| `Example_02_2D_Button_and_Viz` | On/off button and a simple visualization driven by a shader uniform |
| `Example_03_2D_Sequencer_with_Oscillator` | *In-progress, button with simple 6-note sequencer*                  |
| `Example_04_3D_Slider_with_Oscillator` | *Coming soon*                                                       |

To try an example:
```bash
git switch Example_02_2D_Button_and_Viz
cmake --build build

Thank you! Check out my 3D synthesizer plugin, Animated East at animatedinstruments.com

