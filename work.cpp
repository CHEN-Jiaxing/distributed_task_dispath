#include <string>
#include <vector>
#include <random>
#include "./_zk_server/clust_comm.h"
#include "./_zk_server/clust_comm.cpp"
#include "./_file_recovery/re_file.h"
#include "./_tendis_server/redis_op.h"
#include "_read_csv/read_csv.h"
#include "_auto_scp/passwd_free_scp.h"

#define PROCS_NUM 0

int node_id;

using std::vector;
using std::string;
using std::to_string;

void read_file(int node_id) {
    //??????? path, file_name, thread_num, node_id, lines_per_insert, loop_num, conf_file
    string path = "./";
//    string file_name = "test.csv";
    string file_name = "node_" + to_string(node_id) + ".csv";
    int thread_num = 10;
    int lines_per_insert = 1000;
    int loop_num = 20; //??????task?;????taskid???????????????100
    read_csv readCsv;
    read_csv::SetPath(path);
    read_csv::SetFileName(file_name);
    read_csv::SetNodeID(node_id);
    read_csv::SetThreadNum(thread_num);
    read_csv::SetLoopNum(loop_num);
    read_csv::SetLinePerInsert(lines_per_insert);
    readCsv.FindTotalTaskNum();

    readCsv.load();
    pre_clst();
    for (int i = 1; i <= read_csv::mLoopNum; ++i) {
        if (read_csv::isMemberOfTask[i]) {
            string tmp_str = to_string(i);
            insr2clst(tmp_str.c_str());
        }
    }
    fin_clst();
}

void work(int node_id) {
    g_mode = MODE_WORKER;
    //char host[512] = "127.0.0.1:2181,127.0.0.1:2182,127.0.0.1:2183";
    char host[512] = "9.135.12.132:2181,9.134.74.195:2181,9.135.155.140:2181,9.134.190.157:2181,9.134.0.139:2181";
    zhandle_t *zkhandle = NULL;
    DEBUG();

    INIT(host, &zkhandle);

    PREWORK(zkhandle);

    bool flag = true;

    while (1) {

        usleep(100);

        if (g_mode == MODE_MONITOR) {
//            if (flag) {
//                cout << "=================" << "master" << getpid() << "=================" << endl;
//              flag = false;
//            }

            usleep(1);

        } else {

            // ????????????????
            int task_id = 0;
            int beg = 0, end = 100;
            string cmd = recv_work(zkhandle);
            if (cmd[0] == 'w') {
                sleep(10);
                cout << "=================" << cmd << "=================" << endl;
                beg = cmd.find("_");
                end = cmd.rfind("_");
                task_id = atoi(cmd.substr(beg + 1, end - beg - 1).c_str());
            } else {
                cout << "================= cmd error =================" << endl;
                cout << "=================" << cmd << "=================" << endl;
                sleep(rand() % 5 + 5);
            }

            // ??????
            if (task_id > 0) {
                cout << "***************************" << endl;
                cout << "*******" << getpid() << "********" << endl;
                cout << cmd.substr(beg + 1, end - beg - 1).c_str() << endl;
                cout << "***************************" << endl;
                pre_clst();
                st(cmd.substr(beg + 1, end - beg - 1).c_str(), "y");


                int ret = csv_recovery(task_id);

                if (ret == -1) {
                    sleep(60);
                    ret = csv_recovery(task_id);
                }

                if (ret == 0) { // ret ? 0 ????????????
                    system("./deltask.sh");
                    st(cmd.substr(beg + 1, end - beg - 1).c_str(), "g");
                    int target_id = atoi(cmd.substr(end + 1, 1).c_str());
                    string file_name = to_string(task_id) + ".csv";
                    send_file(target_id, file_name);
                    set_wk_st(zkhandle);
                }
                fin_clst();
                sleep(rand() % 5 + 5);
            } else if (task_id == -1) {
                printf("files have been recovered!\n");
                break;
            }

            /* vector<string> tmp = get_workers(zkhandle);
            for(auto s : tmp) {
              printf("%s\n", s.c_str());
            } */
        }
    }
    cout << "=================dddddddd=================" << endl;
    CLOSE(zkhandle);
    cout << "=================aaaaaaaa=================" << endl;
}

void insrt(int node_id) {
    bool flag = false;
    redis_data rd;
    string cmd = "get node_status" + to_string(node_id);
    string ret = rd.redis_select(cmd);
    if (ret == "ok") {
        flag = true;
    } else {
        read_file(node_id);
        string cmd = "set node_status" + to_string(node_id) + " ok";
        rd.redis_insert(cmd);
    }

    do {
//        cout << "~~~~" << endl;
        bool vec[5] = {false};
        for (int i = 0; i < 5; ++i) {
            string cmd = "get node_status" + to_string(i);
            string ret = rd.redis_select(cmd);
            if (ret == "ok") {
                vec[i] = true;
            }
            flag = vec[0] && vec[1] && vec[2] && vec[3] && vec[4];
        }
    } while (!flag);
    cout << "2 " << endl;
}

void work() {
    for (int i = 0; i < PROCS_NUM; ++i) {
        int res = fork();
        sleep(3);
        if (res == 0) {
            cout << "fork" << i << endl;
            work(node_id);
        }
    }
//  cout << "1234" << endl;
    work(node_id);
//  cout << "0000" << endl;
}

int main(int argc, char **argv) {
    node_id = atoi(argv[1]);
    // cout << "=================" << node_id << "=================" << endl;
    insrt(node_id);
    work();
}