protoc --cpp_out=. project.proto
pkg-config --cflags protobuf  # fails if protobuf is not installed
c++ datanode_server.cpp datanode_svc.cpp namenode_clnt.c -std=c++11 -w project.pb.cc -o datanode_server `pkg-config --cflags --libs protobuf`