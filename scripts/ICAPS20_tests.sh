#!/usr/bin/env bash
#Just Run
INPATH="exp/ICAPS20/tests/"; #Path of the domain
OUTPATH="out/ICAPS20/tests/"; #Path of the output

OUTPATH_FINDINGPLAN='out/EFP_comparison/findingplan/'

mkdir -p $OUTPATH
DOMAIN_EXT=".txt"; # Name of the domain




#mv $OUTPATH_FINDINGPLAN $OUTPATH$DOMAIN_NAME$DOMAIN_EXT;

echo -e "\nExecution of efp.out in all the folders in" $INPATH "\n"
for dir in $INPATH*; do
    if [ -d "$dir" ]; then

        echo -e "Currently working on the $(basename $dir) directory";
        #mkdir -p $OUTPATH"$(basename $dir)"

        for file in $(find "$dir"/ -type f -name *$DOMAIN_EXT); do
        mkdir -p $OUTPATH"$(basename $dir)"/"$(basename ${file%%.*})"/"for_average"

            echo -e "    Solving the instance $(basename ${file%%.*})";
            for i in {1..3}; do
                . scripts/comparison_timeout.sh "$file";
                #mv $OUTPATH_FINDINGPLAN"$(basename $file)" $OUTPATH"$(basename $dir)"/"$(basename $file)"
                grep -w "EFP" $OUTPATH_FINDINGPLAN"$(basename $file)" >> $OUTPATH"$(basename $dir)"/"$(basename ${file%%.*})"/"for_average"/"exec_$i.txt";
               # while read line; do
               #    if ["$line" = "EFP"*]; then
               #       [ -z "$line" ] && continue;
               #      echo ${line##* };
               # fi;
               #done < $OUTPATH_FINDINGPLAN"$(basename $file)";
            done
            mv $OUTPATH_FINDINGPLAN"$(basename $file)" $OUTPATH"$(basename $dir)"/"$(basename ${file%%.*})"/"results.txt";
        done
    fi
    echo -e "Done with $(basename $dir)\n"
done
echo -e "The end :)"