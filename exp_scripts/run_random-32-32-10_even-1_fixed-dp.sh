#!/bin/sh
DATE=`date +%Y%m%d`
seed_start=0
seed_limit=100
source run.sh

field=random-32-32-10
declare -a delay_probs=(0.5)
max_activation=100000
declare -a configs=(
    30,5,28,14
    23,18,23,27
    16,6,1,20
    18,1,29,29
    2,25,0,30
    17,29,31,0
    0,21,1,23
    23,2,2,27
    31,0,4,17
    17,27,16,19
    21,18,11,18
    6,10,29,18
    30,25,20,14
    7,21,6,19
    31,22,1,11
    12,28,12,5
    13,19,7,28
    0,15,9,0
    28,29,17,19
    19,22,19,23
    8,5,22,30
    20,24,19,30
    2,5,16,30
    18,17,20,20
    13,1,10,7
    18,18,15,23
    16,21,14,23
    20,31,22,6
    4,15,13,27
    8,28,5,1
    25,3,31,30
    13,13,27,2
    28,5,29,24
    1,11,17,30
    13,29,6,0
    22,27,3,16
    20,4,27,3
    29,10,28,13
    17,26,23,22
    22,30,25,2
    28,3,6,3
    18,12,25,15
    29,26,24,13
    24,14,2,9
    24,29,31,19
    14,25,20,6
    20,1,20,19
    28,1,18,24
    9,10,18,20
    20,8,24,2
    1,2,27,17
    7,22,15,31
    17,5,31,27
    23,13,24,30
    10,24,26,1
    7,15,22,3
    26,4,3,14
    7,18,26,30
    26,12,26,13
    19,6,14,28
    31,9,18,23
    29,28,1,13
    26,24,21,25
    11,2,9,3
    27,31,24,27
    13,24,26,26
    11,19,29,10
    15,19,13,18
    14,21,15,25
    14,17,1,17
    12,30,30,16
    1,4,11,29
    14,29,2,1
    5,17,10,12
    20,13,14,5
    16,0,0,15
    19,17,16,11
    26,5,29,12
    1,12,1,31
    15,9,14,25
    12,25,16,14
    13,9,16,9
    10,16,23,29
    20,12,26,11
    23,16,0,14
    3,5,29,7
    2,16,27,6
    29,23,18,28
    16,13,0,24
    6 30,2,3
)

mapf_plan=random-32-32-10_even-1_10agents.txt
run $field "10" FSP $delay_probs $max_activation $mapf_plan $configs
run $field "10" MCP $delay_probs $max_activation $mapf_plan $configs
run $field "10" CAUSAL_PIBT $delay_probs $max_activation $mapf_plan $configs
run $field "10" CAUSAL_PIBT_MAPF $delay_probs $max_activation $mapf_plan $configs

mapf_plan=random-32-32-10_even-1_20agents.txt
run $field "20" FSP $delay_probs $max_activation $mapf_plan $configs
run $field "20" MCP $delay_probs $max_activation $mapf_plan $configs
run $field "20" CAUSAL_PIBT $delay_probs $max_activation $mapf_plan $configs
run $field "20" CAUSAL_PIBT_MAPF $delay_probs $max_activation $mapf_plan $configs

mapf_plan=random-32-32-10_even-1_30agents.txt
run $field "30" FSP $delay_probs $max_activation $mapf_plan $configs
run $field "30" MCP $delay_probs $max_activation $mapf_plan $configs
run $field "30" CAUSAL_PIBT $delay_probs $max_activation $mapf_plan $configs
run $field "30" CAUSAL_PIBT_MAPF $delay_probs $max_activation $mapf_plan $configs

mapf_plan=random-32-32-10_even-1_40agents.txt
run $field "40" FSP $delay_probs $max_activation $mapf_plan $configs
run $field "40" MCP $delay_probs $max_activation $mapf_plan $configs
run $field "40" CAUSAL_PIBT $delay_probs $max_activation $mapf_plan $configs
run $field "40" CAUSAL_PIBT_MAPF $delay_probs $max_activation $mapf_plan $configs

mapf_plan=random-32-32-10_even-1_50agents.txt
run $field "50" FSP $delay_probs $max_activation $mapf_plan $configs
run $field "50" MCP $delay_probs $max_activation $mapf_plan $configs
run $field "50" CAUSAL_PIBT $delay_probs $max_activation $mapf_plan $configs
run $field "50" CAUSAL_PIBT_MAPF $delay_probs $max_activation $mapf_plan $configs

mapf_plan=random-32-32-10_even-1_60agents.txt
run $field "60" FSP $delay_probs $max_activation $mapf_plan $configs
run $field "60" MCP $delay_probs $max_activation $mapf_plan $configs
run $field "60" CAUSAL_PIBT $delay_probs $max_activation $mapf_plan $configs
run $field "60" CAUSAL_PIBT_MAPF $delay_probs $max_activation $mapf_plan $configs
