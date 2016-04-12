#!/usr/bin/perl

#
# Written by Shimin Chen
#

my $script_path=`dirname $0`; chomp($script_path);
my $remove_comments= "$script_path/remove-comments.pl";
my $num_groups= 6;

# 1. all the test input and output data

my @join_cmd, @join_col, @join_res;

     $join_cmd[0]= "R=/hw1/join_0R.tbl S=/hw1/join_0S.tbl join:R0=S0 res:R1,S1";
     $join_cmd[1]= "R=/hw1/join_1R.tbl S=/hw1/join_1S.tbl join:R0=S0 res:R1,S1";
     $join_cmd[2]= "R=/hw1/join_2R.tbl S=/hw1/join_2S.tbl join:R0=S0 res:R1,S1";
     $join_cmd[3]= "R=/hw1/join_3R.tbl S=/hw1/join_3S.tbl join:R0=S0 res:R1,S1";
     $join_cmd[4]= "R=/hw1/join_4R.tbl S=/hw1/join_4S.tbl join:R0=S2 res:R1,S1";
     $join_cmd[5]= "R=/hw1/join_5R.tbl S=/hw1/join_5S.tbl join:R0=S3 res:R1,S1";
     $join_cmd[6]= "R=/hw1/join_6R.tbl S=/hw1/join_6S.tbl join:R0=S4 res:R1,S1";
     $join_cmd[7]= "R=/hw1/join_7R.tbl S=/hw1/join_7S.tbl join:R0=S5 res:R1,S1";
     $join_cmd[8]= "R=/hw1/join_8R.tbl S=/hw1/join_8S.tbl join:R2=S3 res:R1,S1";

     $join_col[0]= "rowkey res:R1 res:S1";
     $join_col[1]= "rowkey res:R1 res:S1";
     $join_col[2]= "rowkey res:R1 res:S1";
     $join_col[3]= "rowkey res:R1 res:S1";
     $join_col[4]= "rowkey res:R1 res:S1";
     $join_col[5]= "rowkey res:R1 res:S1";
     $join_col[6]= "rowkey res:R1 res:S1";
     $join_col[7]= "rowkey res:R1 res:S1";
     $join_col[8]= "rowkey res:R1 res:S1";
     
     $join_res[0]= "join_0res.tbl";
     $join_res[1]= "join_1res.tbl";
     $join_res[2]= "join_2res.tbl";
     $join_res[3]= "join_3res.tbl";
     $join_res[4]= "join_4res.tbl";
     $join_res[5]= "join_5res.tbl";
     $join_res[6]= "join_6res.tbl";
     $join_res[7]= "join_7res.tbl";
     $join_res[8]= "join_8res.tbl";

my @groupby_cmd, @groupby_col, @groupby_res;

     $groupby_cmd[0]= "R=/hw1/groupby_0.tbl groupby:R0 'res:count'";
     $groupby_cmd[1]= "R=/hw1/groupby_1.tbl groupby:R1 'res:max(R0),count'";
     $groupby_cmd[2]= "R=/hw1/groupby_2.tbl groupby:R2 'res:max(R0),max(R1),count'";
     $groupby_cmd[3]= "R=/hw1/groupby_3.tbl groupby:R3 'res:max(R0),max(R2)'";
     $groupby_cmd[4]= "R=/hw1/groupby_4.tbl groupby:R0 'res:count'";
     $groupby_cmd[5]= "R=/hw1/groupby_5.tbl groupby:R1 'res:max(R0),count'";
     $groupby_cmd[6]= "R=/hw1/groupby_6.tbl groupby:R2 'res:max(R0),max(R1),count'";
     $groupby_cmd[7]= "R=/hw1/groupby_7.tbl groupby:R3 'res:max(R0),max(R2)'";
     $groupby_cmd[8]= "R=/hw1/groupby_8.tbl groupby:R4 'res:count,max(R0),max(R3)'";

     $groupby_col[0]= "rowkey res:count";
     $groupby_col[1]= "rowkey 'res:max(R0)' res:count";
     $groupby_col[2]= "rowkey 'res:max(R0)' 'res:max(R1)' res:count";
     $groupby_col[3]= "rowkey 'res:max(R0)' 'res:max(R2)'";
     $groupby_col[4]= "rowkey res:count";
     $groupby_col[5]= "rowkey 'res:max(R0)' res:count";
     $groupby_col[6]= "rowkey 'res:max(R0)' 'res:max(R1)' res:count";
     $groupby_col[7]= "rowkey 'res:max(R0)' 'res:max(R2)'";
     $groupby_col[8]= "rowkey res:count 'res:max(R0)' 'res:max(R3)'";

     $groupby_res[0]= "groupby_0res.tbl";
     $groupby_res[1]= "groupby_1res.tbl";
     $groupby_res[2]= "groupby_2res.tbl";
     $groupby_res[3]= "groupby_3res.tbl";
     $groupby_res[4]= "groupby_4res.tbl";
     $groupby_res[5]= "groupby_5res.tbl";
     $groupby_res[6]= "groupby_6res.tbl";
     $groupby_res[7]= "groupby_7res.tbl";
     $groupby_res[8]= "groupby_8res.tbl";

