Time-Independent Planning
===
[![Build Status](https://travis-ci.com/Kei18/time-independent-planning.svg?token=NJ5EpN7k73FqKbLee887&branch=master)](https://travis-ci.com/Kei18/time-independent-planning)
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

A simulator and visualizer of time-independent planning, used in a paper "Time-Independent Planning with Multiple Moving Agents".
It is written in C++(17) with [CMake](https://cmake.org/) build and tested on OSX 10.15.
The visualizer uses [openFrameworks](https://openframeworks.cc).

The implementations include emulation of FCP and MCP [1] in the time-independent model.

## Demo
![sample](/material/sample.gif)

executed with CausalPIBT enhanced by an MAPF plan

## Building

```
git clone https://github.com/Kei18/time-independent-planning
cd mapf
mkdir build
cd build
cmake ..
make
```

## Usage
Causal-PIBT
```sh
./app -i ../instances/sample.txt -s CAUSAL_PIBT -o result.txt -v
```

You can find details and explanations for all parameters with:
```sh
./app --help
```

Please see `instances/sample.txt` for parameters of instances, e.g., filed, number of agents, time limit, etc.

## Visualizer

### Building
It takes around 10 minutes.
```sh
git submodule init
git submodule update
sh ./openFrameworks/scripts/osx/download_libs.sh
cd visualizer/
make build
cd ..
chmod +x ./visualize.sh
```

### Usage
```sh
cd build
../visualize.sh result.txt
```

## Licence
This software is released under the MIT License, see [LICENSE.txt](LICENCE.txt).

## Notes
- Maps in `maps/` are from [MAPF benchmarks](https://movingai.com/benchmarks/mapf.html).
  When you add a new map, please place it in the `maps/` directory.
- The font in `visualizer/bin/data` is from [Google Fonts](https://fonts.google.com/).
- Scripts for the experiments are in `exp_scripts/`.

## Author
[Keisuke Okumura](https://kei18.github.io) is a Ph.D. candidate at Tokyo Institute of Technology, working on multiple moving agents.

## Reference
1. Ma, H., Kumar, T. K., & Koenig, S. (2016).
   Multi-agent path finding with delay probabilities.
   arXiv preprint arXiv:1612.05309.
