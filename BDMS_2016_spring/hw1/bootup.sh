
#先格式化
#rm -rf ~/work/hdfs ~/work/hbase-zookeeper
#hdfs namenode -format

#再启动服务
start-dfs.sh

#放好数据
hdfs dfsadmin -safemode leave
hdfs dfs -mkdir /hw1
hdfs dfs -put ./tpch/* /hw1
hdfs dfs -put ./hw1-check/input/* /hw1
echo '配置完成，是否关闭服务？'
stop-dfs.sh
