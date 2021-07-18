//
// Created by elokwang on 2021/6/17.
//

#ifndef READCSV_READ_CSV_H
#define READCSV_READ_CSV_H

#include <string>
#include "../_tendis_server/redis_op.h"


using std::string;

struct read_split_block_param{
    unsigned int begin;
    unsigned int limit;
};

class read_csv {
public:
    static string mPath;
    static string mFileName;

    static int limit;
    static void insert_to_redis(int task_id);
    static int mThreadNum;
    static int mTotalTask;
    static int mNodeId;
    static int mLinePerInsert;
    static int mLoopNum;
    static bool isMemberOfTask[100];
    static string mIpPort;
    static void SetFileName(string file_name);
    static void SetPath(string path);
    static void SetNodeID(int node_id);
    static void SetLinePerInsert(int line_per_insert);
    static void SetThreadNum(int thread_num);
    static void FindTotalTaskNum();
    static void SetLoopNum(int loop_num);
    static void SetIpPort(string ip_port);

    string GetFileName();
    string GetPath();

    read_csv();
    ~read_csv();

    void load();


};


#endif //READCSV_READ_CSV_H
