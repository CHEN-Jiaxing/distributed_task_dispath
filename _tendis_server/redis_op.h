//
// Created by sternchen on 2021/6/7.
// Modified by shinji on 2021/6/25.
//

#ifndef DISTRIBUTED_SERVER_REDIS_OP_H
#define DISTRIBUTED_SERVER_REDIS_OP_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <string>
#include <iostream>
#include <map>
#include "hiredis-vip/hircluster.h"

using std::string;
using std::cout;
using std::endl;
using std::map;


class redis_data {
public:
  redis_data();
  ~redis_data();
  void redis_insert(const string &cmd);
  string redis_select(const string &cmd);

private:
  bool connect_init();
  redisClusterContext *cc;
  int node_num = 6;
};

#endif //DISTRIBUTED_SERVER_REDIS_OP_H
