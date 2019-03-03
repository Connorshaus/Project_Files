#!/usr/bin/perl -w

use strict;
my @line;

my $i = 0;
my $j = 0;
my $k = 0;



while(<STDIN>)
{
	last unless /\S/;
	$line[$i]=$_;
	#chomp $line;
	$i++;
}
	
if ($i <= 9) 
{
	while( $j < $i )
	{
        	print $line[$j];
        	$j++;
	}
}
else 
{
	while ($j < 10)
	{
		print $line[$j];
		$j++;
	}
}
