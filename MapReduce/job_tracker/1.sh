protoc -I=../Proto --cpp_out=../Proto ../Proto/MapReduce.proto
pkg-config --cflags protobuf  # fails if protobuf is not installed
c++ jobtracker_server.cpp jobtracker_svc.c ../task_tracker/tasktracker_clnt.c -std=c++11 -w ../Proto/MapReduce.pb.cc -o jobtracker `pkg-config --cflags --libs protobuf` -lpthread -I /usr/include/tirpc/ -I /usr/include/rpc2/
