#!/bin/bash
source `dirname $0`/util.sh
start_date=`getDate`

scen_start=1
scen_end=5
force=0
map="random-32-32-10.map"
agents_list="20 40 60"
exec_repetations=100

solvers=(
    "MCP"
    "CAUSAL_PIBT"
    "CAUSAL_PIBT_MAPF"
)

delay_probs="0.5"

for solver in ${solvers[@]}
do
    for dp in ${delay_probs[@]}
    do
        bash `dirname $0`/run.sh \
             $map \
             "$agents_list" \
             "$solver" \
             $scen_start \
             $scen_end \
             $dp \
             $exec_repetations \
             $force
    done
done
