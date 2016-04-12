1. basic hdfs and hbase setup

   This should have been already done for Homework 1

2. start hdfs and hbase

  $ start-dfs.sh
  $ start-hbase.sh

3. check or copy input files to hdfs

  $ hdfs dfs -ls /hw1
  
  if the files in /input/* are not in /hw1, copy them to hdfs

  $ cd input
  $ for i in *; do hdfs dfs -put $i /hw1; done

4. run test 

e.g., suppose the java file is ./0_20141234000000_hw1.java

  $ ./run-test.pl score.out ./0_20141234000000_hw1.java 2>&1 | tee run.out
  $ cat score.out

  score.out will look like this:

./0_20141234000000_hw1.java group: good
./0_20141234000000_hw1.java raw score: 9

  The first line shows if the group is correctly computed from student ID
  The second line shows the number of tests passed

  sandbox/ directory contains the running results

  run.out contains all the outputs from run-test.pl.  You can have a look
          to see the actual test commands.


(Note: To avoid any precision problem from floating point computation, the
groupby test uses only count and max.)

