# Distributed programs that use then RPC paradigm

## Programs

- registration - a client server program which implements a class registration platform. The program should allow the user to register themselves by entering their details which include the student registration number and names.
- calculator - a client server program which implements a simple calculator. The calculator can perform the following operations (+, -, *, /) chosen by the user on any two integers supplied by the user.

1. Install gRPC and Protobuf on your Linux system:

Step 1: Install the necessary dependencies
```
sudo apt-get update
sudo apt-get install build-essential autoconf libtool pkg-config
```

Step 2: Install gRPC and Protobuf using `apt`
```
sudo apt-get install libgrpc++-dev protobuf-compiler-grpc
```

Step 3: Install gRPC C++ Plugin
```
sudo apt-get install grpc-compiler
```

2. Compile the Protobuf Files

- In the same directory as your program, run the following command to compile the Protobuf files:

```
 protoc --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` filename.idl.proto
```

Replace 'filename' with the actual name of your file. This command generates the necessary C++ files (`filename.idl.pb.h` and `filename.idl.grpc.pb.h`) based on the Protobuf definition (`filename.idl.proto`).

3. Build the Program

- Use a C++ compiler to build the program and link against the required gRPC libraries. Assuming you have `g++` installed, you can use the following command:

```
g++ -std=c++11 -o filename_client filename_client.cpp filename.idl.grpc.pb.cc filename.idl.pb.cc -lgrpc++ -lgrpc -lprotobuf -pthread
```

4. Run the Program

- After the compilation is successful, you can run the program with the following command:

```
./filename_client
```

This will execute the program.
