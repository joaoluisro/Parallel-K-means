make
path="../input/"
i=1
for file in "10x1M.txt"
do
  ./kmeans < $path$file > sequencial.txt
  ./kmeans_p < $path$file > paralela.txt
  x=$(diff sequencial.txt paralela.txt)
  if [ -n "$x" ]
  then
    echo  [test $i status :  Failure]
    echo  error in file $file
  else
    echo  [test $i status : Success]
  fi
  i=$((i+1))
done
rm sequencial.txt
rm paralela.txt
make clean
