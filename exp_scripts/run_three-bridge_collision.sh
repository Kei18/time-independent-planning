#!/bin/sh
DATE=`date +%Y%m%d`
seed_start=0
seed_limit=100
source run.sh

field=three-bridge
declare -a delay_probs=(0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9)
max_activation=10000
mapf_plan=three-bridge_collision.txt
declare -a configs=(
    0,0,5,0
    5,0,0,0
    5,2,0,2
    0,2,5,2
    5,4,0,4
    0,4,5,4
)

run $field "6" FSP $delay_probs $max_activation $mapf_plan $configs
run $field "6" MCP $delay_probs $max_activation $mapf_plan $configs
run $field "6" GREEDY $delay_probs $max_activation $mapf_plan $configs
run $field "6" CAUSAL_PIBT $delay_probs $max_activation $mapf_plan $configs
run $field "6" CAUSAL_PIBT_MAPF $delay_probs $max_activation $mapf_plan $configs
