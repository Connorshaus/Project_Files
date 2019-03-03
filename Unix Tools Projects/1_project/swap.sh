#!/bin/bash
#This shell script is used to swap two files around.
if [ -e $1 ] && [ -e $2 ];
then
	mv $1 temp_$1
	mv $2 $1
	mv temp_$1 $2
	echo "The swap has been completed!"
else
	if [ -e $2 ];
	then
		echo "There seems to be a problem with" $1
	elif [ -e $1 ];
	then	
		echo "There seems to be a problem with" $2
	else
		echo "There seems to be an error with BOTH files! :("

	fi
fi

