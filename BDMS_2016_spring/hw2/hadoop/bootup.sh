
#先格式化
#rm -rf ~/work/hdfs ~/work/hbase-zookeeper
#hdfs namenode -format

#再启动服务
start-dfs.sh

#放好数据
hdfs dfsadmin -safemode leave
hdfs dfs -mkdir /hw2
hdfs dfs -put ./example-input.txt /hw2
hdfs dfs -put ./part1-input/* /hw2
echo '配置完成，是否关闭服务？'
stop-dfs.sh
