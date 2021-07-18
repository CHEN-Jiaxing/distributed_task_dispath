#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// 0. elok
// 1. shinji
// 2. stern
// 3. mentor
// 4. colleague

void send_file(int node_id, const string &file_name) {
  vector<string> node_ip = {"9.134.190.157", "9.135.155.140", "9.134.74.195", "9.134.0.139", "9.135.12.132"};
  vector<string> user = {"root", "shinji", "root", "elokwang", "root"};
  vector<string> passwd = {"asdfzxcv123$", "eva2015REI", "AufWiedersehen77", "WangElok@#1122", "La597911628!"};
  vector<string> dest = {"/root/mini/mini_project", "/home/shinji/mini/mini_project", "/root/mini/mini_project", "/home/elokwang/mini/mini_project", "/root/mini/mini_project"};

  string cmd = "./autoscp.sh " + node_ip[node_id] + " " + user[node_id] + " " + passwd[node_id] + " " + file_name + " " + dest[node_id];
  int ret = system(cmd.c_str());
  printf("scp down, return number is: %d\n", ret);
}

/*

        int target_id = atoi(cmd.substr(end+1, 1).c_str());
        string file_name = to_string(task_id) + ".csv";
        send_file(target_id, file_name);

*/