#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "registration.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using registration::RegistrationRequest;
using registration::RegistrationResponse;
using registration::RegistrationService;

class RegistrationServiceImpl final : public RegistrationService::Service {
    Status ProcessRegistration(ServerContext* context, const RegistrationRequest* request, RegistrationResponse* response) override {
        std::string registration = request->registration();
        std::string name = request->name();

        std::string message = "Registration: " + registration + "\nName: " + name + "\nData processed successfully.";
        response->set_message(message);

        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:8888");
    RegistrationServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}
