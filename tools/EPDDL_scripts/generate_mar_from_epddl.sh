#!/usr/bin/env bash

#####Used to transform each .dot file inside a folder (argument) in a .pdf
#This script is used inside the code
PARSE_DIR="scripts/EPDDL_scripts/EPDDL_parser/custom-PEM"
TEMP_DIR="temp"

if [ -d "$TEMP_DIR" ]; then rm -Rf $TEMP_DIR; fi

python3 "$PARSE_DIR"/EPDDL_customPEM.py "$@" "$TEMP_DIR"
