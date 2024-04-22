#!/bin/bash

if [ $1 == 'write' ]
then
    for FNAME in 'find . -type f'
    do
    	if [ 'stat -c  %A $FNAME | cut -c 3' == 'w' ]
    	then 
    	    mkdir $FNAME.dir
    	    echo 'du -b $FNAME | cut -f 1' > $FNAME.dir/size.txt
    	fi
    done
    
elif [ $1 == 'read' ]
then
    for FNAME in 'find . -type f'
    do
    	if [ 'stat -c  %A $FNAME | cut -c 2' == 'r' ]
    	then 
    	    mkdir $FNAME.dir
    	    echo 'du -b $FNAME | cut -f 1' > $FNAME.dir/size.txt
    	fi
    done
    
elif [ $1 == 'execute' ]
then
    for FNAME in 'find . -type f'
    do
    	if [ 'stat -c  %A $FNAME | cut -c 4' == 'x' ]
    	then 
    	    mkdir $FNAME.dir
    	    echo 'du -b $FNAME | cut -f 1' > $FNAME.dir/size.txt
    	fi
    done
fi
