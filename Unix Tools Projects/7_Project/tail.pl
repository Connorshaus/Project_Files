#!/usr/bin/perl -w

use strict;
my @line;

my $i = 0;
my $j;
my $k = 0;
my $last_num;
my $another_line;

while(<STDIN>)
{
        last unless /\S/;
        $line[$i]=$_;
        $i++;
}

if($#ARGV == -1)
{

	if ($i <= 9)
	{
		$j = 0;
        	while( $j < $i )
        	{
        	        print $line[$j];
        	        $j++;
        	}
	}	
	else
	{
		$j = $i -1;
       	 	while ($k <= 10)
       		{
               	 	print $line[$j];
                	$j--;
			$k++;
	        }
	}
}
elsif($#ARGV == 0)
{

$last_num = $ARGV[0];
substr($last_num, 0, 1) = "";

	if ($i <= $last_num)
	{
		
        	foreach $another_line (@line)
		{
			print $another_line, "\n";
		}
	}	
	else
	{
		$j = $i -1;
       	 	while ($k <= $last_num)
       		{
               	 	print $line[$j];
                	$j--;

			$k++;
	        }
	}

}



























