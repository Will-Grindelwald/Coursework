
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "GraphLite.h"
//change VERTEX_CLASS_NAME(name) definition to use a different class name
#define VERTEX_CLASS_NAME(name) PageRankVertex##name

#define EPS 1e-6

//VERTEX_CLASS_NAME(InputFormatter) can be kept as is
class VERTEX_CLASS_NAME(InputFormatter): public InputFormatter {
public:
    int64_t getVertexNum() {
        unsigned long long n;
		//从m_ptotal_vertex_line读入数据存到n
        sscanf(m_ptotal_vertex_line, "%lld", &n);
        m_total_vertex= n;
        return m_total_vertex;
    }
    int64_t getEdgeNum() {
        unsigned long long n;
        sscanf(m_ptotal_edge_line, "%lld", &n);
        m_total_edge= n;
        return m_total_edge;
    }
    int getVertexValueSize() {
		//计算一个double类型的数据占用的内存字节数
        m_n_value_size = sizeof(double);
        return m_n_value_size;
    }
    int getEdgeValueSize() {
        m_e_value_size = sizeof(double);
        return m_e_value_size;
    }
    int getMessageValueSize() {
		//    int m_m_value_size;         /**< message value type size */
        m_m_value_size = sizeof(double);
        return m_m_value_size;
    }
    void loadGraph() {
        unsigned long long last_vertex;
        unsigned long long from;
        unsigned long long to;
        double weight = 0;
        
        double value = 1;
        int outdegree = 0;
        
        const char *line= getEdgeLine();
		//读入一行数据把边表示成from和to
        sscanf(line, "%lld %lld", &from, &to);
		//weight pointer of edge weight（边的权重）
		//Add one edge to Edge array.
        addEdge(from, to, &weight);

        last_vertex = from;
        ++outdegree;
		//扫描整个用来添加from相同的数据信息
        for (int64_t i = 1; i < m_total_edge; ++i) {
            line= getEdgeLine();

            // Note: modify this if an edge value is to be read
            //       modify the 'value' variable

            sscanf(line, "%lld %lld", &from, &to);
            if (last_vertex != from) {
				//添加顶点的value和出度信息。
                addVertex(last_vertex, &value, outdegree);
                last_vertex = from;
                outdegree = 1;
            } else {
                ++outdegree;
            }
            addEdge(from, to, &weight);
        }
        addVertex(last_vertex, &value, outdegree);
    }
};

//VERTEX_CLASS_NAME(OutputFormatter): this is where the output is generated
class VERTEX_CLASS_NAME(OutputFormatter): public OutputFormatter {
public:
    void writeResult() {
        int64_t vid;
        double value;
        char s[1024];

        for (ResultIterator r_iter; ! r_iter.done(); r_iter.next() ) {
			//@param vid reference of vertex id to be got
			//@param pvalue pointer of vertex value
            r_iter.getIdValue(vid, &value);
			//把vid和value写入s
			//n是返回写字符的总数，包括冒号和空格
            int n = sprintf(s, "%lld: %f\n", (unsigned long long)vid, value);
            writeNextResLine(s, n);
        }
    }
};

// An aggregator that records a double value to compute sum
//VERTEX_CLASS_NAME(Aggregator): you can implement other types of aggregation
//计算sum	
class VERTEX_CLASS_NAME(Aggregator): public Aggregator<double> {
public:
    void init() {
		//m_global为double型
        m_global = 0;
        m_local = 0;
    }
    void* getGlobal() {
        return &m_global;
    }
    void setGlobal(const void* p) {
        m_global = * (double *)p;
    }
    void* getLocal() {
        return &m_local;
    }
    void merge(const void* p) {
        m_global += * (double *)p;
    }
    void accumulate(const void* p) {
        m_local += * (double *)p;
    }
};

//VERTEX_CLASS_NAME(): the main vertex program with compute()
//template<typename VertexValue, typename EdgeValue, typename MessageValue>
class VERTEX_CLASS_NAME(): public Vertex <double, double, double> {
public:
	//@param pmsgs specialized pointer of received message iterator
    void compute(MessageIterator* pmsgs) {
        double val;
        if (getSuperstep() == 0) {
           val= 1.0;//initial value
        } else {//正常执行pagerank,计算val
			//check if converged
            if (getSuperstep() >= 2) {
				//global_val存放acc(差值)判断是否需要voteToHalt()，并不是全局val的意思
                double global_val = * (double *)getAggrGlobal(0);
                if (global_val < EPS) {
                    voteToHalt(); return;
                }
            }
			//compute pagerank
            double sum = 0;
            for ( ; ! pmsgs->done(); pmsgs->next() ) {
                sum += pmsgs->getValue();
            }
            val = 0.15 + 0.85 * sum;
			//accumulate delta pageranks
            double acc = fabs(getValue() - val);
            accumulateAggr(0, &acc);
        }
		//set new pagerank value and propagate
        * mutableValue() = val;
        const int64_t n = getOutEdgeIterator().size();
        sendMessageToAllNeighbors(val / n);
    }
};

//VERTEX_CLASS_NAME(Graph): set the running configuration here
class VERTEX_CLASS_NAME(Graph): public Graph {
public:
	//新建了一个类指针aggregator，指向VERTEX_CLASS_NAME(Aggregator)类
    VERTEX_CLASS_NAME(Aggregator)* aggregator;

public:
    // argv[0]: PageRankVertex.so
    // argv[1]: <input path>
    // argv[2]: <output path>
    void init(int argc, char* argv[]) {
	//argc is the counter of arguments
	//argv 字符串数组，用来存放指向你的字符串参数的指针数组，每一个元素指向一个参数
        setNumHosts(5);
        setHost(0, "localhost", 1411);
        setHost(1, "localhost", 1421);
        setHost(2, "localhost", 1431);
        setHost(3, "localhost", 1441);
        setHost(4, "localhost", 1451);

        if (argc < 3) {
           printf ("Usage: %s <input path> <output path>\n", argv[0]);
           exit(1);
        }

        m_pin_path = argv[1];
        m_pout_path = argv[2];
		//对类指针赋值，赋值到一个指向含有两个VERTEX_CLASS_NAME(Aggregator)[1]类的数组中。
        aggregator = new VERTEX_CLASS_NAME(Aggregator)[1];
		//新建2个指向AggregatorBase的指针
        regNumAggr(1);
		//(aggregator：集合)m_paggregator[0]= aggregator[0]，AggregatorBase** m_paggregator;    /**< pointers of AggregatorBase */
        regAggr(0, &aggregator[0]);
    }

    void term() {
        delete[] aggregator;
    }
};

/* STOP: do not change the code below. */
extern "C" Graph* create_graph() {
    Graph* pgraph = new VERTEX_CLASS_NAME(Graph);

    pgraph->m_pin_formatter = new VERTEX_CLASS_NAME(InputFormatter);
    pgraph->m_pout_formatter = new VERTEX_CLASS_NAME(OutputFormatter);
    pgraph->m_pver_base = new VERTEX_CLASS_NAME();

    return pgraph;
}

extern "C" void destroy_graph(Graph* pobject) {
	//释放指向该类的内存
    delete ( VERTEX_CLASS_NAME()* )(pobject->m_pver_base);
	
    delete ( VERTEX_CLASS_NAME(OutputFormatter)* )(pobject->m_pout_formatter);
    delete ( VERTEX_CLASS_NAME(InputFormatter)* )(pobject->m_pin_formatter);
    delete ( VERTEX_CLASS_NAME(Graph)* )pobject;
}
