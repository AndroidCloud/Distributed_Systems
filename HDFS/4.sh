protoc --cpp_out=. project.proto
pkg-config --cflags protobuf  # fails if protobuf is not installed
c++ namenode_server.cpp namenode_svc.c -std=c++11 -w project.pb.cc -o namenode_server `pkg-config --cflags --libs protobuf`