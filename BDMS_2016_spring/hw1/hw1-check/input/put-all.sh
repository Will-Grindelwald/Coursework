#!/bin/bash

for i in *.tbl; do 
   echo hdfs dfs -put $i /hw1/$i; 
   hdfs dfs -put $i /hw1/$i; 
done
