//
// Created by sternchen on 2021/6/23.
//

#include "clust_comm.h"

#include <unistd.h>
#include "clust_comm.h"
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <vector>
using std::set;
using std::string;
using std::vector;

/*
 * =================== /file operation ===================
 */

zhandle_t *f_zkhandle = NULL;

void show_list() {
    char path[512] = {0};
    memcpy(path, g_path_files, sizeof(g_path_files));
    struct String_vector children;
    int i = 0;
    int ret = zoo_get_children(f_zkhandle, path, 1, &children);
    printf("ret is ====%d===", ret);
    if (ret == ZOK) {
        char child_path[512] = {0};
        char notify_buffer[1024] = {0};
        int notify_len = sizeof(notify_buffer);

        printf("--------------\n");
        for (i = 0; i < children.count; ++i) {
            sprintf(child_path, "%s/%s", path, children.data[i]);
            ret = zoo_get(f_zkhandle, child_path, 1, notify_buffer, &notify_len, NULL);
            if (ret != ZOK) {
                fprintf(stderr, "failed to get the data of path %s!\n", child_path);
            } else {
                printf("%s:%s\n", children.data[i], notify_buffer);
            }
        }
    } else {
        fprintf(stderr, "failed to get the children of path %s!\n", path);
    }

    for (i = 0; i < children.count; ++i) {
        free(children.data[i]);
        children.data[i] = NULL;
    }
}

void zktest_watcher_gfile(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx) {
    /*
    printf("watcher event\n");
    sleep(2);
    printf("type: ===%d===\n", type);
    sleep(2);
    printf("state: ===%d===\n", state);
    sleep(1);
    printf("path: ===%s===\n", path);
    sleep(1);
    printf("watcherCtx: %s\n", (char *)watcherCtx);
    sleep(1);
    printf("--------------\n");
  */
/*
  if(strcmp(path, g_path_elect) == 0) {
    if(type == ZOO_CHILD_EVENT && state == ZOO_CONNECTED_STATE) {
      printf("run to here!!!\n");
      choose_mater(zh,path);
      if(g_mode == MODE_MONITOR){
        _master(zh,path);show_list
      }
    }
  }
  else {
    if(type == ZOO_CHILD_EVENT && state == ZOO_CONNECTED_STATE ){
      printf("=====1====\n");
      show_list(zh,path);
    }else if(type == ZOO_CHANGED_EVENT && state == ZOO_CONNECTED_STATE ){
      printf("=====2====\n");
      show_list(zh,path);
    }
  }
  printf("global----------------->\n");
  */
    // show_list();
}

void create() {
    char path_buffer[512];
    int bufferlen = sizeof(path_buffer);
    int ret = 0;
    /*
    ret = zoo_exists(f_zkhandle, g_path_elect, 0, NULL);
    while(ret != ZOK){
      ret = zoo_create(f_zkhandle,g_path_elect,"elct",4,&ZOO_OPEN_ACL_UNSAFE,0,path_buffer,bufferlen);
    }
    show_list(f_zkhandle,g_path_elect);

    ret = zoo_exists(f_zkhandle, g_path_works, 0, NULL);
    while(ret != ZOK){
      ret = zoo_create(f_zkhandle,g_path_works,"work",4,&ZOO_OPEN_ACL_UNSAFE,0,path_buffer,bufferlen);
    }
    show_list(f_zkhandle,g_path_works);
  */
    ret = zoo_exists(f_zkhandle, g_path_files, 0, NULL);
    while (ret != ZOK) {
        ret = zoo_create(f_zkhandle, g_path_files, "file", 4, &ZOO_OPEN_ACL_UNSAFE, 0, path_buffer, bufferlen);
    }
    // show_list(f_zkhandle,g_path_files);
    // show_list();
}

void pre_clst() {
//  char host[512]= "127.0.0.1:2181,127.0.0.1:2182,127.0.0.1:2183";
    char host[512] = "9.135.12.132:2181,9.134.74.195:2181,9.135.155.140:2181,9.134.190.157:2181,9.134.0.139:2181";
    zoo_set_debug_level(ZOO_LOG_LEVEL_WARN); //设置日志级别,避免出现一些其他信息

    f_zkhandle = zookeeper_init(host, zktest_watcher_gfile, 30000, 0, (char *) "Hotpot Test", 0);
    while (f_zkhandle == NULL) {
        f_zkhandle = zookeeper_init(host, zktest_watcher_gfile, 30000, 0, (char *) "Hotpot Test", 0);
    }

    create();
}

