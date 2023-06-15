# Distributed programs that use then RPC paradigm

## Programs

- registration - a client server program which implements a class registration platform. The program should allow the user to register themselves by entering their details which include the student registration number and names.
- calculator - a client server program which implements a simple calculator. The calculator can perform the following operations (+, -, *, /) chosen by the user on any two integers supplied by the user.

1. Install gRPC and Protobuf on your Linux system:

Step 1: Install the necessary dependencies:

```css
sudo apt-get update
sudo apt-get install build-essential autoconf libtool pkg-config
```

Step 2: Install gRPC and Protobuf using `apt`:

```css
sudo apt-get install libgrpc++-dev protobuf-compiler-grpc
```

Step 3: Install gRPC C++ Plugin:

```css
sudo apt-get install grpc-compiler
```

2. Compile the Protobuf Files

- In the same directory as your program, run the following command to compile the Protobuf files:

```css
 protoc --cpp_out=. --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` filename.idl.proto
```

Replace 'filename' with the actual name of your file. This command generates the necessary C++ files (`filename.idl.pb.h` and `filename.idl.grpc.pb.h`) based on the Protobuf definition (`filename.idl.proto`).

3. Build the server

- Use a C++ compiler to build the server code and link against the required gRPC libraries. Assuming you have `g++` installed, you can use the following command:

```c
g++ -std=c++11 -o server server.cpp filename.idl.grpc.pb.cc filename.idl.pb.cc -lgrpc++ -lgrpc -lprotobuf -pthread
```

4. Build the client

```c
g++ -std=c++11 -o client client.cpp filename.idl.grpc.pb.cc filename.idl.pb.cc -lgrpc++ -lgrpc -lprotobuf -pthread
```

5. Run the server

- Execute the server program in one terminal window:

```bash
./server
```

6. Run the client

- In another terminal window, execute the client program:

```bash
./client
```
