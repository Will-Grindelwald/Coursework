.
├── 0                               // so easy，我写的
│   ├── Makefile
│   ├── readme
│   └── SSSP.cc
├── 1                               // 从 github 上找的，未整理，感觉不太行
│   ├── 0_201428013229024_hw2.cc
│   ├── Kcore.cc
│   ├── Makefile
│   ├── PageRankVertex(带注释版).cc // 没细看，感觉还行
│   └── readme
├── 2                               // 我写的，更好
│   ├── GraphColor.cc
│   ├── Makefile
│   └── readme
├── 2.1                             // 改的别人的，一般般吧
│   ├── Grp_Color.cc
│   ├── Makefile
│   └── readme
├── ColorTest.py                    // 大神写的 2 组的检测脚本，因为 2 组解题要用到随机，所以没有标准答案
├── 3                               // 我组，这个题没有用到 GraphLite 的迭代性质，跟其他几个组的题不太一样
│   ├── DirectedTriangleCount.cc
│   ├── Makefile
│   └── readme
├── GraphLite-0.20                  // GraphLite 框架
│   ├── bin                         // 一些脚本
│   │   ├── clean-output
│   │   ├── graphlite
│   │   ├── hash-partitioner.pl
│   │   ├── setenv
│   │   ├── start-graphlite
│   │   └── start-worker
│   ├── engine                      // engine 源码
│   │   ├── ChunkedList.h
│   │   ├── FreeList.h
│   │   ├── InputFormatter.cc
│   │   ├── main.cc
│   │   ├── Makefile
│   │   ├── Master.cc
│   │   ├── Master.h
│   │   ├── MsgBuffer.h
│   │   ├── MW.begin.proto
│   │   ├── MW.end.proto
│   │   ├── MW.nextss_start.proto
│   │   ├── Node.cc
│   │   ├── OutputFormatter.cc
│   │   ├── Receiver.cc
│   │   ├── Receiver.h
│   │   ├── Sender.cc
│   │   ├── Sender.h
│   │   ├── Utility.h
│   │   ├── WM.begin.proto
│   │   ├── WM.curss_finish.proto
│   │   ├── WM.end.proto
│   │   ├── Worker.cc
│   │   ├── Worker.h
│   │   └── WW.nodemsg_list.proto
│   ├── example                     // 框架提供的一个示例，PageRank
│   │   ├── Makefile
│   │   └── PageRankVertex.cc
│   ├── include                     // 框架的头文件
│   │   ├── Addr.h
│   │   ├── AggregatorBase.h
│   │   ├── Aggregator.h
│   │   ├── GenericArrayIterator.h
│   │   ├── GenericLinkIterator.h
│   │   ├── Graph.h
│   │   ├── GraphLite.h
│   │   ├── InputFormatter.h
│   │   ├── Node.h
│   │   ├── OutputFormatter.h
│   │   ├── VertexBase.h
│   │   └── Vertex.h
│   ├── Input                       // PageRank 示例的输入文件
│   ├── Output                      // 运行的结果输出
│   ├── Makefile
│   ├── doxygen.conf
│   ├── mainpage.dox
│   ├── LICENSE.txt
│   └── README.txt                  // Readme
├── GraphLite-0.20-doc.zip
├── part2-input                     // 本题的测试用例
├── Aggregator.txt                  // 介绍了 aggregator 的运行机制
├── test                            // 我写的自动化测试脚本
└── test README                     // 作业说明