void insr2clst(const char *file_no) {
//    char path[512] = {0};
//    memcpy(path, g_path_files, sizeof(g_path_files));
    char child_path[512] = {0};
    char path_buffer[1024] = {0};
    int bufferlen = sizeof(path_buffer);

    sprintf(child_path, "%s/%s", g_path_files, file_no);
    int ret = zoo_create(f_zkhandle, child_path, "r", 1, &ZOO_OPEN_ACL_UNSAFE, 0, path_buffer, bufferlen);
/*    if (ret != ZOK) {
        fprintf(stderr, "failed to get the data of path %s!\n", child_path);
    } else {
        printf("%s:%s\n", child_path, path_buffer);
    }*/
}

void dlt(const char *file_no) {
    char path[512] = {0};
    memcpy(path, g_path_files, sizeof(g_path_files));
    char child_path[512] = {0};
    char path_buffer[1024] = {0};
    int bufferlen = sizeof(path_buffer);

    sprintf(child_path, "%s/%s", path, file_no);
    int ret = zoo_delete(f_zkhandle, child_path, -1);
    if (ret) {
        fprintf(stderr, "Error %d for %s\n", ret, "adelete");
        exit(EXIT_FAILURE);
    }
}

void st(const char *file_no, const char *value) {
//    char path[512] = {0};
//    memcpy(path, g_path_files, sizeof(g_path_files));
    char child_path[512] = {0};
//    char path_buffer[1024] = {0};
//    int bufferlen = sizeof(path_buffer);

   sprintf(child_path, "%s/%s", g_path_files, file_no);
//    int ret = zoo_set(f_zkhandle, child_path, value, sizeof(value), -1);
//    if (ret) {
//        fprintf(stderr, "Error %d for %s\n", ret, "adelete");
//        exit(EXIT_FAILURE);
//    }

  zoo_set(f_zkhandle, child_path, value, 1, -1);
  // zoo_set(f_zkhandle, child_path, value, sizeof(value), -1);
}

string gt(const char *file_no) {
    char value[64] = {0};
    int value_len = sizeof(value);
    char work_path[512] = {0};
    sprintf(work_path, "%s/%s", g_path_files, file_no);
    zoo_get(f_zkhandle, work_path, 0, value, &value_len, NULL);
    string res(value);
    res = res.substr(0, value_len);
    return res;
}

void fin_clst() {
    zookeeper_close(f_zkhandle);
}

/*
 *  ===== /work and /elect operation
 *
 *
 *
 *
 *
 *
 */
void create_works(zhandle_t *zkhandle);

void create_elect(zhandle_t *zkhandle);

void regist(zhandle_t *zkhandle);

void identify(zhandle_t *zkhandle, const char *path);

void monitor_work(zhandle_t *zkhandle, const char *path);

void getlocalhost(char *ip_pid, int len);

//if success,the g_mode will become MODE_MONITOR
void choose_mater(zhandle_t *zkhandle, const char *path);

void zktest_watcher_g(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx);

void PREWORK(zhandle_t *zkhandle) {
    create_works(zkhandle);

    create_elect(zkhandle);

    regist(zkhandle);

    identify(zkhandle, g_path_elect);

    monitor_work(zkhandle, g_path_works);
}

void DEBUG() {
    // zoo_set_debug_level(ZOO_LOG_LEVEL_WARN); //设置日志级别,避免出现一些其他信息
}

void INIT(char *host, zhandle_t **zkhandle) {
    *zkhandle = zookeeper_init(host, zktest_watcher_g, 30000, 0, (char *) "MINI PRJT", 0);
    while (*zkhandle == NULL) {
        *zkhandle = zookeeper_init(host, zktest_watcher_g, 30000, 0, (char *) "MINI PRJT", 0);
    }
}

void CLOSE(zhandle_t *zkhandle) {
    zookeeper_close(zkhandle);
}

void set_wk_st(zhandle_t *zkhandle) {
    char localhost[512] = {0};
    getlocalhost(localhost, sizeof(localhost));
    char child_path[512] = {0};
    sprintf(child_path, "%s/%s", g_path_works, localhost);
    int ret = zoo_set(zkhandle, child_path, "s", 1, -1);
//    if (ret) {
//        fprintf(stderr, "Error %d for %s\n", ret, "adelete");
//        exit(EXIT_FAILURE);
//    }
}

