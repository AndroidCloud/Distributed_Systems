protoc -I=../Proto --cpp_out=../Proto ../Proto/MapReduce.proto
pkg-config --cflags protobuf  # fails if protobuf is not installed
c++ -g -DNEBUG tasktracker_server.cpp tasktracker_svc.c tasktracker_clnt.c -std=c++11 -w ../Proto/MapReduce.pb.cc -pthread -ldl -o tasktracker `pkg-config --cflags --libs protobuf` 
#g++ -Wall -g -DNDEBUG 1.cpp -ldl -o exe
