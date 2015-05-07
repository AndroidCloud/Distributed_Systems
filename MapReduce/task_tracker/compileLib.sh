g++ -std=c++11 -c -fPIC wordCount_map.cpp -o libwordcount_map.o
g++ -std=c++11 -c -fPIC wordCount_reduce.cpp -o libwordcount_reduce.o
g++ -std=c++11 -shared -o libwordcount.so libwordcount_map.o libwordcount_reduce.o
#g++ -Wall -g -DNDEBUG 1.cpp -ldl -o exe
