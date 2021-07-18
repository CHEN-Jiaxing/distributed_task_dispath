run.out:main.cpp redis_op.o
	g++  main.cpp redis_op.o -Lhiredis-vip -lhiredis_vip -o $@

redis_op.o:redis_op.cpp
	g++ -c  redis_op.cpp

.PHONY: clean
clean:
	rm *.o *.out