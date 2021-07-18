#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include "re_file.h"
#include "../_tendis_server/redis_op.h"
#include "ThreadPool.h"

#define MAX_BLOCK_SIZE 55000
#define MAX_LINE_SIZE 40
#define NODE_NUM 5

using std::to_string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::sort;
using std::future;

int csv_recovery(int task_id) {
  vector<int> line_nums(5);
  {
    ThreadPool pool(5);
    for (int node_id = 0; node_id < 5; ++node_id) {
      auto result = pool.enqueue(create_block, task_id, node_id);
      line_nums[node_id] = result.get();
      if (line_nums[node_id] == -1) return -1;
    }
  }

  sort(line_nums.begin(), line_nums.end());
  int max_line = line_nums.back();
  printf("%d\n", max_line);

  sort_file(task_id, max_line);
  
  return 0;
}

int create_block(int task_id, int node_id) {
  printf("Thread %d start!\n", node_id);
  vector<int> block_num = sort_blocks(task_id, node_id);
  if (block_num[1] == -1) return -1;
  sort_nodes(task_id, node_id, block_num[0]);
  return block_num[1];
}

vector<int> sort_blocks(int task_id, int node_id) {
  redis_data rd;
  int block_id = 1;
  int max_line_num = 0;
  while (true) {
    //printf("start get blocks\n");
    string cmd = "get task" + to_string(task_id) + "_" + to_string(node_id) + "_" + to_string(block_id);
    string* block = new string(rd.redis_select(cmd));

    if (*block == "") break;
    if (*block == "error") return vector<int>{-1, -1};
    
    char **buf;
    buf = new char *[MAX_BLOCK_SIZE];
    for(int i = 0; i < MAX_BLOCK_SIZE; ++i) {
      buf[i] = new char[MAX_LINE_SIZE];
    }
    int beg = 0, end = 0;
    int idx = 0;
    while (true) {
      end = block->find("\n", beg);
      
      if (end == string::npos) break;
      
      strcpy(buf[idx], block->substr(beg, end-beg+1).c_str());
      ++idx;
      beg = end + 1;
    }
    delete block;
    
    vector<pair<int, char*>> buf_vec;
    for (int i = 0; i < idx; ++i) {
      char line_num_str[20];
      memset(line_num_str, '\0', sizeof(line_num_str));
      strncpy(line_num_str, buf[i] + 1, strchr(buf[i]+1, ',') - buf[i] - 1);
      int line_num = atoi(line_num_str);
      buf_vec.emplace_back(pair<int, char*>(line_num, buf[i]));
    }
    sort(buf_vec.begin(), buf_vec.end(), [](const pair<int, char*>& lhs, 
                                            const pair<int, char*>& rhs) {
                                              return lhs.first < rhs.first;
                                            });
    string file_name = "task" + to_string(task_id) + "_" + to_string(node_id) + "_" + to_string(block_id);
    FILE *fp = fopen(file_name.c_str(), "w");
    for (auto line : buf_vec) {
      fputs(line.second, fp);
    }

    char* last_line = buf_vec.back().second;
    //if (node_id == 0) printf("%s", last_line);
    //int last = buf_vec.back().first;
    char line_num_str[20];
    memset(line_num_str, '\0', sizeof(line_num_str));
    strncpy(line_num_str, last_line + 1, strchr(last_line+1, ',') - last_line - 1);
    int line_num = atoi(line_num_str);
    
    if (line_num > max_line_num) max_line_num = line_num;
    
    ++block_id;
    // delete[] buf;
    for(int i = 0; i < MAX_BLOCK_SIZE; i ++)
      delete[] buf[i];
    delete[] buf;
    fclose(fp);
  }
  //printf("%d\n", max_line_num);
  return vector<int> {block_id - 1, max_line_num};
}

