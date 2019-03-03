#!/bin/bash

#This program will help create shemas.txt and will output an error if one occurs

schema_name="$1"
schema_fields=( "$@" )

if [ ! -f schemas.txt ]; then
	touch schemas.txt
fi	

	schemas=( `cut -d' ' -f1 schemas.txt`)

if [ $# -gt 9 ];then
	echo "ABORT! Too many fields. Please only use eight or less!"
	exit
fi

for i in ${schemas[@]};
do
	if [ "$i" = "$schema_name" ]; then
		echo "Error, $1 already exists as a schema!"
		exit
	fi
done	

echo ${schema_fields[@]} >> schemas.txt;
echo "Schema \"$1\" has been created!"


