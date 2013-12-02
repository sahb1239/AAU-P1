find *.c *.h | while read i 
do
	wc -l $i
done