void sort_nodes(int task_id, int node_id, int block_num) {
  //printf("block number is: %d\n", block_num);
  FILE* fp[block_num];
  for (int block_id = 1; block_id <= block_num; ++block_id) {
    string file_name = "task" + to_string(task_id) + "_" + to_string(node_id) + "_" + to_string(block_id);
    fp[block_id-1] = fopen(file_name.c_str(), "r");
  }
  char **buf;
  buf = new char *[block_num];
  for(int i = 0; i < block_num; ++i) {
    buf[i] = new char[MAX_LINE_SIZE];
  }
  // char *buf[MAX_LINE_SIZE] = new char[block_num][MAX_LINE_SIZE];
  for (int i = 0; i < block_num; ++i) {
    fgets(buf[i], MAX_LINE_SIZE, fp[i]); 
  }
  auto cmp = [] (const pair<int, int>& lhs, const pair<int, int>& rhs) {
                  return lhs.second > rhs.second;
                };
  
  // 优先级队列中元素为<block_id, line_num>
  priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> pri_queue(cmp);
  for (int i = 0; i < block_num; ++i) {
    char line_num_str[20];
    memset(line_num_str, '\0', sizeof(line_num_str));
    strncpy(line_num_str, buf[i] + 1, strchr(buf[i]+1, ',') - buf[i] - 1);
    int line_num = atoi(line_num_str);
    pri_queue.push(pair<int, int>(i, line_num));
  }
  
  string file_name = "task" + to_string(task_id) + "_" + to_string(node_id);
  FILE *fp_out = fopen(file_name.c_str(), "w");
  while (!pri_queue.empty()) {
    int block_idx = pri_queue.top().first;
    fputs(buf[block_idx], fp_out);
    pri_queue.pop();
    if (fgets(buf[block_idx], MAX_LINE_SIZE, fp[block_idx])) {
      char line_num_str[20];
      memset(line_num_str, '\0', sizeof(line_num_str));
      strncpy(line_num_str, buf[block_idx] + 1, strchr(buf[block_idx]+1, ',') - buf[block_idx] - 1);
      int line_num = atoi(line_num_str);
      pri_queue.push(pair<int, int>(block_idx, line_num));
    }
  }
  for(int i = 0; i < block_num; i ++)
    delete[] buf[i];
  delete[] buf;
  // delete buf[];
  for(int i = 0; i < block_num; i ++)
    fclose(fp[i]);
  fclose(fp_out);
  return;
}

void sort_file(int task_id, int max_line) {
  printf("start sort file\n");
  bool *line_flag;
  line_flag = new bool[max_line];
  printf("success\n");
  FILE* fp[NODE_NUM];
  for (int node_id = 0; node_id < NODE_NUM; ++node_id) {
    string file_name = "task" + to_string(task_id) + "_" + to_string(node_id);
    fp[node_id] = fopen(file_name.c_str(), "r");
  }
  
  char **buf;
  buf = new char *[NODE_NUM];
  for(int i = 0; i < NODE_NUM; ++i) {
    buf[i] = new char[MAX_LINE_SIZE];
  }
  
  for (int i = 0; i < NODE_NUM; ++i) {
    fgets(buf[i], MAX_LINE_SIZE, fp[i]); 
  }
  
  auto cmp = [] (const pair<int, int>& lhs, const pair<int, int>& rhs) {
                  return lhs.second > rhs.second;
                };

  // 优先级队列中元素为<node_id, line_num>
  priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> pri_queue(cmp);
  for (int i = 0; i < NODE_NUM; ++i) {
    char line_num_str[20];
    memset(line_num_str, '\0', sizeof(line_num_str));
    strncpy(line_num_str, buf[i] + 1, strchr(buf[i]+1, ',') - buf[i] - 1);
    int line_num = atoi(line_num_str);
    pri_queue.push(pair<int, int>(i, line_num));
  }

  string file_name = to_string(task_id) + ".csv";
  FILE *fp_out = fopen(file_name.c_str(), "w");
  while (!pri_queue.empty()) {
    int node_idx = pri_queue.top().first;
    int line_idx = pri_queue.top().second;
    if (line_flag[line_idx] == false) {
      string write_line = to_string(task_id) + buf[node_idx];
      fputs(write_line.c_str(), fp_out);
      line_flag[line_idx] = true;
    }
    pri_queue.pop();
    if (fgets(buf[node_idx], MAX_LINE_SIZE, fp[node_idx])) {
      char line_num_str[20];
      memset(line_num_str, '\0', sizeof(line_num_str));
      strncpy(line_num_str, buf[node_idx] + 1, strchr(buf[node_idx]+1, ',') - buf[node_idx] - 1);
      int line_num = atoi(line_num_str);
      pri_queue.push(pair<int, int>(node_idx, line_num));
    }
    
  }
  for(int i = 0; i < NODE_NUM; i ++)
    delete[] buf[i];
  delete[] buf;
  delete[] line_flag;
  // delete buf[];
  for (int i = 0; i < NODE_NUM; ++i) {
    fclose(fp[i]);
  }
  return;
}