string calc_dest_no(string fileno) {
    return std::to_string((stoi(fileno) + 4) % 5);
}

void dispatch_work(zhandle_t *zkhandle) {
    struct String_vector fls;
    zoo_get_children(zkhandle, g_path_files, 0, &fls);
    struct String_vector wks;
    zoo_get_children(zkhandle, g_path_works, 1, &wks);

    int i_fls = 0;
    int i_wks = 0;
    char flno_path[512] = {0};
    char fl_st[64] = {0};
    int fl_st_len = sizeof(fl_st);

    char wkno_path[512] = {0};
    char wk_st[64] = {0};
    int wk_st_len = sizeof(fl_st);

    for (; i_fls < fls.count; ++i_fls) {
        memset(flno_path, '0', sizeof(flno_path));
        sprintf(flno_path, "%s/%s", g_path_files, fls.data[i_fls]);
        zoo_get(zkhandle, flno_path, 0, fl_st, &fl_st_len, NULL);
        if (fl_st[0] == 'r') {
            for (; i_wks < wks.count; ++i_wks) {
                memset(wkno_path, '0', sizeof(wkno_path));
                sprintf(wkno_path, "%s/%s", g_path_works, wks.data[i_wks]);
                zoo_get(zkhandle, wkno_path, 1, wk_st, &wk_st_len, NULL);
                char localhost[512] = {0};
                getlocalhost(localhost, sizeof(localhost));
                if (wk_st[0] == 's' && (strcmp(localhost, wks.data[i_wks]) != 0)) {
                    char wk_con[512] = {0};
                    sprintf(wk_con, "%s%s%s%s", "w_", fls.data[i_fls], "_", calc_dest_no(fls.data[i_fls]).c_str());
                    zoo_set(zkhandle, wkno_path, wk_con, sizeof(wk_con), -1);
                    sleep(10);
                    break;
                }
            }
          if (i_wks == wks.count)
            break;
        }
    }

    if (i_fls == fls.count) {
        for (; i_wks < wks.count; ++i_wks) {
            memset(wkno_path, '0', sizeof(wkno_path));
            sprintf(wkno_path, "%s/%s", g_path_works, wks.data[i_wks]);
            zoo_get(zkhandle, wkno_path, 1, wk_st, &wk_st_len, NULL);
            char localhost[512] = {0};
            getlocalhost(localhost, sizeof(localhost));
            if (wk_st[0] == 's' && (strcmp(localhost, wks.data[i_wks]) != 0)) {
                char wk_con[512] = {0};
                sprintf(wk_con, "%s%s", "w_", std::to_string(-1).c_str());
                zoo_set(zkhandle, wkno_path, wk_con, sizeof(wk_con), -1);
            }
        }
    }

    for (int i = 0; i < fls.count; ++i) {
        free(fls.data[i]);
        fls.data[i] = NULL;
    }
    for (int i = 0; i < wks.count; ++i) {
        free(wks.data[i]);
        wks.data[i] = NULL;
    }

}

string recv_work(zhandle_t *zkhandle) {
    char wk_flno[64] = {0};
    char localhost[512] = {0};
    getlocalhost(localhost, sizeof(localhost));
    char work_path[512] = {0};
    sprintf(work_path, "%s/%s", g_path_works, localhost);
    char wk_cont[64] = {0};
    int wk_cont_len = sizeof(wk_cont);
    zoo_get(zkhandle, work_path, 1, wk_cont, &wk_cont_len, NULL);
    if (wk_cont[0] == 'w') {
        memcpy(wk_flno, wk_cont, sizeof(wk_cont));
    }
    return wk_flno;
}

