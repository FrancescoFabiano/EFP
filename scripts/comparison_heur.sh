#REMOVE OLD COPIES
if [ -f out/EFP_comparison/findingplan/"${@##*/}" ] ; then
    rm out/EFP_comparison/findingplan/"${@##*/}"
fi
representations=("KRIPKE_OPT" "KRIPKE" "POSS" "OLD");

for repr in "${representations[@]}"; do
    if [ "$repr" = "OLD" ]; then 
        EFP_ICAPS_old/cpa+ $@ -old_check -spg > findingplan_comparison.tmp;
    else
        bin/efp.out        $@ -old_check -st $repr -h PG > findingplan_comparison.tmp;
    fi;
    echo $'\n' >> out/EFP_comparison/findingplan/"${@##*/}";
    grep -w "Executed actions:" findingplan_comparison.tmp >> out/EFP_comparison/findingplan/"${@##*/}";
    grep -w "Plan Length:" findingplan_comparison.tmp  >> out/EFP_comparison/findingplan/"${@##*/}";
    #sleep 5s;
done

rm findingplan_comparison.tmp;