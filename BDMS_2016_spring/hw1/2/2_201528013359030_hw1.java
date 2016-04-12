// > Last Update:   2016-04-08 20:19:07
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.Hashtable;
import java.util.Map.Entry;
import java.util.ArrayList;

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

class Value {
    int count;
    ArrayList<Double> result = new ArrayList<Double>();
}

public class Hw1Grp2 {

    public static void main(String[] args) throws IOException, URISyntaxException, MasterNotRunningException, ZooKeeperConnectionException {

        //parameter operation
        String filePath = "hdfs://localhost:9000" + args[0].substring(2);
        int indexR = args[1].indexOf("R");
        int groupbyKey = Integer.valueOf(args[1].substring(indexR + 1));
        int indexRes = args[2].indexOf(":");
        String columnFamily = args[2].substring(0, indexRes);
        String[] columnKey = args[2].substring(indexRes + 1).split(",");
        ArrayList<int[]> operations = new ArrayList<int[]>();
        int[] op = null;
        for (String str : columnKey) {
            op = new int[2];            // op[0] for operation type, op[1] for operand
            indexR = str.indexOf("R");
            indexRes = str.indexOf(")");
            if (str.substring(0, 5).equals("count"))    op[0] = 1;
            else if (str.substring(0, 3).equals("avg")) op[0] = 2;
            else if (str.substring(0, 3).equals("max")) op[0] = 3;  // also for "min", "sum"
            else {
                System.out.println("parameter error!");
                System.exit(0);
            }
            if (indexR < indexRes) op[1] = Integer.valueOf(str.substring(indexR + 1, indexRes));
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

        //read the file
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(filePath), conf);
        Path path = new Path(filePath);
        FSDataInputStream in = fs.open(path);
        BufferedReader br = new BufferedReader(new InputStreamReader(in));
        String line;
        Hashtable<String, Value> hTab = new Hashtable<String, Value>();
        while ((line = br.readLine()) != null) {
            if (line.trim().length() > 0) {
                String[] tuple = line.split("\\|");
                Value val = new Value();
                if (hTab.containsKey(tuple[groupbyKey])) {
                    val.count = hTab.get(tuple[groupbyKey]).count + 1;
                    for (int i = 0; i < operations.size(); i++) {
                        op = operations.get(i);
                        switch(op[0]){
                        case 1: val.result.add(1.0); break;
                        case 2: val.result.add(hTab.get(tuple[groupbyKey]).result.get(i) + Double.valueOf(tuple[op[1]])); break;
                        case 3: val.result.add(hTab.get(tuple[groupbyKey]).result.get(i) > Double.valueOf(tuple[op[1]]) ? hTab.get(tuple[groupbyKey]).result.get(i) : Double.valueOf(tuple[op[1]]));
                        }
                    }
                } else {
                    val.count = 1;
                    for (int i = 0; i < operations.size(); i++) {
                        op = operations.get(i);
                        switch(op[0]){
                        case 1: val.result.add(1.0); break;
                        case 2: 
                        case 3: val.result.add(Double.valueOf(tuple[op[1]]));
                        }
                    }
                }
                hTab.put(tuple[groupbyKey], val);
            }
        }
        br.close();
        fs.close();

        for (Entry<String, Value> entry : hTab.entrySet()) {
            Put put = new Put(entry.getKey().getBytes());
            for (int i = 0; i < operations.size(); i++) {
                op = operations.get(i);
                switch(op[0]){
                case 1: put.add(columnFamily.getBytes(), columnKey[i].getBytes(), (entry.getValue().count + "").getBytes());
                        break;
                case 2: put.add(columnFamily.getBytes(), columnKey[i].getBytes(), (((int)(entry.getValue().result.get(i) / entry.getValue().count * 100)) / 100.0 + "").getBytes()); break;
                case 3: put.add(columnFamily.getBytes(), columnKey[i].getBytes(), (entry.getValue().result.get(i) + "").getBytes());
                }
            }
            table.put(put);
        }
        table.close();
        System.out.println("put successfully");
    }
}

