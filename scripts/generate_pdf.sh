#####Used to transform each .dot file inside a folder (argument) in a .pdf
#This script is used inside the code
for file in "$@"/*;
do
   if [ "${file##*.}" = "dot" ]; then
	  dot -Teps "$file" >> "${file%.*}".eps
	  epstopdf "${file%.*}".eps
	  rm "${file%.*}".eps
	  rm "${file%.*}".dot
   fi
done