
// 实现了Nested Loop Join

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.FSDataInputStream;
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

public class Hw1GrpX {

    /**
     * @param args
     */
    public static void main(String[] args) throws IOException, URISyntaxException ,MasterNotRunningException, ZooKeeperConnectionException {
        // TODO Auto-generated method stub
        String filePath1 = "hdfs://localhost:9000/user/wpy" + args[0].substring(2);
        String filePath2 = "hdfs://localhost:9000/user/wpy" + args[1].substring(2);

        Configuration conf = new Configuration();

        FileSystem hdfs1 = FileSystem.get(URI.create(filePath1), conf);
        Path path1 = new Path(filePath1);
        FSDataInputStream in1 = hdfs1.open(path1);
        BufferedReader br1 = new BufferedReader(new InputStreamReader(in1));
        String line1;
        ArrayList<ArrayList<String>> listList1 = new ArrayList<ArrayList<String>>();
        ArrayList<String> al1 = null;
        while ((line1 = br1.readLine()) != null) {
            if (line1.trim().length() > 0) {
                String str1[] = line1.split("\\|");
                al1 = new ArrayList<String>();
                for (String s1 : str1) {
                    al1.add(s1);
                }
            }
            listList1.add(al1);
        }

        FileSystem hdfs2= FileSystem.get(URI.create(filePath2), conf);
        Path path2 = new Path(filePath2);
        FSDataInputStream in2 = hdfs2.open(path2);
        BufferedReader br2 = new BufferedReader(new InputStreamReader(in2));
        String line2;
        ArrayList<ArrayList<String>> listList2 = new ArrayList<ArrayList<String>>();
        ArrayList<String> al2 = null;
        while ((line2 = br2.readLine()) != null) {
            if (line2.trim().length() > 0) {
                String str2[] = line2.split("\\|");
                al2 = new ArrayList<String>();
                for (String s2 : str2) {
                    al2.add(s2);
                }
            }
            listList2.add(al2);
        }

        // create table descriptor
        String tableName = "Result";
        HTableDescriptor htd = new HTableDescriptor(TableName.valueOf(tableName));
        // create column descriptor
        int e=args[3].indexOf(":");
        HColumnDescriptor cf = new HColumnDescriptor(args[3].substring(0, e));
        htd.addFamily(cf);
        // configure HBase
        Configuration configuration = HBaseConfiguration.create();
        HBaseAdmin hAdmin = new HBaseAdmin(configuration);
        if (hAdmin.tableExists(tableName)) {
             System.out.println("table already exists!");
        } else {
            hAdmin.createTable(htd);
            System.out.println("table create success!");
        }
        hAdmin.close();

        HTable table = new HTable(configuration, tableName);

        int x=args[2].indexOf("R");
        int y=args[2].indexOf("=");
        int z=args[2].indexOf("S");
        int m=0;
        int n=0;
        if (y > x) {
            m=Integer.valueOf(args[2].substring(x+1, y));
            n=Integer.valueOf(args[2].substring(z+1, args[2].length()));
        } else if (y > z) {
            n=Integer.valueOf(args[2].substring(z+1,y));
            m=Integer.valueOf(args[2].substring(x+1, args[2].length()));
        }
        String s=args[3].substring(e+1);
        String[]arrString=s.split(",");

        for (ArrayList<String> arr1 : listList1) {
            for (ArrayList<String> arr2 : listList2) {
                if (arr1.get(m).equals(arr2.get(n))) {
                    Put put = new Put(arr1.get(m).getBytes());
                    for(String u:arrString){
                        put.add(args[3].substring(0, e).getBytes(), u.getBytes(), arr1.get(Integer.valueOf(u.substring(1))).getBytes());
                    }
                    table.put(put);
                }
            }
        }
        table.close();
        System.out.println("put successfully");
    }
}