my @distinct_cmd, @distinct_col, @distinct_res;

     $distinct_cmd[0]= "R=/hw1/distinct_0.tbl select:R0,gt,5 distinct:R1";
     $distinct_cmd[1]= "R=/hw1/distinct_1.tbl select:R1,ge,25 distinct:R2,R0";
     $distinct_cmd[2]= "R=/hw1/distinct_2.tbl select:R2,lt,167 distinct:R3,R3";
     $distinct_cmd[3]= "R=/hw1/distinct_3.tbl select:R3,le,1250 distinct:R4,R3";
     $distinct_cmd[4]= "R=/hw1/distinct_4.tbl select:R0,gt,5 distinct:R2";
     $distinct_cmd[5]= "R=/hw1/distinct_5.tbl select:R1,ge,25 distinct:R3,R0";
     $distinct_cmd[6]= "R=/hw1/distinct_6.tbl select:R2,lt,167 distinct:R4,R4";
     $distinct_cmd[7]= "R=/hw1/distinct_7.tbl select:R3,le,1250 distinct:R5,R4";
     $distinct_cmd[8]= "R=/hw1/distinct_8.tbl select:R4,gt,1000 distinct:R5,R4,R3";

     $distinct_col[0]= "res:R1";
     $distinct_col[1]= "res:R2 res:R0";
     $distinct_col[2]= "res:R3 res:R3";
     $distinct_col[3]= "res:R4 res:R3";
     $distinct_col[4]= "res:R2";
     $distinct_col[5]= "res:R3 res:R0";
     $distinct_col[6]= "res:R4 res:R4";
     $distinct_col[7]= "res:R5 res:R4";
     $distinct_col[8]= "res:R5 res:R4 res:R3";

     $distinct_res[0]= "distinct_0res.tbl";
     $distinct_res[1]= "distinct_1res.tbl";
     $distinct_res[2]= "distinct_2res.tbl";
     $distinct_res[3]= "distinct_3res.tbl";
     $distinct_res[4]= "distinct_4res.tbl";
     $distinct_res[5]= "distinct_5res.tbl";
     $distinct_res[6]= "distinct_6res.tbl";
     $distinct_res[7]= "distinct_7res.tbl";
     $distinct_res[8]= "distinct_8res.tbl";

if ($#ARGV < 1) {
   print "Usage: $0 <score output> <file...>\n";
   exit (0);
}

$| = 1; # set auto flush

open OUT, ">$ARGV[0]" or die "can't open $ARGV[0] for writing!\n";

for (my $i=1; $i<=$#ARGV; $i++) {
   my $file_name= $ARGV[$i];
   my $chk_grp_result= &checkGroup($file_name);
   print OUT $file_name, " group: ", $chk_grp_result, "\n";
   my $score= &grading($file_name);
   print OUT $file_name, " raw score: ", $score, "\n";
}

close(OUT);

# ---
# check if the group is computed correctly
# ---
sub checkGroup($)
{
   my ($file_name)= @_;

   if ($file_name =~ /(\d)_(\w+)_hw1.java/) {
      my $group= $1;
      my $student_id= $2;

      my $last_6digits= 0;
      if ($file_name =~ /(\d\d\d\d\d\d)_hw1.java/) {
            $last_6digits= $1;
      }
      else {
            return "last 6 char of ID not digits?";
      }
      my $compute = $last_6digits % $num_groups;
      if ($compute != $group) {
         return "should be group $compute";
      }

      return "good";
   }
   else {
      return "bad file name format";
   }
}

# ---
# get the group and student id
# ---
sub getGroupID($)
{
   my ($file_name)= @_;

   if ($file_name =~ /(\d)_(\w+)_hw1.java/) {
      my $group= $1;
      my $student_id= $2;

      return ($group, $student_id);
   }
   else {
      print "Error: Bad file name $file_name\n";
      return (-1, -1);
   }
}

# ---
# print the command and run it
# ---
sub mysystem($)
{
  my ($command) = @_;
  print $command, "\n";
  return system($command);
}

