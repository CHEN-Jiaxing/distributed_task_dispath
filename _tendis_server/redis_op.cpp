//
// Created by sternchen on 2021/6/7.
// Modified by shinji on 2021/6/25.
//

#include "redis_op.h"

using std::to_string;
using std::string;
using std::cout;
using std::endl;
using std::make_pair;

redis_data::redis_data() {
  cc = redisClusterContextInit();
  connect_init();
}

redis_data::~redis_data() {
  redisClusterFree(cc);
}

void redis_data::redis_insert(const string &cmd) {
  redisReply *re = (redisReply *)redisClusterCommand(cc, cmd.c_str());
  /* redisReply *re;
  redisClusterAppendCommand(cc, cmd.c_str());
  redisClusterGetReply(cc, (void **)&re);
  redisClusterReset(cc); */
  if (nullptr == re) {
    printf("set failed, reply is null[%s]\n", cc->errstr);
    return;
  }
  //printf("%s\n", cmd.c_str());
  //printf("set successfully!\n");
}

string redis_data::redis_select(const string &cmd){
  redisReply *re = (redisReply *)redisClusterCommand(cc, cmd.c_str());
  if (nullptr == re) {
    printf("get failed, reply is null[%s]\n", cc->errstr);
    return "error";
  }
  //printf("get successfully!\n");
  //printf("value is: %s\n", re->str);
  if (re->str == NULL) {
    return "";
  }
  return re->str;
}

bool redis_data::connect_init() {
  
//  char *nodes = "127.0.0.1:6001,127.0.0.1:6002,127.0.0.1:6003,127.0.0.1:6004,127.0.0.1:6005,127.0.0.1:6006";
    char *nodes = "9.134.0.139:51002,9.134.0.139:51001,9.134.190.157:52002,9.134.74.195:5002,9.134.74.195:5001,9.134.54.5:7002,9.135.155.140:51002,9.135.155.140:51003,9.134.54.5:7001,9.134.190.157:52001";
//    cout << nodes << endl;
    //char *nodes = "9.135.155.140:51002,9.135.155.140:51003,9.134.74.195:5001,9.134.74.195:5002,9.134.190.157:5001,9.134.190.157:5002";

  redisClusterSetOptionAddNodes(cc, nodes);
  redisClusterSetOptionRouteUseSlots(cc);
  redisClusterConnect2(cc);

  if (cc != NULL && cc->err) {
    printf("Error: %s\n", cc->errstr);
    return false;
  }

  printf("Connect successfully!\n");
  
  return true;
}