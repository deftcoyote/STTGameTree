#this calls the program for each of the roots 

roots=(00 01 02 07 08 10 11 17 )
innum=10
outnum=13

for i in "${roots[@]}"; do
    echo "Running $i $innum $outnum"
	./segmentdriver $i $innum $outnum;
done


#; executes sequentially
#& executes in parallel

#to flush stdout
#script -c <PROGRAM> -f OUTPUT.txt
#or
#stdbuf -oL /homedir/MyScript &> some_log.log
