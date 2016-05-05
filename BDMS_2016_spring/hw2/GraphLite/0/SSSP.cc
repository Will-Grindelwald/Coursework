
#include <cstdio>
#include <cstring>
#include <cmath>

#include "GraphLite.h"

#define VERTEX_CLASS_NAME(name) SSSP##name

typedef int ResType;
typedef int AggrType;

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
        m_n_value_size = sizeof(ResType);
        return m_n_value_size;
    }
    int getEdgeValueSize() {
        m_e_value_size = sizeof(ResType);
        return m_e_value_size;
    }
    int getMessageValueSize() {
        m_m_value_size = sizeof(ResType);
        return m_m_value_size;
    }
    void loadGraph() {
        unsigned long long last_vertex;
        unsigned long long from;
        unsigned long long to;
        ResType weight = 1;

        ResType value = 0;
        int outdegree = 0;

        const char *line= getEdgeLine();

        sscanf(line, "%lld %lld %d", &from, &to, &weight);
        addEdge(from, to, &weight);

        last_vertex = from;
        ++outdegree;
        for (int64_t i = 1; i < m_total_edge; ++i) {
            line= getEdgeLine();
            sscanf(line, "%lld %lld %d", &from, &to, &weight);
            if (last_vertex != from) {
                addVertex(last_vertex, &value, outdegree);
                last_vertex = from;
                outdegree = 1;
            } else ++outdegree;
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
        ResType value;

        for (ResultIterator r_iter; !r_iter.done(); r_iter.next()) {
            r_iter.getIdValue(vid, &value);
            int n = sprintf(s, "%lld: %d\n", (long long)vid, value);
            writeNextResLine(s, n);
        }
    }
};

// An aggregator that records a double value tom compute sum
class VERTEX_CLASS_NAME(Aggregator): public Aggregator<AggrType> {
public:
    void init() { }

    void* getGlobal() { return &m_global; }

    void setGlobal(const void* p) { memmove(&m_global, p, sizeof(AggrType)); }

    void* getLocal() { return &m_local; }

    void merge(const void* p) { }

    void accumulate(const void* p) { }
};

class VERTEX_CLASS_NAME(): public Vertex <ResType, ResType, ResType> {
public:
    void compute(MessageIterator* pmsgs) {
        if(getSuperstep() == 0)
            if(m_pme->m_v_id == * (AggrType *)getAggrGlobal(0)) * mutableValue() = 0;
            else * mutableValue() = 100000;
        else {
            ResType recive, min = getValue();
            for(; !pmsgs->done(); pmsgs->next())
                if((recive = pmsgs->getValue()) < min) min = recive;
            if(min == getValue()) { voteToHalt(); return; }
            * mutableValue() = min;
        }
        for(OutEdgeIterator out_iter = getOutEdgeIterator(); !out_iter.done(); out_iter.next())
            sendMessageTo(out_iter.target(), getValue() + out_iter.getValue());
    }
};

class VERTEX_CLASS_NAME(Graph): public Graph {
public:
    VERTEX_CLASS_NAME(Aggregator)* aggregator;

public:
    // argv[0]: SSSP.so
    // argv[1]: <input path>
    // argv[2]: <output path>
    // argv[3]: <source id>
    void init(int argc, char* argv[]) {

        setNumHosts(5);
        setHost(0, "localhost", 1411);
        setHost(1, "localhost", 1421);
        setHost(2, "localhost", 1431);
        setHost(3, "localhost", 1441);
        setHost(4, "localhost", 1451);

        if (argc < 4) {
           printf ("Usage: %s <input path> <output path> <sid>\n", argv[0]);
           exit(1);
        }

        m_pin_path = argv[1];
        m_pout_path = argv[2];
        AggrType sid;
        sscanf(argv[3],"%d",&sid);

        aggregator = new VERTEX_CLASS_NAME(Aggregator)[1];
        regNumAggr(1);
        regAggr(0, &aggregator[0]);

        aggregator->setGlobal(&sid);
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
