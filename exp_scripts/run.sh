#!/bin/sh

param_file=param.txt

run () {
    OUTPUT_DIR=../../data/$DATE/$1/$3
    mkdir -p $OUTPUT_DIR
    agents_list=$2
    delay_probs=$4
    configs=$7
    for agents in ${agents_list[@]}
    do
        for dp in ${delay_probs[@]}
        do
            seed=$seed_start
            while [ $seed -lt $seed_limit ]
            do
                if [ -e $param_file ]; then rm $param_file; fi
                touch $param_file
                {
                    echo problem_type=MAPF_DP
                    echo map_file=${1}.map
                    echo agents=$agents
                    echo seed=$seed
                    echo delay_prob=$dp
                    echo random_problem=0
                    echo max_activation=$5
                    echo mapf_plan=$6
                    for config in ${configs[@]}
                    do
                        echo $config
                    done
                } > $param_file
                output_file=${1}_${agents}agents_dp-${dp}_seed-${seed}.txt
                ../build/app -i $param_file \
                             -s $3 \
                             -o $OUTPUT_DIR/$output_file \
                             -l
                seed=`expr $seed + 1`
            done
        done
    done
}
