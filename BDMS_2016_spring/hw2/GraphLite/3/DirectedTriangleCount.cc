/* 3, 201528013329007, Jiecheng Li */
/**
 * @file DirectedTriangleCount.cc
 * @author Jiecheng Li
 */

#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <set>

#include "GraphLite.h"

#define VERTEX_CLASS_NAME(name) DirectedTriangleCount##name

typedef struct Res {
    int through, cycle;
} Res;

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
        m_n_value_size = sizeof(Res);
        return m_n_value_size;
    }
    int getEdgeValueSize() {
        m_e_value_size = sizeof(double);
        return m_e_value_size;
    }
    int getMessageValueSize() {
        m_m_value_size = sizeof(int64_t);
        return m_m_value_size;
    }
    void loadGraph() {
        unsigned long long last_vertex;
        unsigned long long from;
        unsigned long long to;
        double weight = 0;

        Res value;
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
        Res value;

        ResultIterator r_iter;
        r_iter.getIdValue(vid, &value);
        int n = sprintf(s, "in: %d\nout: %d\nthrough: %d\ncycle: %d\n", value.through, value.through, value.through, value.cycle);
        writeNextResLine(s, n);
    }
};

class VERTEX_CLASS_NAME(Aggregator): public Aggregator<Res> {
public:
    void init() {
        memset(&m_global, 0, sizeof(Res));
        memset(&m_local, 0, sizeof(Res));
    }
    void* getGlobal() { return &m_global; }

    void setGlobal(const void* p) { memmove(&m_global, p, sizeof(Res)); }

    void* getLocal() { return &m_local; }

    void merge(const void* p) {
        m_global.through += ((Res*)p)->through;
        m_global.cycle += ((Res*)p)->cycle;
    }
    void accumulate(const void* p) {
        m_local.through += ((Res*)p)->through;
        m_local.cycle += ((Res*)p)->cycle;
    }
};

class VERTEX_CLASS_NAME(): public Vertex <Res, double, int64_t> {
public:
    void compute(MessageIterator* pmsgs) {
        if(getSuperstep() == 0) sendMessageToAllNeighbors(m_pme->m_v_id); // send outedge's src node info to neighbor
        else if(getSuperstep() == 1)
            for(; !pmsgs->done(); pmsgs->next()) {
                sendMessageToAllNeighbors(pmsgs->getValue());    // send inedge's src node info to neighbor
                sendMessageTo(m_pme->m_v_id, pmsgs->getValue()); // tell superstep 2 inedge info
            }
        else if(getSuperstep() == 2) {
            vector<int64_t> grandfathers;
            set<int64_t> fathers, kids;
            Res res_local = {0, 0};
            for(; !pmsgs->done(); pmsgs->next())
                if(((Msg*)pmsgs->getCurrent())->s_id == m_pme->m_v_id)
                    fathers.insert(pmsgs->getValue());           // establish in_list/fathers set
                else grandfathers.push_back(pmsgs->getValue());  // establish grandfathers vector
            for(auto kids_iter = getOutEdgeIterator(); !kids_iter.done(); kids_iter.next())
                kids.insert(kids_iter.target());                 // establish out_list/kids set
            for(auto iter = grandfathers.begin(); iter != grandfathers.end(); ++iter) {
                if(fathers.find(*iter) != fathers.end()) ++res_local.through;
                if(kids.find(*iter) != kids.end()) ++res_local.cycle;
            }
            accumulateAggr(0, &res_local);
        } else if(getSuperstep() == 3) {
            * mutableValue() = *(Res*)getAggrGlobal(0);
            voteToHalt();
        }
    }
};

class VERTEX_CLASS_NAME(Graph): public Graph {
public:
    VERTEX_CLASS_NAME(Aggregator)* aggregator;

public:
    // argv[0]: DirectedTriangleCount.so
    // argv[1]: <input path>
    // argv[2]: <output path>
    void init(int argc, char* argv[]) {

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