# ---
# set up a single test
# &setupTest($source, $main_class);
# ---
sub setupTest($$)
{
   my ($source, $main_class)= @_;

   # 1. create a sandbox directory
   &mysystem("rm -rf sandbox; mkdir sandbox");
   &mysystem("cp $source sandbox/$main_class.java");

   # 2. compile
   print "------------------------------------------------------------\n";
   print "Compile\n";
   print "------------------------------------------------------------\n";
   &mysystem("cd sandbox; javac $main_class.java 2>&1; cd ..");

   # 3. check class file
   my $classfile= "$main_class.class";
   if ( -f $classfile ) {
      return 0;
   }

   # 4. one common error is bad unknown characters in comments
   # let's try removing comments
   print "------------------------------------------------------------\n";
   print "remove comments\n";
   print "------------------------------------------------------------\n";
   &mysystem("cd sandbox; rm -f *; cd ..");
   &mysystem("cp $source sandbox/1.java");
   &mysystem("cd sandbox; dos2unix 1.java 2>&1; cd ..");
   &mysystem("$remove_comments sandbox/1.java > sandbox/$main_class.java");

   # 5. compile again
   print "------------------------------------------------------------\n";
   print "Compile again\n";
   print "------------------------------------------------------------\n";
   &mysystem("cd sandbox; javac $main_class.java 2>&1; cd ..");

   # 6. check class file
   my $classfile= "sandbox/$main_class.class";
   if ( -f $classfile ) {
      return 0;
   }
   else {
      print "Error compiling the source java file!\n";
      return -1;
   }
}

# ---
# run a single test
# &runTest($main_class, $param, $col, $result);
# ---
sub runTest($$$$)
{
   my ($main_class, $param, $col, $result)= @_;

   print "------------------------------------------------------------\n";
   print "Run $param\n";
   print "------------------------------------------------------------\n";

   # 1. remove hbase table
   &mysystem("hbase shell ./hbase-drop-result 2>&1");

   # 2. run
   &mysystem("cd sandbox; java $main_class $param >out 2>err; cd ..");

   # 3. obtain result
   &mysystem("cd bld; java HBase2TableAll Result $col > r.tbl; cd ..");
   &mysystem("mv bld/r.tbl $result");
}

# ---
# &checkResult($out, $good_result);
# ---
sub checkResult($$)
{
   my ($out, $good_result)= @_;

   print "------------------------------------------------------------\n";
   print "checkResult $out $good_result\n";
   print "------------------------------------------------------------\n";

   # 1. sort the output
   &mysystem("sort $out > sandbox/tmp.tbl");

   # 2. diff
   &mysystem("diff sandbox/tmp.tbl $good_result >sandbox/tmp.diff 2>&1");

   # 3. check diff
   my $mydiff= `cat sandbox/tmp.diff`; chomp($mydiff);
   if ($mydiff eq '') {
      return 1;
   }
   else {
      return 0;
   }
}

# ---
# grading homework 1
# &grading($source)
# ---
sub grading($)
{
   my ($source)= @_;
   my ($group, $student_id)= &getGroupID($source);

   if ($group == -1) {return 0;}
   
   my $main_class= "Hw1Grp". $group;

   # 1. set up test
   if (&setupTest($source, $main_class) < 0) {return 0;}

   # 2. run test
   for (my $i=0; $i<9; $i++) {
      my $out= "sandbox/$i.tbl";

      if (($group == 0) || ($group == 1)) {
        # join
        &runTest($main_class, $join_cmd[$i], $join_col[$i], $out);
      }
      elsif (($group == 2) || ($group == 3)) {
        # groupby
        &runTest($main_class, $groupby_cmd[$i], $groupby_col[$i], $out);
      }
      else {
        # distinct
        &runTest($main_class, $distinct_cmd[$i], $distinct_col[$i], $out);
      }
   }
   
   # 3. check result
   my $score= 0;
   for (my $i=0; $i<9; $i++) {
      my $out= "sandbox/$i.tbl";
      my $good_result;
      my $r= 0;

      if (($group == 0) || ($group == 1)) {
        # join
        $good_result= "result-sorted/$join_res[$i]";
      }
      elsif (($group == 2) || ($group == 3)) {
        # groupby
        $good_result= "result-sorted/$groupby_res[$i]";
      }
      else {
        # distinct
        $good_result= "result-sorted/$distinct_res[$i]";
      }

      $r= &checkResult($out, $good_result);
      $score += $r;
   }

   # 4. preserve the sandbox
   #my $pos= index($source, ".java");
   #if ($pos >= 0) {
   #    my $d= substr($source, 0, $pos);
   #    &mysystem("rm -rf $d; mv sandbox $d");
   #}

   return $score;
}
