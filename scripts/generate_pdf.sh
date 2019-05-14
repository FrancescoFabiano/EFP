for file in "$@"/*;
do
   if [ "${file##*.}" = "dot" ]; then
	  dot -Teps "$file" >> "${file%.*}".eps
	  epstopdf "${file%.*}".eps
	  rm "${file%.*}".eps
	  rm "${file%.*}".dot
   fi
done