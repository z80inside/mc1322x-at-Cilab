#!/usr/bin/perl -w

use strict;
use Carp;
use English;
use Text::CSV;
use Readonly;

Readonly my $SRC => 1;
Readonly my $DST => 2;
Readonly my $URI => 3;
Readonly my $PAYLOAD => -1;

#my $input_file = 'wire.txt';

#open my $wire_cap , '<', $input_file or 
#	croak "Can't open file $input_file: $OS_ERROR";
my $csv = Text::CSV->new();

while (my $line = <>) {
	if ($csv->parse($line)) {
		my @values = $csv->fields();
		if ($values[$URI] eq '') {
			next;
		}
		print "$values[$SRC]  $values[$DST]  $values[$URI]";
		if ($values[-1] =~ /offset: [ ] \d+, (.+)/xs) {
			print " $1";
		}
		print "\n";
	}
}
