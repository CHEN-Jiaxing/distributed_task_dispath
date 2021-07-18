//
// Created by elokwang on 2021/6/17.
//

#include "read_csv.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include <unistd.h>
#include <stdlib.h>
#include <cctype>
#include "./ThreadPool.h"
#include <pthread.h>
#include "redis_op.h"

using std::endl;
using std::cout;
using std::ios;
using std::ofstream;
using std::ifstream;
using std::stringstream;
using std::to_string;


string read_csv::mPath = "";
string read_csv::mFileName = "";
string read_csv::mIpPort = "";
int read_csv::limit = 0;
int read_csv::mThreadNum = 10;
int read_csv::mTotalTask = 10;
int read_csv::mNodeId = 1;
int read_csv::mLinePerInsert = 55000;
int read_csv::mLoopNum = 50;
bool read_csv::isMemberOfTask[100] = {false};

read_csv::read_csv() {
    for (int i = 0; i < 100; ++i) {
        isMemberOfTask[i] = 0;
    }
}

read_csv::~read_csv() {
}

string read_csv::GetFileName() {
    return mFileName;
}

string read_csv::GetPath() {
    return mPath;
}

void read_csv::SetFileName(string file_name) {
    mFileName = file_name;
}

void read_csv::SetPath(string path) {
    mPath = path;
}

void read_csv::SetNodeID(int node_id) {
    mNodeId = node_id;
}

void read_csv::SetThreadNum(int thread_num) {
    mThreadNum = thread_num;
}

void read_csv::SetLinePerInsert(int line_per_insert) {
    mLinePerInsert = line_per_insert;
}

void read_csv::SetLoopNum(int loop_num) {
    mLoopNum = loop_num;
}

void read_csv::SetIpPort(string ip_port) {
    mIpPort = ip_port;
};

void read_csv::load() {
    cout << mThreadNum << endl;

    ThreadPool pool(mThreadNum);

    for (int i = 0; i < mLoopNum; ++i) {
        if (isMemberOfTask[i]) {
            pool.enqueue(insert_to_redis, i);
        }
    }
}

void read_csv::insert_to_redis(int task_id) {
    FILE *fp = fopen((mPath + mFileName).c_str(), "r");
//    printf("thread %d\n", task_id);
    char buf[200];
    int size = to_string(task_id).length();
    int index = 0;
    string str;
    str.clear();
//    redis_data rd(mIpPort.c_str());
    redis_data rd;
    string str_node_id = "_" + to_string(mNodeId) + "_";
    int block_idx = 1;
    while (fgets(buf, 200, fp)) {
        if (index == mLinePerInsert) {
            string set = "set task" + to_string(task_id) + str_node_id + to_string(block_idx) + " ";
            rd.redis_insert(set + str);
            block_idx += 1;
            index = 0;
            str.clear();
        }
        char id_str_[3];
        memset(id_str_, '\0', sizeof(id_str_));
        int end = strchr(buf, ',') - buf;
        strncpy(id_str_, buf, end);
        if (task_id == atoi(id_str_)) {
            str += (string) (buf + size);
            index++;
        }
    }
    string set = "set task" + to_string(task_id)  + str_node_id + to_string(block_idx) + " ";
    rd.redis_insert(set + str);
    printf("one task done!");
}

void read_csv::FindTotalTaskNum() {
//    char *buf = new char[200];
    char buf[200];
    FILE *fp = fopen((mPath + mFileName).c_str(), "r");
    while (fgets(buf, 200, fp)) {
        string str = buf;
        int end = str.find(",");
        isMemberOfTask[atoi(str.substr(0, end).c_str())] = true;
    }

}

