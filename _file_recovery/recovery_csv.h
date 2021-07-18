#ifndef RECOVERY_CSV_H
#define RECOVERY_CSV_H

#include <cstdio>
#include <mutex>
#include <queue>
#include "../_tendis_server/redis_op.h"

class RecoveryCsv {
 public:
  RecoveryCsv(int task_id);
  ~RecoveryCsv() {};
  void do_recovery();
  void easy_recovery();
  // static void write_block(RecoveryCsv *pthis, const int &block_id_, redis_data &rd, FILE *fp);
  static void do_select(RecoveryCsv *pthis, const int &block_id_);
  static void do_write(RecoveryCsv *pthis, FILE *fp);
 private:
  redis_data rd_;
  int thread_num_ = 1;
  int write_flag_ = 1;
  int task_id_;
  long total_blocks_;
  std::queue<string> buf_queue_;
  std::mutex mtx_;
};

// ***********************************************

/* void WriteBlocks(
    const int &task_id, const int &block_id, const redis_data &rd, 
    int &write_flag, FILE *fp) {
  std::string value_query =std::to_string(task_id) + "," + std::to_string(block_id);
  std::string value = rd.redis_select(value_query);
  value_query += "," + value + "\n";
  while (true) {
    if (block_id == write_flag) {
      m_mutex.lock();
      fputs(value_query.c_str(), fp);
      write_flag++;
      m_mutex.unlock();
      return;
    }
  }
}

int RecoveryCsv(int &task_id, redis_data &data_query) {
  int thread_num = 3;
  int write_flag = 0;
  std::mutex m_mutex;

  long total_blocks = 0;
  std::string s_task_id = std::to_string(task_id);
  for (int node_id = 0; node_id < 5; ++node_id) {
    std::string max_block_query = std::to_string(node_id) + "_" + s_task_id;
    std::string value;
    try {
      value = data_query.redis_select("get " + max_block_query);
    }
    catch (...) { }
    long max_block_id = atol(value.c_str());
    if (max_block_id > total_blocks) {
      total_blocks = max_block_id;
    }
  }

  FILE *fp;
  std::string file_name = "task_" + s_task_id + ".csv";
  fp = open(file_name.c_str(), "w+");

  ThreadPool pool(thread_num);
  for (int block_id = 0; block_id < total_blocks; ++block_id) {
    pool.enqueue(WriteBlocks, task_id, block_id, data_query, write_flag, fp);
  }
} */

#endif // RECOVERY_CSV_H