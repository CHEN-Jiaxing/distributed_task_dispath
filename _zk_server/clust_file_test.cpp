#include "clust_comm.h"
#include <string>
#include <cstdio>
#include <iostream>

#define no 5

int main() {
  pr();

  for(int i = 0; i < no; i++) {
    insr2clst(std::to_string(i).c_str());
    getchar();
  }

  for(int i = 0; i < no; i++) {
    std::cout<<"==="<<gt(std::to_string(i).c_str())<<"==="<<std::endl;
    getchar();
  }

  for(int i = 0; i < no; i++) {
    string s;
    std::cout<<"ÊäÈëÒªÉèÖÃ×Ö·û´®"<<std::endl;
    std::cin>>s;
    st(std::to_string(i).c_str(), s.c_str());
    getchar();
  }

  for(int i = 0; i < no; i++) {
    std::cout<<gt(std::to_string(i).c_str())<<std::endl;
    getchar();
  }
  for(int i = 0; i < no; i++) {
    dlt(std::to_string(i).c_str());
    getchar();
  }

  fin();
}