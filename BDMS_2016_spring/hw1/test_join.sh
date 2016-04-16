#!/bin/bash
javac Hw1Grp0.java
java Hw1Grp0 R=/hw1/lineitem.tbl S=/hw1/orders.tbl join:R0=S0 res:S1,R1,R5
#java Hw1Grp0 R=/hw1/lineitem.tbl S=/hw1/part.tbl join:R1=S0 res:S1,S3,R5