void choose_mater(zhandle_t *zkhandle, const char *path) {
    struct String_vector procs;
    int i = 0;
    int ret = zoo_get_children(zkhandle, path, 1, &procs);

    if (ret != ZOK || procs.count == 0) {
        fprintf(stderr, "failed to get the children of path %s!\n", path);
    } else {
        char master[512] = {0};
        strcpy(master, procs.data[0]);
        for (i = 1; i < procs.count; ++i) {
            if (strcmp(master, procs.data[i]) > 0) {
                strcpy(master, procs.data[i]);
            }
        }
        char master_path[512] = {0};
        sprintf(master_path, "%s/%s", path, master);
        char ip_pid[64] = {0};
        int ip_pid_len = sizeof(ip_pid);
        ret = zoo_get(zkhandle, master_path, 0, ip_pid, &ip_pid_len, NULL);
        char localhost[512] = {0};
        getlocalhost(localhost, sizeof(localhost));
        if (ret != ZOK) {
            fprintf(stderr, "failed to get the data of path %s!\n", master_path);
        } else if (strcmp(ip_pid, localhost) == 0) {
            g_mode = MODE_MONITOR;
        } else {
            g_mode = MODE_WORKER;
        }
    }

    for (i = 0; i < procs.count; ++i) {
        free(procs.data[i]);
        procs.data[i] = NULL;
    }
}

//void getlocalhost(char *ip_pid, int len) {
//    char hostname[64] = {0};
//    struct hostent *hent;
//
//    gethostname(hostname, sizeof(hostname));
//    hent = gethostbyname(hostname);
//
//    char *localhost = inet_ntoa(*((struct in_addr *) (hent->h_addr_list[0])));
//
//    snprintf(ip_pid, len, "%s:%lld", localhost, getpid());
//}

bool GetIP(const vector<string>& vNetType,string& strip)
{
  for(size_t i=0;i<vNetType.size();i++)
  {
    for(char c='0';c<='9';++c)
    {
      string strDevice = vNetType[i] + c; //根据网卡类型，遍历设备如eth0，eth1
      int fd;
      struct ifreq ifr;
      //使用UDP协议建立无连接的服务
      fd = socket(AF_INET, SOCK_DGRAM, 0);
      strcpy(ifr.ifr_name, strDevice.c_str() );
      //获取IP地址
      if (ioctl(fd, SIOCGIFADDR, &ifr) <  0)
      {
        close(fd);
        continue;
      }

      // 将一个IP转换成一个互联网标准点分格式的字符串
      strip = inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr);
      if(!strip.empty())
      {
        close(fd);
        return true;
      }
    }
  }
  return false;
}

void getlocalhost(char *ip_pid, int len) {

    vector<string> vs;//预先定义了几种可能的网卡类型
    vs.push_back("eth");
    //vs.push_back("em");
    //vs.push_back("oct");
    string str;
    GetIP(vs,str);


/*
  char name[256];
  gethostname(name, sizeof(name));

  struct hostent* host = gethostbyname(name);
  char ipStr[32];
  inet_ntop(host->h_addrtype, host->h_addr_list[0], ipStr, sizeof(ipStr));

    char hostname[64] = {0};
    struct hostent *hent;

    gethostname(hostname, sizeof(hostname));
    hent = gethostbyname(hostname);

    char *localhost = inet_ntoa(*((struct in_addr *) (hent->h_addr_list[0])));
*/
    snprintf(ip_pid, len, "%s:%lld", str.c_str(), getpid());
}


void create_works(zhandle_t *zkhandle) {
    char path_buffer[512];
    int ret = 0;
    /*
    ret = zoo_exists(zkhandle, g_path_elect, 0, NULL);
    while(ret != ZOK){
      ret = zoo_create(zkhandle,g_path_elect,"elct",4,&ZOO_OPEN_ACL_UNSAFE,0,path_buffer,bufferlen);
    }
    show_list(zkhandle,g_path_elect);
    */
    ret = zoo_exists(zkhandle, g_path_works, 0, NULL);
    while (ret != ZOK) {
        ret = zoo_create(zkhandle, g_path_works, "work", 4, &ZOO_OPEN_ACL_UNSAFE, 0, path_buffer, sizeof(path_buffer));
    }
    // zoo_exists(zkhandle, g_path_works, 1, NULL);
    /*
    ret = zoo_exists(zkhandle, g_path_files, 0, NULL);
    while(ret != ZOK){
      ret = zoo_create(zkhandle,g_path_files,"file",4,&ZOO_OPEN_ACL_UNSAFE,0,path_buffer,bufferlen);
    }
    show_list(zkhandle,g_path_files);
     */
}

