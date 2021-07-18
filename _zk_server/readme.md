# build
g++ clust_comm.cpp -fPIC -shared -I /usr/local/include/zookeeper/ -o libclust_comm.so

g++ clust_file_test.cpp -L. -lclust_comm -L/usr/local/lib -lzookeeper_mt -o file_test

g++ work.cpp -L. -lclust_comm -L/usr/local/lib -lzookeeper_mt -o work