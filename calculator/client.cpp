#include <iostream>
#include <grpcpp/grpcpp.h>
#include "calculator.idl.grpc.pb.h"
#include "calculator.idl.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using calculator::Calculator;
using calculator::CalculateRequest;
using calculator::CalculateResponse;

// Client class for making RPC calls to the calculator server
class CalculatorClient {
public:
    CalculatorClient(std::shared_ptr<Channel> channel)
        : stub_(Calculator::NewStub(channel)) {}

    // Method to perform a calculation by making an RPC call to the server
    int calculate(int num1, int num2, std::string op) {
        CalculateRequest request;
        request.set_num1(num1);
        request.set_num2(num2);
        request.set_op(op);

        CalculateResponse response;
        ClientContext context;

        // Call the RPC method on the server and get the response
        Status status = stub_->Calculate(&context, request, &response);
        if (status.ok()) {
            return response.result(); // Return the result if the RPC was successful
        } else {
            std::cout << "RPC failed: " << status.error_code() << ": " << status.error_message() << std::endl;
            return 0; // or handle the error in an appropriate way
        }
    }

private:
    std::unique_ptr<Calculator::Stub> stub_; // gRPC stub for making RPC calls
};

int main() {
    CalculatorClient client(grpc::CreateChannel("localhost:8000", grpc::InsecureChannelCredentials()));

    // Get input from user
    int num1, num2;
    std::string op;
    std::cout << "Enter first number: ";
    std::cin >> num1;
    std::cout << "Enter operator (+, -, *, /): ";
    std::cin >> op;
    std::cout << "Enter second number: ";
    std::cin >> num2;

    // Make an RPC call to the server to perform the calculation
    int result = client.calculate(num1, num2, op);
    std::cout << "Result: " << result << std::endl;

    return 0;
}
