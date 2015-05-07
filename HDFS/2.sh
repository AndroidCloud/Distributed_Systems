protoc --cpp_out=. project.proto
pkg-config --cflags protobuf  # fails if protobuf is not installed
c++ datanode_client.cpp namenode_clnt.c datanode_clnt.c -std=c++11 -w project.pb.cc -o datanode_client `pkg-config --cflags --libs protobuf`