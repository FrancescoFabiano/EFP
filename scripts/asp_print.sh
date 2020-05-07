#!/usr/bin/env bash

DOMAIN="$1"      # Name of the domain
shift            # Shift all arguments to the left (original $1 gets lost)
ARGUMENTS=("$@") # Rebuild the array with rest of arguments
ASP_PATH="../asp"
OUT_PATH="../out/asp"
DOMAIN_PATH="../out/ASP_encoding"

clingo $ASP_PATH/planner_multi_noW.lp --configuration=jumpy $DOMAIN_PATH/$DOMAIN > $OUT_PATH/output.txt;
python3 $ASP_PATH/output_render/out_reader.py $OUT_PATH/output;
dot -Tpdf $OUT_PATH/output.dot > $OUT_PATH/output.pdf;

#rm $OUT_PATH/output.txt;
#rm $OUT_PATH/output.dot;
