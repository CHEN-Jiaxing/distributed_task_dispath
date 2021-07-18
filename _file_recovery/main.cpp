#include <iostream>
#include <ctime>
#include <string>
#include <unistd.h>
// #include "recovery_csv.h"
#include "re_file.h"
#include "../_tendis_server/redis_op.h"

using std::string;
using std::to_string;

int main() {

  int task_id = 1;
  clock_t tik, tok;
  tik = clock();
  int ret = csv_recovery(task_id);
  if (ret == -1) {
    sleep(60);
    ret = csv_recovery(task_id);
  }
  //re_csv.easy_recovery();
  tok = clock();
  double time = (double)(tok-tik) / CLOCKS_PER_SEC;
  printf("the time is %f\n", time);
  return 0;

  /* int task_id = 1;
  for (int node_id = 0; node_id < 5; ++node_id) {
    redis_data rd;
    int block_id = 1;
    int max_line_num = 0;
    while (true) {
      //printf("start get blocks\n");
      string cmd = "get task" + to_string(task_id) + "_" + to_string(node_id) + "_" + to_string(block_id);
      string* block = new string(rd.redis_select(cmd));

      if (*block == "") {
        printf("!!!get failed, exit\n");
        printf("cmd: %s\n", cmd.c_str());
        break;
      }
      ++block_id;
      printf("get task%d_%d_%d succeed\n", task_id, node_id, block_id);
      
      delete block;
    }
    //printf("%d\n", max_line_num);
  } */
}