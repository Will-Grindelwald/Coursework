#coding=utf-8
# 大规模作业二Part2,Color组测试脚本
# @Author Qero Lai
# @Email qingquanlai@foxmail.com qingquan@zkjinshi.com
# @Version 1.0.0
#
# usage： python ColorTest.py <your homework outpath> <graph file>
# example : python ColorTest.py outpath input/Color-graph0
#
# 免责声明：
# 本脚本只能简单地检测答案，其正确性和完备性尚未确定，请勿作为您的作业的最终评价标准。
# 若由于使用此程序而导致您最终的成绩出现问题，本人深感抱歉，但本人概不负责。

import os
import sys

def main():
    out_path = sys.argv[1]
    graph_file = sys.argv[2]
    point2color = {}

    for pnames, dnames, filenames in os.walk(out_path):
        for filename in filenames :
            infile = open(os.path.join(pnames, filename))
            for line in infile :
                parameter = line.split(": ")
                point2color[parameter[0]] = parameter[1]

    graph_file = open(graph_file)
    line_num = 0

    for line in graph_file :
        line_num = line_num + 1
        parameter = line.split(" ")
        if len(parameter) < 2:
            continue
        if point2color[parameter[0]] == point2color[parameter[1]] :
            print "Error in graph file %d line : ponit %s and point %s have the same color %s" % \
                (line_num, parameter[0], parameter[1], point2color[parameter[0]])


if __name__ == '__main__':
    main()
