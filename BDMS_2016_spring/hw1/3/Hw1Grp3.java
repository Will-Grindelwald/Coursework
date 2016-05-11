// > Last Update:   2016-04-07 10:30:56
// > Created Time:  2016-03-27 09:37:44

import java.io.*;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

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

/**
 * This Hw1Grp3 class achieve sort-group
 * @author 201528013329007
 */
public class Hw1Grp3 {

    private static int groupbyKey;              // group by key
    private static String columnFamily;         // the string of column family
    private static String[] columnKey;          // the string list of column key
    private static ArrayList<int[]> operations; // the list of operation type and operand
    private static double[] result;             // the result of operation
    private static int count;                   // for count and average
    private static String tableName = "Result"; // HBase table's name
    private static ArrayList<ArrayList<String>> listList; // the list of tuple from a line of object file
    private static HTable table;                // HBase table
    
    /**
     * The main method of class Hw1Grp3
     * @param args should be provided when run the java program in command line
     * @throws IOException                  Signals that an I/O exception of some sort has occurred. This class is the general class of exceptions produced by failed or interrupted I/O operations
     * @throws URISyntaxException           Thrown when the URI writen with mistake(s)
     * @throws MasterNotRunningException    Thrown if the master is not running
     * @throws ZooKeeperConnectionException Thrown if the client can't connect to zookeeper
     */
    public static void main(String[] args) throws IOException, URISyntaxException, MasterNotRunningException, ZooKeeperConnectionException {

        // 1.parameter operation
        String filePath = "hdfs://localhost:9000" + args[0].substring(2);
        int indexR = args[1].indexOf("R");
        groupbyKey = Integer.valueOf(args[1].substring(indexR + 1));
        int indexRes = args[2].indexOf(":");
        columnFamily = args[2].substring(0, indexRes);
        columnKey = args[2].substring(indexRes + 1).split(",");
        operations = new ArrayList<int[]>();
        int[] op = null;
        for (String str : columnKey) {
            op = new int[2];            // op[0] for operation type, op[1] for operand
            indexR = str.indexOf("R");
            indexRes = str.indexOf(")");
            if (str.substring(0, 5).equals("count"))    op[0] = 1;
            else if (str.substring(0, 3).equals("avg")) op[0] = 2;
            else if (str.substring(0, 3).equals("max")) op[0] = 3;
            else {                      // also for "min", "sum"
                System.out.println("parameter error!");
                System.exit(0);
            }
            if (indexR < indexRes) op[1] = Integer.valueOf(str.substring(indexR + 1, indexRes));
            operations.add(op);
        }
        result = new double[operations.size()];
        Arrays.fill(result, 0);
        count = 0;
        //System.out.println("groupbyKey = " + groupbyKey);
        //for (int[] opt : operations) System.out.println("operation = " + opt[0] + "     object = " + opt[1]);

        // 2.create the HBase table
        HTableDescriptor htd = new HTableDescriptor(TableName.valueOf(tableName)); // create table descriptor
        htd.addFamily(new HColumnDescriptor(columnFamily));                        // create column descriptor
        Configuration configuration = HBaseConfiguration.create();                 // configure HBase
        HBaseAdmin hAdmin = new HBaseAdmin(configuration);
        if (hAdmin.tableExists(tableName)) {
            hAdmin.disableTable(tableName);
            hAdmin.deleteTable(tableName);
        }
        hAdmin.createTable(htd);
        hAdmin.close();
        table = new HTable(configuration, tableName);

        // 3.read the file
        FileSystem fs = FileSystem.get(URI.create(filePath), new Configuration());
        FSDataInputStream in = fs.open(new Path(filePath));
        BufferedReader br = new BufferedReader(new InputStreamReader(in));
        String line;
        listList = new ArrayList<ArrayList<String>>();
        ArrayList<String> al = null;
        while ((line = br.readLine()) != null) {
            if (line.trim().length() > 0) {
                String[] str = line.split("\\|");
                al = new ArrayList<String>();
                for (String s : str) al.add(s);
            }
            listList.add(al);
        }
        br.close();
        fs.close();

        // 4.data processing
        Collections.sort(listList, new Comparator<ArrayList<String>>() {
            @Override
            public int compare(ArrayList<String> o1, ArrayList<String> o2) {
                return o1.get(groupbyKey).compareTo(o2.get(groupbyKey));
            }
        });
        String tmp = listList.get(0).get(groupbyKey);
        for (ArrayList<String> tuple : listList) {
            if (!tmp.equals(tuple.get(groupbyKey))) {
                resultPut(tmp);
                tmp = tuple.get(groupbyKey);
                Arrays.fill(result, 0);
                count = 0;
            }
            count++;
            for (int i = 0; i < operations.size(); i++) {
                op = operations.get(i);
                switch(op[0]){
                case 2: result[i] += Double.valueOf(tuple.get(op[1])); break;
                case 3: if (result[i] < Double.valueOf(tuple.get(op[1]))) result[i] = Double.valueOf(tuple.get(op[1]));
                }
            }
        }
        resultPut(tmp);

        table.close();
        //System.out.println("succeed!");
    }

    /**
     * This method is used to put result data to the HBase table 
     * @param rowKey is the row key of a put
     */
    private static void resultPut (String rowKey) throws IOException {
        int[] op;
        Put put = new Put(rowKey.getBytes());
        for (int i = 0; i < operations.size(); i++) {
            op = operations.get(i);
            switch(op[0]){
            case 1: put.add(columnFamily.getBytes(), columnKey[i].getBytes(), (count + "").getBytes()); break;
            case 2: result[i] = ((int)(result[i] / count * 100 + 0.5)) / 100.0;
            case 3: put.add(columnFamily.getBytes(), columnKey[i].getBytes(), (result[i] + "").getBytes());
            }
        }
        table.put(put);
    }
}
