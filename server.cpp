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

class CalculatorServiceImpl final : public Calculator::Service {
    Status Calculate(ServerContext* context, const CalculateRequest* request, CalculateResponse* response) override {
        int num1 = request->num1();
        int num2 = request->num2();
        std::string op = request->op();

        int result;
        if (op == "+") {
            result = num1 + num2;
        } else if (op == "-") {
            result = num1 - num2;
        } else if (op == "*") {
            result = num1 * num2;
        } else if (op == "/") {
            result = num1 / num2;
        } else {
            return Status(grpc::INVALID_ARGUMENT, "Invalid operator");
        }

        response->set_result(result);
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:8000");
    CalculatorServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main() {
    RunServer();
    return 0;
}
