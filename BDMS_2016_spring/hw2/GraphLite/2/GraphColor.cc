/* 3, 201528013329007, Jiecheng Li */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <vector>
#include <map>

#include "GraphLite.h"

#define VERTEX_CLASS_NAME(name) GraphColor##name

typedef short ColorType;
typedef bool StatusType;
typedef struct ValueType {
    ColorType c;
    StatusType s;
} ValueType;
typedef struct MyAggr {
    int start, cm;
} MyAggr;

class VERTEX_CLASS_NAME(InputFormatter): public InputFormatter {
public:
    int64_t getVertexNum() {
        unsigned long long n;
        sscanf(m_ptotal_vertex_line, "%lld", &n);
        m_total_vertex = n;
        return m_total_vertex;
    }
    int64_t getEdgeNum() {
        unsigned long long n;
        sscanf(m_ptotal_edge_line, "%lld", &n);
        m_total_edge = n;
        return m_total_edge;
    }
    int getVertexValueSize() {
        m_n_value_size = sizeof(ValueType);
        return m_n_value_size;
    }
    int getEdgeValueSize() {
        m_e_value_size = sizeof(double);
        return m_e_value_size;
    }
    int getMessageValueSize() {
        m_m_value_size = sizeof(ValueType);
        return m_m_value_size;
    }
    void loadGraph() {
        unsigned long long last_vertex;
        unsigned long long from;
        unsigned long long to;
        double weight = 0;

        double value = 1;
        int outdegree = 0;

        const char *line = getEdgeLine();

        sscanf(line, "%lld %lld", &from, &to);
        addEdge(from, to, &weight);

        last_vertex = from;
        ++outdegree;
        for (int64_t i = 1; i < m_total_edge; ++i) {
            line = getEdgeLine();

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

class VERTEX_CLASS_NAME(OutputFormatter): public OutputFormatter {
public:
    void writeResult() {
        int64_t vid;
        char s[1024];
        ValueType value;

        for (ResultIterator r_iter; !r_iter.done(); r_iter.next()) {
            r_iter.getIdValue(vid, &value);
            int n = sprintf(s, "%lld: %d\n", (long long)vid, value.c);
            writeNextResLine(s, n);
        }
    }
};

class VERTEX_CLASS_NAME(Aggregator): public Aggregator<MyAggr> {
public:
    void init() { }
    
    void* getGlobal() { return &m_global; }

    void setGlobal(const void* p) { m_global = * (MyAggr *)p; }

    void* getLocal() { return &m_local; }

    void merge(const void* p) { }

    void accumulate(const void* p) { }
};

class VERTEX_CLASS_NAME(): public Vertex <ValueType, double, ValueType> {
public:
    void compute(MessageIterator* pmsgs) {
        ValueType val;
        MyAggr ARG = * (MyAggr *)getAggrGlobal(0);
        srand(time(0) * m_pme->m_v_id * clock());
        if(getSuperstep() == 0)              // 初始化
            if(m_pme->m_v_id == ARG.start) { val.c = 0; val.s = true; }
            else { val.c = rand() % ARG.cm; val.s = false; }
        else {
            vector<ColorType> sure;          // 邻居的已确定的颜色
            map<ColorType, int64_t> total;   // 邻居的所有颜色 <颜色，选这个颜色的最小的v_id>
            for( ; !pmsgs->done(); pmsgs->next()) {
                val = pmsgs->getValue();
                if(val.s) { sure.push_back(val.c); total[val.c] = -1; }
                else if(total[val.c] == 0 || total[val.c] > ((Msg*)pmsgs->getCurrent())->s_id)
                    total[val.c] = ((Msg*)pmsgs->getCurrent())->s_id;    // 让 total[key] 为选这个颜色的最小的v_id
            }
            val = getValue();
            if(val.s) { if(sure.size() == total.size()) { voteToHalt(); return; } /* 邻居点均已确定，则 voteToHalt */}
            else if(total.find(val.c) != total.end() && total[val.c] < m_pme->m_v_id) // 冲突且自己不是选这个颜色的最小的v_id
                while(total.find(val.c) != total.end() && total[val.c] < m_pme->m_v_id)
                    val.c = rand() % ARG.cm;                // 一直随机到一个不冲突的
            else if(sure.size() != 0) val.s = true;         // 至少周围有一点已确定, 保证单点洪泛
        }
        * mutableValue() = val;
        sendMessageToAllNeighbors(val);
    }
};

class VERTEX_CLASS_NAME(Graph): public Graph {
public:
    VERTEX_CLASS_NAME(Aggregator)* aggregator;

public:
    // argv[0]: GraphColor.so
    // argv[1]: <input path>
    // argv[2]: <output path>
    // argv[3]: <v0 id>
    // argv[4]: <color num>
    void init(int argc, char* argv[]) {

        setNumHosts(5);
        setHost(0, "localhost", 1411);
        setHost(1, "localhost", 1421);
        setHost(2, "localhost", 1431);
        setHost(3, "localhost", 1441);
        setHost(4, "localhost", 1451);

        if (argc < 5) {
            printf ("Usage: %s <input path> <output path> <v0 id> <color num>\n", argv[0]);
            exit(1);
        }

        m_pin_path = argv[1];
        m_pout_path = argv[2];
        MyAggr args;
        sscanf(argv[3],"%d",&args.start);
        sscanf(argv[4],"%d",&args.cm);

        aggregator = new VERTEX_CLASS_NAME(Aggregator)[1];
        regNumAggr(1);
        regAggr(0, &aggregator[0]);

        aggregator->setGlobal(&args);
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
