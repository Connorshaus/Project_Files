#!/usr/bin/perl -w

use strict;
use warnings;


my %word_count; 

while (my $line = <>)
{
    for ($line =~ /\w\w+/g) 
    {
        push @{$word_count{lc $_}{lines}}, $.;
        $word_count{lc $_}{repeat}++;
    }
}

foreach my $substring (sort keys %word_count)
{
    print "$substring: $word_count{$substring}{repeat} times, lines: @{$word_count{$substring}{lines}}\n";
}
