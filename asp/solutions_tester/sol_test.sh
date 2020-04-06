#!/usr/bin/env bash
DOMAIN="$1" # Name of the domain
shift            # Shift all arguments to the left (original $1 gets lost)
ARGUMENTS=("$@") # Rebuild the array with rest of arguments

rm -f tmp_sol_test.tmp;
../../bin/efp.out $DOMAIN -st POSS -e "${ARGUMENTS[@]}" >> tmp_sol_test.tmp;