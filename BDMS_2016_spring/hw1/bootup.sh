
#先格式化
rm -rf ~/work/hdfs ~/work/hbase-zookeeper
hdfs namenode -format

#再启动服务
start-dfs.sh

#放好数据
hdfs dfsadmin -safemode leave
hdfs dfs -mkdir hdfs://localhost:9000/hw1/
hdfs dfs -put ~/work/hw1/tpch/* hdfs://localhost:9000/hw1/
hdfs dfs -put ~/work/hw1/hw1-check/input/* hdfs://localhost:9000/hw1/
echo '配置完成，是否关闭服务？'
stop-dfs.sh
