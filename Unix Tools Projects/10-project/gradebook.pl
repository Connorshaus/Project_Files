#!/usr/bin/perl -w
use strict; 
use warnings;

#declaring all the variable to store filenames
my $studentFilename = "students.txt";       
my $scoreFilename = "scores.txt";       
my $weightFilename = "items.txt";       
my $reportFilename = "reports.txt";

#Declaring hash to store the student ID - Name
my %studentIDName;
#Declaring hash to store the student Name  - ID 
my %studentNameID;
#Declaring hash to store the student subject name - weight 
my %examWeight;
#declaring hash ID - Subject - Marks
my %score;


# This opens file, but if file isn't there it mentions this will not open
open INFILE, $studentFilename or die "Can't open $studentFilename: $!";       
#reading file line by line
while (my $line = <INFILE>) {
    #removing new line character
    chomp $line;
    #spliting line into : which get data into array
    my @array = split(':',$line);
    $studentIDName{$array[1]} = $array[0];
}


%studentNameID = reverse %studentIDName;

# This opens file, but if file isn't there it mentions this will not open
open INFILE, $weightFilename or die "Can't open $weightFilename: $!";       
#reading file line by line
while (my $line = <INFILE>) {
    #removing new line character
    chomp $line;
    #spliting line into : which get data into array
    my @array = split(' ',$line);
    $examWeight{$array[0]} = $array[1];
}


# This opens file, but if file isn't there it mentions this will not open
open INFILE, $scoreFilename or die "Can't open $scoreFilename: $!";       
#reading file line by line
while (my $line = <INFILE>) {
    #removing new line character
    chomp $line;
    #spliting line into : which get data into array
    my @array = split(' ',$line);
    $score{$array[0]}{$array[1]} = $array[2];
}


my $exam1Avg = 0;
my $exam2Avg = 0;
my $asg1Avg = 0;
my $asg2Avg = 0;
my $asg3Avg = 0;
my $finalAvg = 0;

# writing the final report
open(my $fh, '>', $reportFilename) or die "Could not open file '$reportFilename' $!";
my $string = sprintf("%-20s %6s %6s %6s %5s %5s %5s %9s\n", "Name", "StuID","exam1", "exam2", "asg1","asg2" ,"asg3","average");
print $fh $string;
my $dash = "-------------------- ------ ------ ------ ----- ----- ----- ---------\n";
print $fh $dash;
foreach my $name (sort(keys %studentNameID)){
    my $finalScore = 0;
    my $id = $studentNameID{$name};
    $string = sprintf("%-20s ",$name);
    $string .= sprintf("%6s ",$id);
    if ( exists $score{$id}{"exam1"}){
        $string .= sprintf("%6s ",$score{$id}{"exam1"});
        $exam1Avg += $score{$id}{"exam1"};
        $finalScore += $score{$id}{"exam1"} * $examWeight{"exam1"} /100;
    }
    else{
        $string .= sprintf("%6s ","");
    }

    if ( exists $score{$id}{"exam2"}){
        $string .= sprintf("%6s ",$score{$id}{"exam2"});
        $exam2Avg += $score{$id}{"exam2"};
        $finalScore += $score{$id}{"exam2"} * $examWeight{"exam2"} /100;
    }
    else{
        $string .= sprintf("%6s ","");
    }
    
    if ( exists $score{$id}{"asg1"}){
        $string .= sprintf("%5s ",$score{$id}{"asg1"});
        $asg1Avg += $score{$id}{"asg1"};
        $finalScore += $score{$id}{"asg1"} * $examWeight{"asg1"} /100;
    }
    else{
        $string .= sprintf("%5s ","");
    }
    if ( exists $score{$id}{"asg2"}){
        $string .= sprintf("%5s ",$score{$id}{"asg2"});
        $asg2Avg += $score{$id}{"asg2"};
        $finalScore += $score{$id}{"asg2"} * $examWeight{"asg2"} /100;
    }
    else{
        $string .= sprintf("%5s ","");
    }
    if ( exists $score{$id}{"asg3"}){
        $string .= sprintf("%5s ",$score{$id}{"asg3"});
        $asg3Avg += $score{$id}{"asg3"} ;
        $finalScore += $score{$id}{"asg3"} * $examWeight{"asg3"} /100;
    }
    else{
        $string .= sprintf("%5s ","");
    }
    $string .= sprintf("%7.2f\n",$finalScore);
    $finalAvg += $finalScore;
    print $fh "$string";
}
print $fh $dash;
my $numStudent  = keys %studentIDName;
$string = sprintf("%-20s ", "average");
$string .= sprintf("%6s ", "");
$string .= sprintf("%6d ", $exam1Avg / $numStudent);
$string .= sprintf("%6d ", $exam2Avg / $numStudent);
$string .= sprintf("%5d ", $asg1Avg /  $numStudent);
$string .= sprintf("%5d ", $asg2Avg /  $numStudent);
$string .= sprintf("%5d ", $asg3Avg /  $numStudent);
$string .= sprintf("%7.2f\n", $finalAvg / $numStudent);
print $fh $string;

