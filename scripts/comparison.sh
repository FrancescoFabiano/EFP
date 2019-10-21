#This script prints out the time for executing a plan on the same domain with the two version of EFP
#USAGE EXAMPLE (from the project root folder): ". scripts/comparison.sh exp/paper/CC/CC_2_2_3.txt"
#	where "exp/paper/CC/CC_2_2_3.txt" is the path to the domain name
bin/efp.out $@ -old_check -st KRIPKE_OPT;
# bin/efp.out $@ -old_check;
bin/efp.out $@ -old_check -st POSS;
EFP_ICAPS_old/cpa+ $@ -old_check