void create_elect(zhandle_t *zkhandle) {
    char path[512] = {0};
    memcpy(path, g_path_elect, sizeof(g_path_elect));
    char path_buffer[512];

    int ret = zoo_exists(zkhandle, g_path_elect, 0, NULL);
    while (ret != ZOK) {
        ret = zoo_create(zkhandle, g_path_elect, "elct", 4, &ZOO_OPEN_ACL_UNSAFE, 0, path_buffer, sizeof(path_buffer));
    }
    // zoo_exists(zkhandle, g_path_elect, 1, NULL);
}

void recover(zhandle_t *zkhandle) {
    char localhost[512] = {0};
    getlocalhost(localhost, sizeof(localhost));
    char work_path[512] = {0};
    sprintf(work_path, "%s/%s", g_path_works, localhost);
    char wk_cont[64] = {0};
    int wk_cont_len = sizeof(wk_cont);
    zoo_get(zkhandle, work_path, 1, wk_cont, &wk_cont_len, NULL);
    if (wk_cont[0] == 'w') {
        char wk_flno[64] = {0};
        memcpy(wk_flno, wk_cont + 1, sizeof(wk_cont) - 1);
        char file_path[512] = {0};
        sprintf(file_path, "%s/%s", g_path_files, wk_flno);
        zoo_set(zkhandle, file_path, "r", 1, -1);
        zoo_set(zkhandle, work_path, "s", 1, -1);
    }
printf("------------------------\n");
    set<string> dct;
    struct String_vector procs;
    int i = 0;
    zoo_get_children(zkhandle, g_path_elect, 1, &procs);

    char master_path[512] = {0};
    char ip_pid[64] = {0};
    int ip_pid_len = sizeof(ip_pid);
    for (i = 0; i < procs.count; ++i) {
        memset(master_path, '\0', sizeof(master_path));
        sprintf(master_path, "%s/%s", g_path_elect, procs.data[i]);
        memset(ip_pid, '\0', sizeof(ip_pid));
        zoo_get(zkhandle, master_path, 0, ip_pid, &ip_pid_len, NULL);
      printf("======%s=== is in elect=====\n", ip_pid);
        string s(ip_pid);
        s = s.substr(0, ip_pid_len);
      printf("======%s=== is in elect=====\n", s.c_str());
        dct.insert(s);
        free(procs.data[i]);
        procs.data[i] = NULL;
    }

    struct String_vector wks;
    zoo_get_children(zkhandle, g_path_works, 1, &wks);

    char wks_path[512] = {0};
    char wks_st[64] = {0};
    int wks_st_len = sizeof(wks_st);
    for (i = 0; i < wks.count; ++i) {
        memset(wks_path, '0', sizeof(wks_path));

      printf("======%s=== is in work=====\n", wks.data[i]);

        if (dct.find(wks.data[i]) == dct.end()) {
            sprintf(wks_path, "%s/%s", g_path_works, wks.data[i]);
            memset(wks_st, '0', sizeof(wks_st));
            zoo_get(zkhandle, wks_path, 0, wks_st, &wks_st_len, NULL);
            if (wks_st[0] == 'w') {
                char wk_flno[64] = {0};
                memcpy(wk_flno, wks_st + 1, wks_st_len - 1);
                char file_path[512] = {0};
                sprintf(file_path, "%s/%s", g_path_files, wk_flno);
                zoo_set(zkhandle, file_path, "r", 1, -1);
            }
            zoo_delete(zkhandle, wks_path, -1);
        }

        free(wks.data[i]);
        wks.data[i] = NULL;
    }
}

void reelect(zhandle_t *zkhandle) {
    struct String_vector els;
    zoo_get_children(zkhandle, g_path_elect, 1, &els);
    struct String_vector wks;
    zoo_get_children(zkhandle, g_path_works, 1, &wks);

    // printf("come to reeelect!!!\n");

    if (els.count != wks.count) {
        // printf(" reeelect!!!\n");
        choose_mater(zkhandle, g_path_elect);

        if (g_mode == MODE_MONITOR) {
            recover(zkhandle);
        }
    }

    for (int i = 0; i < els.count; ++i) {
        free(els.data[i]);
        els.data[i] = NULL;
    }
    for (int i = 0; i < wks.count; ++i) {
        free(wks.data[i]);
        wks.data[i] = NULL;
    }
}

