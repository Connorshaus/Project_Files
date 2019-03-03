#!/usr/bin/perl -w

use strict;
my @line;
my $count;
my $bigger_count = 0;

my $i = 0;

while(<STDIN>)
{
        last unless /\S/;
        $line[$i]=$_;
	$count = length($line[$i]);
	if($count >= $bigger_count)
	{
		$bigger_count = $count;
	}
	chomp $line[$i];
        $i++;
	
}

my %num;
 
foreach my $str (@line)
{
    $num{$str}++;
}
 
foreach my $str (sort keys %num) 
{
    printf "%10s: %4s\n", $str, $num{$str};
}
