#include <cstdio>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <ctime>
#include "hiredis-vip/hircluster.h"
// #include "hiredis/hiredis.h"
#include "redis_op.h"
#include "ThreadPool.h"
#include "read_csv.h"
#include "../_zk_server/clust_comm.h"
#include "../_zk_server/clust_comm.cpp"

using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::map;
using std::vector;


int main() {
    //传入参数 path, file_name, thread_num, node_id, lines_per_insert, loop_num, conf_file
    string path = "./data/";
    string file_name = "test2.csv";
    int thread_num = 10;
    int node_id = 1;
    int lines_per_insert = 1000;
    int loop_num = 20; //预估最大task值;适用taskid不连续；不能大于100
    string conf_file = "conf.txt";

    std::ifstream in(conf_file, std::ios::in);
    string ip_port_conf;
    in >> ip_port_conf;
    in.close();
    cout << ip_port_conf << endl;

    read_csv readCsv;
    read_csv::SetPath(path);
    read_csv::SetFileName(file_name);
    read_csv::SetNodeID(node_id);
    read_csv::SetThreadNum(thread_num);
    read_csv::SetLoopNum(loop_num);
    read_csv::SetLinePerInsert(lines_per_insert);
    read_csv::SetIpPort(ip_port_conf);
    read_csv::FindTotalTaskNum();

    readCsv.load();
//    for(int i=0; i<read_csv::mLoopNum; ++i){
//        if(read_csv::isMemberOfTask[i]){
//            string tmp_str = to_string(i);
//            insr2clst(tmp_str.c_str());
//        }
//    }
    return 0;
}