void zktest_watcher_g(zhandle_t *zkhandle, int type, int state, const char *path, void *watcherCtx) {
    /*
    printf("watcher event\n");
    printf("type: ===%d===\n", type);
    printf("state: ===%d===\n", state);
    printf("path: ===%s===\n", path);
    printf("watcherCtx: %s\n", (char *)watcherCtx);
    printf("--------------\n");
    */

    char e_path[512] = {0};
    memcpy(e_path, path, 6);
    if (g_mode == MODE_MONITOR) {
        if ((strcmp(e_path, g_path_works) == 0) && ((type == ZOO_CHILD_EVENT && state == ZOO_CONNECTED_STATE) ||
                                                    (type == ZOO_CHANGED_EVENT && state == ZOO_CONNECTED_STATE))) {
            dispatch_work(zkhandle);
        } else if ((strcmp(e_path, g_path_elect) == 0) && (type == ZOO_CHILD_EVENT && state == ZOO_CONNECTED_STATE)) {
            reelect(zkhandle);
        }
    } else {
        if ((strcmp(e_path, g_path_works) == 0) && (strlen(path) > 6) &&
            (type == ZOO_CHANGED_EVENT && state == ZOO_CONNECTED_STATE)) {
            recv_work(zkhandle);
        } else if ((strcmp(e_path, g_path_elect) == 0) && (type == ZOO_CHILD_EVENT && state == ZOO_CONNECTED_STATE)) {
            reelect(zkhandle);
        }
    }
}

/*
void create(zhandle_t *zkhandle) {
  char path_buffer[512];
  int bufferlen=sizeof(path_buffer);
  int ret = 0;

  ret = zoo_exists(zkhandle, g_path_elect, 0, NULL);
  while(ret != ZOK){
    ret = zoo_create(zkhandle,g_path_elect,"elct",4,&ZOO_OPEN_ACL_UNSAFE,0,path_buffer,bufferlen);
  }
  show_list(zkhandle,g_path_elect);

  ret = zoo_exists(zkhandle, g_path_works, 0, NULL);
  while(ret != ZOK){
    ret = zoo_create(zkhandle,g_path_works,"work",4,&ZOO_OPEN_ACL_UNSAFE,0,path_buffer,bufferlen);
  }
  show_list(zkhandle,g_path_works);

  ret = zoo_exists(zkhandle, g_path_files, 0, NULL);
  while(ret != ZOK){
    ret = zoo_create(zkhandle,g_path_files,"file",4,&ZOO_OPEN_ACL_UNSAFE,0,path_buffer,bufferlen);
  }
  show_list(zkhandle,g_path_files);
}
 */

void regist(zhandle_t *zkhandle) {
    char localhost[512] = {0};
    getlocalhost(localhost, sizeof(localhost));
    char child_path[512] = {0};
    char path_buffer[1024] = {0};

  sprintf(child_path, "%s/proc-", g_path_elect);
  // int ret = zoo_create(zkhandle, child_path, localhost, sizeof(localhost), &ZOO_OPEN_ACL_UNSAFE,
    //               ZOO_SEQUENCE | ZOO_EPHEMERAL, path_buffer, sizeof(path_buffer));
  int ret = zoo_create(zkhandle, child_path, localhost, 24, &ZOO_OPEN_ACL_UNSAFE,
                       ZOO_SEQUENCE | ZOO_EPHEMERAL, path_buffer, sizeof(path_buffer));
  if (ret != ZOK) {
    fprintf(stderr, "failed to create the child_path %s,buffer:%s!\n", child_path, path_buffer);
  } else {
    printf("create child path %s successfully!\n", path_buffer);
  }

  memset(child_path, '0', sizeof(child_path));

    sprintf(child_path, "%s/%s", g_path_works, localhost);
     ret = zoo_create(zkhandle, child_path, "s", 1, &ZOO_OPEN_ACL_UNSAFE, 0, path_buffer, sizeof(path_buffer));
    if (ret != ZOK) {
        fprintf(stderr, "failed to create the child_path %s,buffer:%s!\n", child_path, path_buffer);
    } else {
        printf("create child path %s successfully!\n", path_buffer);
    }
}

void do_monitor_work(zhandle_t *zkhandle) {
    // printf("=====im monitor=====\n");
}

void do_worker_work(zhandle_t *zkhandle) {
    // printf("=====im worker=====\n");
}

