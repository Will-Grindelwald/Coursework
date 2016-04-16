# hw1
本文件夹存放了 Homework_1 的相关内容

需要安装 Hadoop & HBase，详见`安装与配置`  
`content.txt`存放了本文件夹的 tree 及 说明

作业的主要内容是:  
从HDFS读文件, 做中间处理, 输出到HBase  
中间处理: 

0. Hash join
* Sort‐merge join
* Hash based group‐by
* Sort based group‐by
* Hash based distinct
* Sort based distinct

需要额外提出的是:

1. `homework_1.pdf`是本次作业的说明
* 一切工作开始前先`./start.sh`，结束后`./stop.sh`
* 测试工作，详见`hw1-check`文件夹下的`README`
