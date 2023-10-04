#!/usr/bin/env bash
#This script prints out the time for executing a plan on the same domain with all the heuristics of EFP
#USAGE EXAMPLE (from the project root folder): ". scripts/comparison_heuristics.sh"

#REMOVE OLD COPIES
if [ -f out/EFP_comparison/findingplan/"${@##*/}" ] ; then
    rm out/EFP_comparison/findingplan/"${@##*/}"
fi

TIMEOUT="30";

heuristics=("NONE" "S_PG" "L_PG" "C_PG" "SUBGOALS"); 

func(){
  # echo "1 is ${1}"
  # echo "1##*/ is ${1##*/}"
  # echo "-------------------"

  file=$1
  heur=$2
  tmp_output="findingplan_comparison_${heur}.tmp"
  header=""
  body=""
  # echo "Working with file: $tmp_output"
  

  timeout $TIMEOUT bin/efp.out $file -results_file -check_visited -st POSS -h $heur > "$tmp_output"
  exit_status=$?;

	if [[ $exit_status -eq 124 ]]; then
        header="EFP Version 2.0 (on POSS and fast Bisimulation) with $heur and VISTED-STATE TIMED-OUT \n"
        echo -e "$header" >> out/EFP_comparison/findingplan/"${1##*/}";
	else
		body=" [$heur]$(grep -w "Executed actions:" "$tmp_output")"
    body="$body \n [$heur]$(grep -w "Plan Length:" "$tmp_output")"
    body="$body \n [$heur]$(grep -w "Expanded Nodes:" "$tmp_output") \n"
    echo -e "$body" >> out/EFP_comparison/findingplan/"${1##*/}";
	fi;
    
    # sleep 2s;
  # rm "$tmp_output"
}

for heur in "${heuristics[@]}"; do
  echo "Spawning thread: $heur"
  func $@ $heur &
  # sleep 5
done;
wait

rm findingplan_comparison_*.tmp

# cd /mnt/d/Documents/NMSU/Research/Parallel\ Programming/Codebase/EFP/