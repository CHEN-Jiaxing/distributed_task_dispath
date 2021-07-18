#run.out : main.cpp recovery_csv.o ../_tendis_server/redis_op.o
#	g++ -o run.out main.cpp recovery_csv.o ../_tendis_server/redis_op.o -L../_tendis_server/hiredis-vip -lhiredis_vip -std=c++11 -lpthread

#recovery_csv.o : recovery_csv.cpp 
#	g++ -c recovery_csv.cpp  -L../_tendis_server/hiredis-vip -lhiredis_vip -std=c++11 -lpthread

run.out : main.cpp re_file.o ../_tendis_server/redis_op.o
	g++ -g -o run.out main.cpp re_file.o ../_tendis_server/redis_op.o -L../_tendis_server/hiredis-vip -lhiredis_vip -std=c++11 -lpthread

re_file.o : re_file.cpp 
	g++ -g -c re_file.cpp  -L../_tendis_server/hiredis-vip -lhiredis_vip -std=c++11 -lpthread

.PHONY: clean
clean:
	rm -f *.o *.out