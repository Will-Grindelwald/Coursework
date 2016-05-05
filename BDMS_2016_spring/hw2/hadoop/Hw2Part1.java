
import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import java.util.StringTokenizer;
import java.text.DecimalFormat;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapred.TextInputFormat;
import org.apache.hadoop.mapred.TextOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class Hw2Part1 {

    private static class ValueType implements Writable {

        public int count;
        public double sum;

        public void write(DataOutput out) throws IOException {
            out.writeInt(count);
            out.writeDouble(sum);
        }

        public void readFields(DataInput in) throws IOException {
            count = in.readInt();
            sum = in.readDouble();
        }
    }

    // This is the Mapper class
    public static class TokenizerMapper extends Mapper<Object, Text, Text, ValueType> {

        private ValueType val = new ValueType();
        private Text src_dst = new Text();
        private static String tmp;

        public void map(Object key, Text value, Context context) throws IOException, InterruptedException {
            StringTokenizer itr = new StringTokenizer(value.toString());
            if (itr.countTokens() != 3) return;
            tmp = itr.nextToken() + " ";                      // source
            src_dst.set(tmp.getBytes("UTF-8"), 0, tmp.length());
            tmp = itr.nextToken();                            // destination
            src_dst.append(tmp.getBytes("UTF-8"), 0, tmp.length());
            try {
                val.sum = Double.valueOf(itr.nextToken());    // time
            } catch (Exception e) {
                return;
            }
            val.count = 1;
            context.write(src_dst, val);
        }
    }

    // This is the Combiner class
    public static class CntSumCombiner extends Reducer<Text, ValueType, Text, ValueType> {

        private ValueType result_value = new ValueType();

        public void reduce(Text key, Iterable<ValueType> values, Context context) throws IOException, InterruptedException {
            result_value.count = 0;
            result_value.sum = 0;
            for (ValueType val : values) {
                result_value.count += val.count;
                result_value.sum += val.sum;
            }
            context.write(key, result_value);
        }
    }

    // This is the Reducer class
    public static class CntSumReducer extends Reducer<Text, ValueType, Text, Text> {

        private Text result_text = new Text();

        public void reduce(Text key, Iterable<ValueType> values, Context context) throws IOException, InterruptedException {
            int count = 0;
            double sum = 0/*, avg = 0*/;
            for (ValueType val : values) {
                count += val.count;
                sum += val.sum;
            }
            //avg = ((int)(sum / count * 1000 + 0.5)) / 1000.0;
            String avg = new DecimalFormat("##.000").format(sum / count);
            result_text.set((count + " ").getBytes("UTF-8"), 0, (count + " ").length());
            //result_text.append((avg + "").getBytes("UTF-8"), 0, (avg + "").length());
            result_text.append(avg.getBytes("UTF-8"), 0, avg.length());
            context.write(key, result_text);
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
        if (otherArgs.length < 2) {
            System.err.println("Usage: compute <in> [<in>...] <out>");
            System.exit(2);
        }

        Job job = Job.getInstance(conf, "source-destination");
        job.setJarByClass(Hw2Part1.class);
        job.setMapperClass(TokenizerMapper.class);
        job.setCombinerClass(CntSumCombiner.class);
        job.setReducerClass(CntSumReducer.class);
        job.setMapOutputKeyClass(Text.class);
        job.setMapOutputValueClass(ValueType.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        // add the input paths as given by command line
        for (int i = 0; i < otherArgs.length - 1; ++i) {
          FileInputFormat.addInputPath(job, new Path(otherArgs[i]));
        }
        // add the output path as given by the command line
        FileOutputFormat.setOutputPath(job, new Path(otherArgs[otherArgs.length - 1]));

        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}

