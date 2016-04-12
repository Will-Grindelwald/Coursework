#!/usr/bin/perl

#
# Written by Shimin Chen
#

if ($#ARGV != 0) {
  print "Usage: $0 <input_java>\n";
  exit (0);
}

my $in_comment= 0;
my $in_string= 0;

open IN, "$ARGV[0]" or die "can't open $ARGV[0]!\n";
while (<IN>) {chomp;
  my $line= $_;

 again:
  if ($in_comment == 1) {
     my $pos4 = index($line, '*/');
     if ($pos4 >= 0) {
        # */
        $line = substr($line, $pos4+2);
        $in_comment= 0;
        goto again;
     }
  }
  elsif ($in_string == 1) {
     my $pos5 = index($line, '"');
     if ($pos5 >= 0) {
        # found the matching "
        my $first = substr($line, 0, $pos5 +1); # including "
        my $second = substr($line, $pos5 +1);

        print $first;
        $line= $second;
        $in_string= 0;
        goto again;
     }
     else {
        print $line, "\n";
     }
  }
  else {
     my ($which, $pos)= &getNextMarker($line);

     if ($which == 0) {
        # "
        my $first= substr($line, 0, $pos+1); # including "
        my $second= substr($line, $pos+1);

        my $pos3 = index($second, '"');
        if ($pos3 >= 0) {
          # found the matching "
          my $third= substr($second, 0, $pos3+1); # including "
          my $fourth= substr($second, $pos3+1);
          print $first, $third;

          $line= $fourth;
          goto again;
        }
        else {
          print $line, "\n";
          $in_string= 1;
        }
     }
     elsif ($which == 1) {
        # /*
        my $first= substr($line, 0, $pos);
        my $second= substr($line, $pos+2);

        my $pos3 = index($second, '*/');
        if ($pos3 >= 0) {
          my $third= substr($second, $pos3+2);
          $line= $first . $third;
          goto again;
        }

        $line= $first;
        print $line, "\n";

        $in_comment= 1;
     }
     elsif ($which == 2) {
        # //
        $line= substr($line, 0, $pos);
        print $line, "\n";
     }
     else {
        # no markers
        print $line, "\n";
     }
  }
}
close(IN);

sub getNextMarker($)
{
  my ($cur_line)= @_;

  my $len= length($cur_line);

  my $pos_str= index ($cur_line, '"');
  if ($pos_str < 0) {$pos_str = $len + 1;}

  my $pos1 = index($cur_line, '/*');
  if ($pos1 < 0) {$pos1 = $len + 2;}

  my $pos2 = index($cur_line, '//');
  if ($pos2 < 0) {$pos2 = $len + 3;}

  if ($pos_str < $pos1 && $pos_str < $pos2) {
      if ($pos_str > $len) {return (-1, -1);}
      return (0, $pos_str);
  }
  elsif ($pos1 < $pos2) {
      return (1, $pos1);
  }
  else {
      return (2, $pos2);
  }
}
