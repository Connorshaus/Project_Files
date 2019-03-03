#!/bin/bash

#This script checks to see if a database exists, grabs the appropriate schema and allows the user to fill in the given fieldsi
if [ "$#" -gt 1 ]; then
	echo "Please only enter one database at once"
fi

counter=0
flag=0
db_name=$1.db
dbfile="databases.txt"
schemafile="schemas.txt"

if [ ! -e $db_name ]; then
	echo "The database file does not exist..."
	exit
fi

if [ ! -e databases.txt ]; then
	echo "There is was no databases file created. Please run other scripts in order."
	exit
fi


db_grab=( `cut -d' ' -f1 databases.txt` )
schema_grab=( `cut -d' ' -f2 databases.txt` )

for i in ${db_grab[@]};
do
	if [ "$i" = "$1" ]; then
		the_schema="${schema_grab[counter]}"
		flag=1
		break	
	fi
	counter=$[counter + 1]
done

if [ $flag -eq 0 ]; then
	echo "There was no schema associated with the database provided"
	exit
fi



if [ $flag  -eq 1 ]; then
	grep $the_schema $schemafile | awk -F ":" '{print "Please enter the values for the schema "$the_schema""}'
fi

read valueField
echo $valueField >> $db_name
echo "Record added to \"$db_name \" database"
