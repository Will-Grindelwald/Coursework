/*
 * Make sure that the classpath contains all the hbase libraries
 *
 * Compile:
 *  javac HBaseTest.java
 *
 * Run: 
 *  java HBaseTest
 */

import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.MasterNotRunningException;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.ZooKeeperConnectionException;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;

import org.apache.log4j.*;

public class HBaseTest {
	
  public static void main(String[] args) throws MasterNotRunningException, ZooKeeperConnectionException, IOException {

    Logger.getRootLogger().setLevel(Level.WARN);

    // create table descriptor
    String tableName= "mytable";
    HTableDescriptor htd = new HTableDescriptor(TableName.valueOf(tableName));

    // create column descriptor
    HColumnDescriptor cf = new HColumnDescriptor("mycf");
    htd.addFamily(cf);

    // configure HBase
    Configuration configuration = HBaseConfiguration.create();
    HBaseAdmin hAdmin = new HBaseAdmin(configuration);

    if (hAdmin.tableExists(tableName)) {
        System.out.println("Table already exists");
    }
    else {
        hAdmin.createTable(htd);
        System.out.println("table "+tableName+ " created successfully");
    }
    hAdmin.close();

    // put "mytable","abc","mycf:a","789"

    HTable table = new HTable(configuration,tableName);
    Put put = new Put("abc".getBytes());
    put.add("mycf".getBytes(),"a".getBytes(),"789".getBytes());
    table.put(put);
    table.close();
    System.out.println("put successfully");
  }
}
