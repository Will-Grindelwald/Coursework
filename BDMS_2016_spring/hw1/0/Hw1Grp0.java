// > Last Update:   2016-04-09 22:35:02
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Map.Entry;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.MasterNotRunningException;
import org.apache.hadoop.hbase.ZooKeeperConnectionException;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.client.Get;
import org.apache.hadoop.hbase.client.Result;

public class Hw1Grp0 {

    public static void main(String[] args) throws IOException, URISyntaxException, MasterNotRunningException, ZooKeeperConnectionException {

        String filePath1 = "hdfs://localhost:9000" + args[0].substring(2);
        String filePath2 = "hdfs://localhost:9000" + args[1].substring(2);
        int indexR = args[2].indexOf("R");
        int indexE = args[2].indexOf("=");
        int indexS = args[2].indexOf("S");
        int joinKeyR, joinKeyS;
        if (indexR < indexS) {
            joinKeyR = Integer.valueOf(args[2].substring(indexR + 1, indexE));
            joinKeyS = Integer.valueOf(args[2].substring(indexS + 1));
        } else {
            joinKeyS = Integer.valueOf(args[2].substring(indexS + 1, indexE));
            joinKeyR = Integer.valueOf(args[2].substring(indexR + 1));
        }
        int indexRes = args[3].indexOf(":");
        String columnFamily = args[3].substring(0, indexRes);
        String[] columnKey = args[3].substring(indexRes + 1).split(",");
        ArrayList<int[]> operations = new ArrayList<int[]>();
        int[] op = null;
        for (String str : columnKey) {
            op = new int[2];            // op[0] for table, op[1] for tuple
            if (str.substring(0, 1).equals("R")) op[0] = 1;
            else if (str.substring(0, 1).equals("S")) op[0] = 2;
            else {
                System.out.println("parameter error!");
                System.exit(0);
            }
            op[1] = Integer.valueOf(str.substring(1));
            operations.add(op);
        }

        // create table descriptor
        String tableName = "Result";
        HTableDescriptor htd = new HTableDescriptor(TableName.valueOf(tableName));
        // create column descriptor
        HColumnDescriptor cf = new HColumnDescriptor(columnFamily);
        htd.addFamily(cf);
        // configure HBase
        Configuration configuration = HBaseConfiguration.create();
        HBaseAdmin hAdmin = new HBaseAdmin(configuration);
        if (hAdmin.tableExists(tableName)) {
            hAdmin.disableTable(tableName);
            hAdmin.deleteTable(tableName);
        }
        hAdmin.createTable(htd);
        hAdmin.close();
        HTable table = new HTable(configuration, tableName);

        Configuration conf = new Configuration();

        FileSystem hdfs1 = FileSystem.get(URI.create(filePath1), conf);
        Path path1 = new Path(filePath1);
        FSDataInputStream in1 = hdfs1.open(path1);
        BufferedReader br1 = new BufferedReader(new InputStreamReader(in1));
        String line1;

        FileSystem hdfs2= FileSystem.get(URI.create(filePath2), conf);
        Path path2 = new Path(filePath2);
        FSDataInputStream in2 = hdfs2.open(path2);
        BufferedReader br2 = new BufferedReader(new InputStreamReader(in2));
        String line2;

        Hashtable<String, ArrayList<ArrayList<String>>> hTab = new Hashtable<String, ArrayList<ArrayList<String>>>();
        ArrayList<ArrayList<String>> value = null;
        ArrayList<String> value1 = null;
        while ((line1 = br1.readLine()) != null)
            if (line1.trim().length() > 0) {
                String[] tupleR = line1.split("\\|");
                value1 = new ArrayList<String>();
              /*for (int i = 0, j = 0; i < operations.size(); i++) {
                    op = operations.get(i);
                    if (op[0] == 1) {
                        value1.add(tupleR[op[1]]);
                        op[1] = j;
                        j++;
                        operations.set(i, op);
                    }
                }*/
                for (String tmp : tupleR) value1.add(tmp);
                value = new ArrayList<ArrayList<String>>();
                if (hTab.containsKey(tupleR[joinKeyR]))
                    for (ArrayList<String> tmp : hTab.get(tupleR[joinKeyR])) value.add(tmp);
                value.add(value1);
                hTab.put(tupleR[joinKeyR], value);
            }

        int count = 0;
        String valueStr, ckStr;
        while ((line2 = br2.readLine()) != null)
            if (line2.trim().length() > 0) {
                String[] tupleS = line2.split("\\|");
                if(hTab.containsKey(tupleS[joinKeyS])) {
                    Put put = new Put(tupleS[joinKeyS].getBytes());
                    count = table.get(new Get(tupleS[joinKeyS].getBytes())).size() / operations.size();
                    for (ArrayList<String> tupleR : hTab.get(tupleS[joinKeyS])) {
                        for (int i = 0; i < operations.size(); i++) {
                            op = operations.get(i);
                            if (count == 0) ckStr = columnKey[i];
                            else ckStr = columnKey[i] + "." + count;
                            if (op[0] == 1) valueStr = tupleR.get(op[1]);
                            else valueStr = tupleS[op[1]];
                            put.add(columnFamily.getBytes(), ckStr.getBytes(), valueStr.getBytes());
                        }
                        count++;
                    }
                    table.put(put);
                }
            }
        table.close();
        System.out.println("put successfully");
    }
}
