#!/bin/bash
javac Hw1Grp3.java
java Hw1Grp3 R=/hw1/orders.tbl groupby:R1 "res:count,avg(R3)"
java Hw1Grp3 R=/hw1/lineitem.tbl groupby:R2 "res:count,max(R4),max(R5)"
