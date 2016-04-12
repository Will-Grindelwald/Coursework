
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URI;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.client.HBaseAdmin;
import org.apache.hadoop.hbase.client.HTable;
import org.apache.hadoop.hbase.client.Put;
import org.apache.hadoop.hbase.util.Bytes;

public class Hw1Grp5 {
    public static final String baseURI = "hdfs://localhost:9000";
    public static final String TABLE_NAME = "Result";
    
    private String inputFilePath;
    private int selectRow;
    private String opt;
    private double limit;
    private int[] distinctRows;
    
    public Hw1Grp5(){}
    public Hw1Grp5(String inputFilePath, int selectRow, String opt, double limit, int[] distinctRows) {
        this.inputFilePath = baseURI + inputFilePath;
        this.selectRow = selectRow;
        this.opt = opt;
        this.limit = limit;
        this.distinctRows = distinctRows;
    }
    
    //java Hw1GrpX R=<file> select:R1,gt,5.1 distinct:R2,R3,R5
    //R=/hw1/input/distinct_0.tbl select:R0,gt,5 distinct:R1
    //R=/hw1/input/distinct_1.tbl select:R1,ge,25 distinct:R2,R0
    public static void main(String[] args) {
        String inputFilePath = args[0].substring(2);
        String[] select = args[1].substring(7).split(",");
        String[] distinct = args[2].substring(9).split(",");
        
        int selectRow = parseInt(select[0]);
        String opt = select[1];
        double limit = Double.parseDouble(select[2]);
        int[] distinctRows = new int[distinct.length];
        for (int i = 0;i < distinct.length;i ++) {
            distinctRows[i] = parseInt(distinct[i]);
        }
        
        Hw1Grp5 hw1Grp5 = new Hw1Grp5(inputFilePath, selectRow, opt, limit, distinctRows);
        try {
            hw1Grp5.slove();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    public static int parseInt(String str) {
        return str.charAt(1) - '0';
    }
    
    private void createTable(String tableName, String[] columnFamilys, Configuration conf) throws Exception {
        HBaseAdmin admin = new HBaseAdmin(conf);
        if (admin.tableExists(tableName)) {
            admin.disableTable(tableName);
            admin.deleteTable(tableName);
        }
        HTableDescriptor tableDesc = new HTableDescriptor(TableName.valueOf(tableName));
        for (String columnFamily : columnFamilys) {
            tableDesc.addFamily(new HColumnDescriptor(columnFamily));
        }
        admin.createTable(tableDesc);
        admin.close();
    }
    
    public void slove() throws Exception{
        Configuration conf = new Configuration();
        //import the content of file1 to Hashmap1,and sort
        FileSystem fs = FileSystem.get(URI.create(inputFilePath), conf);
        Path path1 = new Path(inputFilePath);
        FSDataInputStream inputStream = fs.open(path1);//read the content of the path
        BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));//read the data of stream
        String line = null;
        List<String> resultList = new ArrayList<String>();
        while ((line = reader.readLine()) != null) {
            String result = parseResult(line);
            if (result.length() > 0) {
                resultList.add(result);
            }
        }
        Object[] results = resultList.toArray();
        Arrays.sort(results);
        resultList.clear();
        resultList.add(results[0].toString());
        for (int i = 1;i < results.length;i ++) {
            if (!results[i].toString().equals(results[i - 1].toString())) {
                resultList.add(results[i].toString());
            }
        }
        
        createTable(TABLE_NAME, new String[]{"res"}, conf);
        HTable table = new HTable(conf, TABLE_NAME);
        int id = 0;
        for (String result : resultList) {
            Put put = new Put(Bytes.toBytes(id ++));
            String[] items = result.split("\\|");
            for (int i = 0;i < items.length;i ++) {
                put.add(Bytes.toBytes("res"), Bytes.toBytes("R" + distinctRows[i]), Bytes.toBytes(items[i]));
            }
            //System.out.println(put.toJSON());
            table.put(put);
        }
        table.close();
    }
    
    private String parseResult(String line) {
        String[] items = line.split("\\|");
        String result = "";
        double checkPoint = Double.parseDouble(items[selectRow]);
        if (check(checkPoint)) {
            for (int row : distinctRows) {
                result += items[row] + '|';
            }
        }
        return result;
    }
    
    private boolean check(double x) {
        //(a)列,gt,值, (b)列,ge,值, (c)列,eq,值, (d)列,ne,值,(e)列,le,值, (f)列,lt,值
        if (opt.equals("gt")) return x > limit;
        if (opt.equals("ge")) return x >= limit;
        if (opt.equals("eq")) return x == limit;
        if (opt.equals("ne")) return x != limit;
        if (opt.equals("le")) return x <= limit;
        return x < limit;
    }
}

