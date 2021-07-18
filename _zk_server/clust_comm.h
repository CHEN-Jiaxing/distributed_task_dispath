//
// Created by sternchen on 2021/6/23.
//

#ifndef _ZKPER_SERVER_CLUST_COMM_H
#define _ZKPER_SERVER_CLUST_COMM_H

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <set>
#include <string>
#include <vector>
#include <zookeeper/zookeeper.h>

using std::string;
using std::vector;

enum WORK_MODE{MODE_MONITOR,MODE_WORKER} g_mode;
char g_path_elect[512]= "/elect";
char g_path_works[512]= "/works";
char g_path_files[512] = "/files";

/*
 *  =================== /file operation
 */

/*
 * ��ʼ�������У�����
 */
void pr();

/*
 * ���ļ����뼯Ⱥ
 */
void insr2clst(const char* file_no);

/*
 * ������Ҫ��
 * ��չʾЧ��
 */
void dlt(const char* file_no);

/*
 * �����ļ����״̬
 * ��һλ�ֱ�Ϊ r y g ����δ���� ��������� �������
 * ��һλ���������� ����������ļ�
 */
void st(const char* file_no, const char* value);

/*
 * ��ȡ�ļ����״̬
 */
string gt(const char* file_no);

/*
 * ʹ����������У�����
 */
void fin();

/*
 *  ===== /work and /elect operation
 */


// ��ȡ��ǰ���ڵ�worker string��ʽip:pid ����127.0.0.1:9999
vector<string> get_workers(zhandle_t *zkhandle);

void d1(zhandle_t *zkhandle);

void d2(zhandle_t *zkhandle);

void set_wk_st(zhandle_t *zkhandle);

string recv_work(zhandle_t *zkhandle);

void DEBUG();

void INIT(char *host, zhandle_t** zkhandle);

void CLOSE(zhandle_t* zkhandle);

void PREWORK(zhandle_t* zkhandle);

#endif //_ZKPER_SERVER_CLUST_COMM_H
