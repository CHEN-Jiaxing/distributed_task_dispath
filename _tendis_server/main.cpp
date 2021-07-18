#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <map>
#include <iostream>
#include <ctime>
#include "hiredis-vip/hircluster.h"
// #include "hiredis/hiredis.h"
#include "redis_op.h"

using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::map;

void work() {
  cout<<"=====0====="<<endl;
  redis_data rd;
  rd.redis_insert("mset stest10 value10 stest11 value11");

  string res;
  res = rd.redis_select("get stest10");
  cout<<getpid()<<res<<std::endl;
  res = rd.redis_select("get stest11");
  cout<<getpid()<<res<<std::endl;
}

int main()
{
  redis_data rd;
  clock_t tik, tok;
  tik = clock();
  for (int i = 1; i <= 50000; ++i) {
    string cmd = "get 1_" + to_string(i);
    string res = rd.redis_select(cmd);
    //cout << res << endl;
  }
  tok = clock();
  double time = double(tok-tik) / CLOCKS_PER_SEC;
  printf("time is: %f\n", time);

  work();

  return 0;
}