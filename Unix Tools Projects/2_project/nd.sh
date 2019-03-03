#1/bin/bash

#This will create a new database

if [ ! -f databases.txt ]; then
	touch databases.txt
fi

if [ ! -f schemas.txt ]; then
	touch schemas.txt
fi


flag=0
database_name="$1"
schema_name="$2"
database=( `cut -d' ' -f1 databases.txt` )
schema=( `cut -d' ' -f2 databases.txt` )
schema_check=( `cut -d' ' -f1 schemas.txt` )

if [ $# -gt 2 ]; then
	echo "ABORT! Too many arguments, please only enter two"
	exit
fi

for i in ${database[@]};
do
        if [ "$i" = "$database_name" ]; then
                echo "Error, $1 already exists as a database!"
		exit
	fi
done

for  i in ${schema_check[@]};
do
        if [ "$i" = "$schema_name" ]; then
		flag=1;
                break
        fi
done

if [ $flag -eq 1 ]; then
	echo "$1 $2" >> databases.txt
	echo "Database \"$1\" using schema \"$2\" has been created"
	touch $1.db
else
	echo "Schema "$2" does not exist!"
fi


