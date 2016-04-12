
import java.io.*;
import java.net.URI;
import java.net.URISyntaxException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;

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

import java.util.ArrayList;
import java.util.Arrays;

/**
 * This Hw1Grp1 class achieve sort-merge-join
 * @author 201528013328011
 */
public class Hw1Grp1 {

	private static String file_R;  //One of the two files address
	private static String file_S;  //another one of the two files address
	private static Integer[] join_key;  //Join key in two files
	private static Integer[] res_R;  //The names of the colomns asked by result in file_R
	private static Integer[] res_S;  //The names of the colomns asked by result in file_S
	private static ArrayList<String[]> table_R = new ArrayList<String[]>();  //The data read from file_R
	private static ArrayList<String[]> table_S = new ArrayList<String[]>();  //The data read from file_S
	private static ArrayList<String[]> join_res = new ArrayList<String[]>();  //The result after join

	/**
	 * This method is the main method of class Hw1Grp1
	 * @param args Should be provided when run the java program in order line
	 * @throws IOException                  Signals that an I/O exception of some sort has occurred. This class is the general class of exceptions produced by failed or interrupted I/O operations
	 * @throws URISyntaxException           Thrown when the URI writen with mistake(s)
	 * @throws MasterNotRunningException    Thrown if the master is not running
	 * @throws ZooKeeperConnectionException Thrown if the client can't connect to zookeeper
	 */
	public static void main(String[] args) throws IOException, URISyntaxException, MasterNotRunningException, ZooKeeperConnectionException {
		ch_args(args);
		hdfsrw(file_R, res_R, table_R);
		hdfsrw(file_S, res_S, table_S);
		System.out.println("read from hdfs successfully");
		sort_join(table_R, table_S);
		System.out.println("sort successfully");		
		hbaserw(join_res);
	}

	/**
	 * This method is used for get useful data from the param 
	 * @param or_args The param fallow the java order when run the java program in order line
	 */
	private static void ch_args(String[] or_args) {
		file_R = "hdfs://localhost:9000"+or_args[0].substring(2);
		file_S = "hdfs://localhost:9000"+or_args[1].substring(2);
		String[] join_key_tmp = or_args[2].substring(5).split("=");
		if(join_key_tmp[0].matches("R.*")) {
			int i0 = Integer.valueOf(join_key_tmp[0].substring(1));
			int i1 = Integer.valueOf(join_key_tmp[1].substring(1));
			join_key = new Integer[] {i0, i1};
		} else if(join_key_tmp[0].matches("S.*")) {
			int i0 = Integer.valueOf(join_key_tmp[1].substring(1));
			int i1 = Integer.valueOf(join_key_tmp[0].substring(1));
			join_key = new Integer[] {i1, i0};
		} else {
			System.out.println("unable tablename");
		}
		String[] res_tmp = or_args[3].substring(4).split(",");
		ArrayList<Integer> res_list_R = new ArrayList<Integer>();
		ArrayList<Integer> res_list_S = new ArrayList<Integer>();
		for(String str:res_tmp) {
			if(str.matches("R.*"))
				res_list_R.add(Integer.valueOf(str.substring(1)));
			else if (str.matches("S.*"))
				res_list_S.add(Integer.valueOf(str.substring(1)));
			else
				System.out.println("unable colname");
		}
		res_R = new Integer[res_list_R.size()+1];
		res_S = new Integer[res_list_S.size()+1];
		res_R[0] = join_key[0];
		res_S[0] = join_key[1];
		for(int i = 0; i < res_list_R.size(); i++)
			res_R[i+1] = res_list_R.get(i);
		for(int i = 0; i < res_list_S.size(); i++)
			res_S[i+1] = res_list_S.get(i);		
	}

	/**
	 * This method is used for read file form hdfs and filter colomn needed
	 * @param file  The address in hdfs of file
	 * @param res   The colomn needed
	 * @param table Used for store the data read from file
	 * @throws IOException        Signals that an I/O exception of some sort has occurred. This class is the general class of exceptions produced by failed or interrupted I/O operations
	 * @throws URISyntaxException Thrown when the URI writen with mistake(s)
	 */
	public static void hdfsrw(String file, Integer[] res, ArrayList<String[]> table) throws IOException, URISyntaxException {
		Configuration conf = new Configuration();
		FileSystem fs = FileSystem.get(URI.create(file), conf);
		Path path = new Path(file);
		FSDataInputStream in_stream = fs.open(path);
		BufferedReader in = new BufferedReader(new InputStreamReader(in_stream));
		String s;
		int l_num = 0;
		while ((s=in.readLine())!=null) {
			String[] line_sp = s.split("\\|");
			String[] line_fl = new String[res.length];
			line_fl[0] = line_sp[res[0]] + "&" +String.valueOf(l_num);
			for(int i = 1; i < line_fl.length; i++)
				line_fl[i] = line_sp[res[i]];
			table.add(line_fl);
			l_num++;
		}
		in.close();
		fs.close();
	}

