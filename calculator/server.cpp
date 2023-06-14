#include <iostream>
#include <grpcpp/grpcpp.h>
#include "calculator.idl.grpc.pb.h"
#include "calculator.idl.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using calculator::Calculator;
using calculator::CalculateRequest;
using calculator::CalculateResponse;

// Implementation of the Calculator service
class CalculatorServiceImpl final : public Calculator::Service {
    // Method to handle the Calculate RPC call
    Status Calculate(ServerContext* context, const CalculateRequest* request, CalculateResponse* response) override {
        // Extract the input values from the request
        int num1 = request->num1();
        int num2 = request->num2();
        std::string op = request->op();

        int result;
        // Perform the appropriate calculation based on the operator
        if (op == "+") {
            result = num1 + num2;
        } else if (op == "-") {
            result = num1 - num2;
        } else if (op == "*") {
            result = num1 * num2;
        } else if (op == "/") {
            result = num1 / num2;
        } else {
            // Return an error if the operator is invalid
            return Status(grpc::INVALID_ARGUMENT, "Invalid operator");
        }

        // Set the result in the response message
        response->set_result(result);
        return Status::OK;
    }
};

// Function to run the gRPC server
void RunServer() {
    std::string server_address("0.0.0.0:8000");
    CalculatorServiceImpl service;

    // Create the server builder
    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service); // Register the Calculator service implementation

    // Build and start the server
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait(); // Wait for the server to finish processing requests
}

int main() {
    RunServer(); // Start the gRPC server
    return 0;
}
