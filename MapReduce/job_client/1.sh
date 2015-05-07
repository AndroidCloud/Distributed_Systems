protoc -I=../Proto --cpp_out=../Proto ../Proto/MapReduce.proto
pkg-config --cflags protobuf  # fails if protobuf is not installed
c++ job_client.cpp job_clnt.c -std=c++11 -w ../Proto/MapReduce.pb.cc -o job_client `pkg-config --cflags --libs protobuf`