	/**
	 * This method is used for joining two table by sort-merge-join arithmatic
	 * @param tableR One of the two table that is the source data to join
	 * @param tableS Another one of the two table that is the source data to join
	 */
	public static void sort_join(ArrayList<String[]> tableR, ArrayList<String[]> tableS) {
		String[] join_key_R = new String[tableR.size()];
		String[] join_key_S = new String[tableS.size()];
		for(int i = 0; i < tableR.size(); i++)
			join_key_R[i] = tableR.get(i)[0];
		for(int i = 0; i < tableS.size(); i++)
			join_key_S[i] = tableS.get(i)[0];
		Arrays.sort(join_key_R);
		Arrays.sort(join_key_S);
		String[] jksp_R;
		String[] jksp_R2;
		String[] jksp_S;
		String[] jksp_t;
		int num = 0;
		int r = 0;
		int s = 0;
		while(r < join_key_R.length && s < join_key_S.length) {
			jksp_R = join_key_R[r].split("\\&");
			jksp_S = join_key_S[s].split("\\&"); 
			if(jksp_R[0].compareTo(jksp_S[0]) < 0){
				r++;
			} else if(jksp_R[0].compareTo(jksp_S[0]) > 0) {
				s++;
			} else {
				for(int t = s; t < join_key_S.length; t++){
					jksp_t = join_key_S[t].split("\\&");
					if(jksp_S[0].equals(jksp_t[0])){
						String[] join_tmp = new String[res_S.length + res_R.length];
						for(int j = 0; j < res_R.length; j++) {
							join_tmp[j] = tableR.get(Integer.valueOf(jksp_R[1]))[j];
						}
						for(int j = res_R.length; j < res_S.length + res_R.length - 1; j++) {
							join_tmp[j] = tableS.get(Integer.valueOf(jksp_t[1]))[j + 1 - res_R.length];
						}
						join_tmp[res_S.length + res_R.length-1] = String.valueOf(num);
						num++;
						join_tmp[0] = jksp_R[0];
						join_res.add(join_tmp);
					}
				}
				r++;
				if(r < join_key_R.length){
					jksp_R2 = join_key_R[r].split("\\&");
					if(!jksp_R2[0].equals(jksp_R[0]))
						num = 0;
				}
			}
		}
	}

	/**
	 * This method is used for writing the result of joining to Hbase
	 * @param join_res The result of joining
	 * @throws MasterNotRunningException    Thrown if the master is not running
	 * @throws ZooKeeperConnectionException Thrown if the client can't connect to zookeeper
	 * @throws IOException                  Signals that an I/O exception of some sort has occurred. This class is the general class of exceptions produced by failed or interrupted I/O operations
	 */
	public static void hbaserw(ArrayList<String[]> join_res) throws MasterNotRunningException, ZooKeeperConnectionException, IOException {
		Logger.getRootLogger().setLevel(Level.WARN);
		String tableName= "Result";
		HTableDescriptor htd = new HTableDescriptor(TableName.valueOf(tableName));
		HColumnDescriptor res = new HColumnDescriptor("res");
		htd.addFamily(res);
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

		HTable table = new HTable(configuration,tableName);
		String row_key = new String();
		String col = new String();
		String value = new String();
		int len = res_R.length + res_S.length - 1;
		for(int i = 0; i < join_res.size(); i++){
			row_key = join_res.get(i)[0];
			for(int j = 1; j < res_R.length; j++){
				col = "R" + String.valueOf(res_R[j]);
				if(!join_res.get(i)[len].equals("0"))
					col = "R" + String.valueOf(res_R[j]) + "." + join_res.get(i)[len];
				value = join_res.get(i)[j];
				Put put = new Put(row_key.getBytes());
				put.add("res".getBytes(),col.getBytes(),value.getBytes());
				table.put(put);				
			}
			for(int j = res_R.length; j < res_R.length + res_S.length - 1; j++){
				col = "S" + String.valueOf(res_S[j+1-res_R.length]);
				if(!join_res.get(i)[len].equals("0"))
					col = "S" + String.valueOf(res_S[j+1-res_R.length]) + "." + join_res.get(i)[len];
				value = join_res.get(i)[j];
				Put put = new Put(row_key.getBytes());
				put.add("res".getBytes(),col.getBytes(),value.getBytes());
				table.put(put);				
			}
		}
		System.out.println("put successfully");
	}
}
