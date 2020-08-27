Time-Independent Planning
===
[![Build Status](https://travis-ci.com/Kei18/time-independent-planning.svg?token=NJ5EpN7k73FqKbLee887&branch=master)](https://travis-ci.com/Kei18/time-independent-planning)
[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

A simulator and visualizer of time-independent planning, used in a paper "Time-Independent Planning with Multiple Moving Agents".
It is written in C++(17) with [CMake](https://cmake.org/) build and tested on OSX 10.15.
The visualizer uses [openFrameworks](https://openframeworks.cc).

The implementations include emulation of FCP and MCP [1] in the time-independent model.

## Demo
![demo in small filed](/material/sample.gif)

Execution with Causal-PIBT enhanced by an MAPF plan.
The left shows the execution in the time-independent model.
Gray rectangles highlight activated agents.
Three modes are represented as:
- contracted: single filled circle (tail)
- requesting, one filled circle (tail) and another non-filled circle (head)
- extended, two filled circle (the large one: tail, the small one: head)

The right shows the adapted execution in the MAPF-DP setting.

## Building

```
git clone https://github.com/Kei18/time-independent-planning
cd time-independent-planning
mkdir build
cd build
cmake ..
make
```

## Usage
Causal-PIBT with MAPF plans
```sh
./app -i ../instances/sample.txt -s CAUSAL_PIBT_MAPF -o result.txt -v
```

Two problem types exist set by param file:
- `problem_type=MAPF_DP`: Emulate MAPF-DP [1]. Used in experiments.
- `problem_type=MAPF_RANDOM`: each agent randomly activated without any synchronization.
  The corresponding MAPF execution is obtained by Simple Temporal Network, inspired from [2].
  An example can be obtained as follows. __Following conflict__ is allowed in MAPF execution. [[result]](/material/small-crossing.gif)

```sh
./app -i ../instances/small-crossing_2agents.txt  # Causal-PIBT, saved in result.txt
```

You can find details and explanations for all parameters with:
```sh
./app --help
```

Please see `instances/sample.txt` for parameters of instances, e.g., filed, number of agents, max activation, etc.
Several instances are available in `instances/`.

Note:
- A log file can be huge. In such a case, using `-l` option simplifies the log but you cannot visualize the execution.

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

You can manipulate it via your keyboard. See printed info.

Note:
- It may take time to visualize large scale problems, e.g., in random-64-64-20 with 200 agents, I do not recommend to visualize it.

## Licence
This software is released under the MIT License, see [LICENSE.txt](LICENCE.txt).

## Notes
- Maps in `maps/` are from [MAPF benchmarks](https://movingai.com/benchmarks/mapf.html).
  When you add a new map, please place it in the `maps/` directory.
- The font in `visualizer/bin/data` is from [Google Fonts](https://fonts.google.com/).
- Scripts for the experiments are in `exp_scripts/`.
- Formats of MAPF plans follows [MAPF simulator](https://github.com/kei18/mapf).
  When you add a new MAPF plan, please place it in the `mapf_plan/` directory.

## Author
[Keisuke Okumura](https://kei18.github.io) is a Ph.D. candidate at Tokyo Institute of Technology, working on multiple moving agents.

## Reference
1. Ma, H., Kumar, T. K., & Koenig, S. (2016).
   Multi-agent path finding with delay probabilities.
   arXiv preprint arXiv:1612.05309.
1. Hönig, W., Kumar, T. S., Cohen, L., Ma, H., Xu, H., Ayanian, N., & Koenig, S. (2016).
   Multi-Agent Path Finding with Kinematic Constraints.
   In ICAPS (Vol. 16, pp. 477-485).