void identify(zhandle_t *zkhandle, const char *path) {
    struct String_vector procs;
    int ret = zoo_get_children(zkhandle, path, 1, &procs);

    if (ret != ZOK || procs.count == 0) {
        fprintf(stderr, "failed to get the children of path %s!\n", path);
    } else if (procs.count == 1) {
        g_mode = MODE_MONITOR;
    } else {
      choose_mater(zkhandle, path);
    }

    for (int i = 0; i < procs.count; ++i) {
        free(procs.data[i]);
        procs.data[i] = NULL;
    }
}

/*
void show_list(zhandle_t *zkhandle,const char *path) {
  struct String_vector children;
  int i = 0;
  int ret = zoo_get_children(zkhandle,path,1,&children);

  if(ret == ZOK){
    char child_path[512] ={0};
    char notify_buffer[1024] = {0};
    int notify_len = sizeof(notify_buffer);

    printf("--------------\n");
    for(i = 0; i < children.count; ++i){
      sprintf(child_path,"%s/%s",path,children.data[i]);
      ret = zoo_get(zkhandle,child_path,1,notify_buffer,&notify_len,NULL);
      if(ret != ZOK){
        fprintf(stderr,"failed to get the data of path %s!\n",child_path);
      }else{
        printf("%s:%s\n",children.data[i],notify_buffer);
      }
    }
  }else{
    fprintf(stderr,"failed to get the children of path %s!\n",path);
  }

  for(i = 0; i < children.count; ++i){
    free(children.data[i]);
    children.data[i] = NULL;
  }
}
 */

void monitor_work(zhandle_t *zkhandle, const char *path) {
    if (g_mode == MODE_MONITOR) {
        struct String_vector children;
        int i = 0;
        int ret = zoo_get_children(zkhandle, path, 1, &children);

        if (ret == ZOK) {
            char child_path[512] = {0};
            char notify_buffer[1024] = {0};
            int notify_len = sizeof(notify_buffer);

            //printf("--------------\n");
            for (i = 0; i < children.count; ++i) {
                sprintf(child_path, "%s/%s", path, children.data[i]);
                ret = zoo_get(zkhandle, child_path, 1, notify_buffer, &notify_len, NULL);
                if (ret != ZOK) {
                    fprintf(stderr, "failed to get the data of path %s!\n", child_path);
                } else {
                    printf("%s:%s\n", children.data[i], notify_buffer);
                }
            }
        }
        for (i = 0; i < children.count; ++i) {
            free(children.data[i]);
            children.data[i] = NULL;
        }
    } else {
        char child_path[512] = {0};
        char localhost[512] = {0};
        getlocalhost(localhost, sizeof(localhost));
        sprintf(child_path, "%s/%s", path, localhost);
        zoo_exists(zkhandle, child_path, 1, NULL);
    }
}

vector<string> get_workers(zhandle_t *zkhandle) {
    vector<string> res;
    struct String_vector children;
    int i = 0;
    int ret = zoo_get_children(zkhandle, g_path_works, 1, &children);

    if (ret == ZOK) {
        printf("--------------\n");
        for (i = 0; i < children.count; ++i) {
            res.push_back(children.data[i]);
        }
    }

    for (i = 0; i < children.count; ++i) {
        free(children.data[i]);
        children.data[i] = NULL;
    }
    return res;
}


/*
void show_list_master(zhandle_t *zkhandle,const char *path)
{
  struct String_vector procs;
  int i = 0;
  char localhost[512]={0};

  getlocalhost(localhost,sizeof(localhost));

  int ret = zoo_get_children(zkhandle,path,1,&procs);

  if(ret != ZOK){
    fprintf(stderr,"failed to get the children of path %s!\n",path);
  }else{
    char child_path[512] ={0};
    char ip_pid[64] = {0};
    int ip_pid_len = sizeof(ip_pid);
    printf("--------------\n");
    printf("ip\tpid\n");
    for(i = 0; i < procs.count; ++i){
      sprintf(child_path,"%s/%s",path,procs.data[i]);
      //printf("%s\n",child_path);
      ret = zoo_get(zkhandle,child_path,0,ip_pid,&ip_pid_len,NULL);
      if(ret != ZOK){
        fprintf(stderr,"failed to get the data of path %s!\n",child_path);
      }else if(strcmp(ip_pid,localhost)==0){
        printf("%s(Master)\n",ip_pid);
      }else{
        printf("%s\n",ip_pid);
      }
    }
  }

  for(i = 0; i < procs.count; ++i){
    free(procs.data[i]);
    procs.data[i] = NULL;
  }
}
*/
