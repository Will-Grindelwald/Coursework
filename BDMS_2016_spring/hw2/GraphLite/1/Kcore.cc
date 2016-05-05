
﻿#include <cstdio>
#include <cstring>
#include <cmath>
#include <iostream>
#include <array>
#include <list>

#include "GraphLite.h"

#define VERTEX_CLASS_NAME(name) Kcore##name

#define EPS 1e-6
int Kcore; // 定义全局的K

// VERTEX_CLASS_NAME(InputFormatter) can be kept as is
class VERTEX_CLASS_NAME(InputFormatter): public InputFormatter {
public:
    int64_t getVertexNum() {
        unsigned long long n;
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
        m_n_value_size = sizeof(double);
        return m_n_value_size;
    }
    int getEdgeValueSize() {
        m_e_value_size = sizeof(double);
        return m_e_value_size;
    }
    int getMessageValueSize() {
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
        sscanf(line, "%lld %lld", &from, &to);
        addEdge(from, to, &weight);

        last_vertex = from;
        ++outdegree;
        for (int64_t i = 1; i < m_total_edge; ++i) {
            line= getEdgeLine();

            // Note: modify this if an edge value is to be read
            //       modify the 'value' variable

            sscanf(line, "%lld %lld", &from, &to);
            if (last_vertex != from) {
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

// VERTEX_CLASS_NAME(OutputFormatter): this is where the output is generated
class VERTEX_CLASS_NAME(OutputFormatter): public OutputFormatter {
public:
    void writeResult() {
        int64_t vid;
        int64_t value;
        char s[1024];

        for (ResultIterator r_iter; ! r_iter.done(); r_iter.next() ) {
			// @param vid reference of vertex id to be got
			// @param pvalue pointer of vertex value
            r_iter.getIdValue(vid, &value); // 此处value取值问题
			// 判断节点value[0]是否为1，如果为1，writeNextResLine(s, n);
			if (value[0]==1){
				int n = sprintf(s, "%d\n", (int)vid);
				writeNextResLine(s, n);
			}
        }
    }
};


// K Core 不需要Aggregator，所以没有更改，也没有进行配置
// An aggregator that records a double value to compute sum
// VERTEX_CLASS_NAME(Aggregator): you can implement other types of aggregation
class VERTEX_CLASS_NAME(Aggregator): public Aggregator<double> {
public:
    void init() {
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
class VERTEX_CLASS_NAME(): public Vertex <int[2],double,int> {
	// 此处VertexValue为数组，MessageValue为数组（定义为整数也可以，只需要存放1，表示告诉其邻居该顶点已删除）
public:
	// int val[2];#val[0]存放该顶点是否被删除，[1]存放current_degree
	// @param pmsgs specialized pointer of received message iterator
    void compute(MessageIterator* pmsgs) {
        int val[2];
        if (getSuperstep= 0) {
			// 初始化所有顶点val[0]=1代表没删除、0代表删除了该顶点
			val[0]=1,val[1]=获得class Node里的 m_out_degree;
		if (getSuperstep>1)
		{	printf("debug info:msg value %d\n",(int)pmsgs->getVertexId()); // 调试时打印顶点value[0],和value[1]curren_degree信息
			for ( ;! pmsgs->done(); pmsgs->next() ) 
			{
				val[1]-=pmsgs->getValue();} // getValue返回MessageValue并对该顶点计算K
				if(val[1]<Kcore){val[0]=0}
				if(val[0]==0){
					int val_to_Others=1; // 告诉邻居该顶点已被删除
					sendMessageToAllNeighbors(val_to_Others);
					voteToHalt();break/*还是return*/;}
		}
		* mutableValue() = val[]; // 更新顶点的val[]信息。	
		
        printf("debug info:VertexValue： %d\n",(int)pmsgs->getVertexId()); // 调试时打印顶点value[0],和value[1]curren_degree信息
       
       
    }
};



// VERTEX_CLASS_NAME(Graph): set the running configuration here
class VERTEX_CLASS_NAME(Graph): public Graph {
public:
	
    VERTEX_CLASS_NAME(Aggregator)* aggregator;

public:
    // argv[0]: PageRankVertex.so
    // argv[1]: <input path>
    // argv[2]: <output path>
	// argv[3]: Kcore
    void init(int argc, char* argv[]) {
	// argc is the counter of arguments
	
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
		Kcore=argv[3];
		// Kcore不需要aggregator所以此处无所谓，以下三行可以删去。
        aggregator = new VERTEX_CLASS_NAME(Aggregator)[1];
		
        regNumAggr(1);
		
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
    delete ( VERTEX_CLASS_NAME()* )(pobject->m_pver_base);
    delete ( VERTEX_CLASS_NAME(OutputFormatter)* )(pobject->m_pout_formatter);
    delete ( VERTEX_CLASS_NAME(InputFormatter)* )(pobject->m_pin_formatter);
    delete ( VERTEX_CLASS_NAME(Graph)* )pobject;
}

