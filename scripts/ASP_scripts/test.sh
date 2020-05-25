#number_out = $@ | tr '\n' ' ' | sed -e 's/[^0-9]/ /g' -e 's/^ *//g' -e 's/ *$//g' | tr -s ' ' | sed 's/ /\n/g'
#echo -e planner_type[1];

echo -e "Configuration frumpy with 4 Threads: 53.365s (Solving: 44.29s 1st Model: 14.22s Unsat: 30.03s), cpu: 185.790s" | tr '\n' ' ' | sed -e 's/[^0-9|\.]/ /g' -e 's/^ *//g' -e 's/ *$//g' | tr -s ' ' | sed 's/ /\n/g' > num.tmp;
mapfile -t myArray < num.tmp

echo "Element [0]: ${myArray[0]}"
echo "Element [1]: ${myArray[1]}"
echo "Element [2]: ${myArray[2]}"
echo "Element [3]: ${myArray[3]}"
echo "Element [4]: ${myArray[4]}"
echo "Element [5]: ${myArray[5]}"
echo "Element [6]: ${myArray[6]}"