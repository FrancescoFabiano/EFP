#!/usr/bin/env bash
#This script prints out the time for executing a plan on the same domain with the two version of EFP
#USAGE EXAMPLE (from the project root folder): ". scripts/comparison.sh exp/paper/CC/CC_2_2_3.txt"
#	where "exp/paper/CC/CC_2_2_3.txt" is the path to the domain name

#REMOVE OLD COPIES
if [ -f out/EFP_comparison/findingplan/"${@##*/}" ] ; then
    rm out/EFP_comparison/findingplan/"${@##*/}"
fi

#EXECUTE WITH KRIPKE OPTIMIZED
bin/efp.out        $@ -old_check -st KRIPKE_OPT > findingplan_comparison.tmp;
grep -w "Executed actions:" findingplan_comparison.tmp >> out/EFP_comparison/findingplan/"${@##*/}";
grep -w "Plan Length:" findingplan_comparison.tmp  >> out/EFP_comparison/findingplan/"${@##*/}";
echo $'\n' >> out/EFP_comparison/findingplan/"${@##*/}";

#EXECUTE WITH KRIPKE
bin/efp.out        $@ -old_check -st KRIPKE > findingplan_comparison.tmp;
grep -w "Executed actions:" findingplan_comparison.tmp >> out/EFP_comparison/findingplan/"${@##*/}";
grep -w "Plan Length:" findingplan_comparison.tmp  >> out/EFP_comparison/findingplan/"${@##*/}";
echo $'\n' >> out/EFP_comparison/findingplan/"${@##*/}";

#EXECUTE WITH KRIPKE POSSIBILITIES
bin/efp.out        $@ -old_check -st POSS > findingplan_comparison.tmp;
grep -w "Executed actions:" findingplan_comparison.tmp >> out/EFP_comparison/findingplan/"${@##*/}";
grep -w "Plan Length:" findingplan_comparison.tmp  >> out/EFP_comparison/findingplan/"${@##*/}";
echo $'\n' >> out/EFP_comparison/findingplan/"${@##*/}";

#EXECUTE WITH OLD VERSIONE
EFP_ICAPS_old/cpa+ $@ -old_check > findingplan_comparison.tmp;
grep -w "Executed actions:" findingplan_comparison.tmp >> out/EFP_comparison/findingplan/"${@##*/}";
grep -w "Plan Length:" findingplan_comparison.tmp  >> out/EFP_comparison/findingplan/"${@##*/}";

rm findingplan_comparison.tmp;