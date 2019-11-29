#!/usr/bin/env bash
#This script prints out the time for executing a plan on the same domain with the two version of EFP
#USAGE EXAMPLE (from the project root folder): ". scripts/comparison.sh exp/paper/CC/CC_2_2_3.txt"
#	where "exp/paper/CC/CC_2_2_3.txt" is the path to the domain name

#REMOVE OLD COPIES
if [ -f out/EFP_comparison/findingplan/"${@##*/}" ] ; then
    rm out/EFP_comparison/findingplan/"${@##*/}"
fi
representations=("KRIPKE_OPT" "KRIPKE" "POSS" "OLD");

for repr in "${representations[@]}"; do
    if [ "$repr" = "OLD" ]; then 
        EFP_ICAPS_old/cpa+ $@ -results_file > findingplan_comparison.tmp;
    else
        bin/efp.out        $@ -results_file -st $repr > findingplan_comparison.tmp;
    fi;
    grep -w "Executed actions:" findingplan_comparison.tmp >> out/EFP_comparison/findingplan/"${@##*/}";
    grep -w "Plan Length:" findingplan_comparison.tmp  >> out/EFP_comparison/findingplan/"${@##*/}";
    echo $'\n' >> out/EFP_comparison/findingplan/"${@##*/}";

    sleep 5s;
done

rm findingplan_comparison.tmp;