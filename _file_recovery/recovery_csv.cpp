#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include "recovery_csv.h"
#include "../_tendis_server/redis_op.h"
#include "ThreadPool.h"

RecoveryCsv::RecoveryCsv(int task_id) : task_id_(task_id) {
  printf("start\n");
  // redis_data rd_;
  total_blocks_ = 0;
  for (int node_id_ = 1; node_id_ <= 5; ++node_id_) {
    // node_id_1_task_id_2
    std::string max_block_query = "node_id_" + std::to_string(node_id_) + "_" + "task_id_" + std::to_string(task_id_);
    std::string value;
    try {
      value = rd_.redis_select("get " + max_block_query);
    }
    catch (...) { }
    long max_block_id_ = atol(value.c_str());
    if (max_block_id_ > total_blocks_) {
      total_blocks_ = max_block_id_;
    }
  }
  total_blocks_ = 10000;
}

void RecoveryCsv::do_recovery() {
  std::string file_name = "task_" + std::to_string(task_id_) + ".csv";
  FILE *fp = fopen(file_name.c_str(), "w+");
  // redis_data rd_;
  ThreadPool pool(thread_num_);
  pool.enqueue(do_write, this, fp);
  for (int block_id_ = 1; block_id_ <= total_blocks_; ++block_id_) {
    pool.enqueue(do_select, this, block_id_);
  }
  printf("get block number: %d\n", total_blocks_);
}

//void RecoveryCsv::write_block(RecoveryCsv *pthis, const int &block_id_, redis_data &rd_, FILE *fp) {
void RecoveryCsv::do_select(RecoveryCsv *pthis, const int &block_id_) {
  // redis_data rd_;
  while (true) {
    if (block_id_ == pthis->write_flag_) {
      pthis->mtx_.lock();
      std::string key = std::to_string(pthis->task_id_) + "_" + std::to_string(block_id_);
      std::string value = pthis->rd_.redis_select("get " + key);
      std::string csv_line = key + "," + value + "\n";
      pthis->buf_queue_.push(csv_line);
      pthis->write_flag_++;
      pthis->mtx_.unlock();
      return;
    }
  }
}

void RecoveryCsv::do_write(RecoveryCsv *pthis, FILE *fp) {
  sleep(0.1);
  while (!pthis->buf_queue_.empty()) {
    fputs(pthis->buf_queue_.front().c_str(), fp);
    pthis->buf_queue_.pop();
  }
}

void RecoveryCsv::easy_recovery() {
  std::string file_name = "task_" + std::to_string(task_id_) + ".csv";
  FILE *fp = fopen(file_name.c_str(), "w+");
  redis_data rd_;

  for (int block_id_ = 1; block_id_ <= total_blocks_; ++block_id_) {
    std::string key = std::to_string(task_id_) + "_" + std::to_string(block_id_);
    std::string value = rd_.redis_select("get " + key);
    std::string csv_line = std::to_string(task_id_) + "," + std::to_string(block_id_) + "," + value + "\n";
    fputs(csv_line.c_str(), fp);
  }
  printf("get block number: %d\n", total_blocks_);
}

// 如果write_flag_设计成静态成员函数的话，其中的task_id_和write_flag_都需要设置成竟态成员变量
// 为什么要设置成静态成员函数，因为