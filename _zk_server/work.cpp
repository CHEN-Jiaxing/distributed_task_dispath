#include "clust_comm.h"

void work() {
  g_mode = MODE_WORKER;
  char host[512] = "127.0.0.1:2181,127.0.0.1:2182,127.0.0.1:2183";
  zhandle_t* zkhandle = NULL;
  DEBUG();

  INIT(host, &zkhandle);

  PREWORK(zkhandle);

  while(1) {
    sleep(1);

    if(g_mode == MODE_MONITOR) {
      // d1(zkhandle);
    } else {
      // d2(zkhandle);

      /*
       * ͨ��recv_work����fileno
       */
      printf("get filno is %s\n", recv_work(zkhandle).c_str());

      /*
       * ͨ��get_workers��ȡ��ǰ���ڵ�ip:pid
       */
      vector<string> tmp = get_workers(zkhandle);
      for(auto s : tmp) {
        printf("%s\n", s.c_str());
      }
    }
  }

  CLOSE(zkhandle);
  free(zkhandle);
}

int main() {
  work();
}
