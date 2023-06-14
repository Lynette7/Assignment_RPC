#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "registration.grpc.pb.h"

// Import necessary namespaces and protobuf generated code
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using registration::RegistrationRequest;
using registration::RegistrationResponse;
using registration::RegistrationService;

// Implementation of the RegistrationService
class RegistrationServiceImpl final : public RegistrationService::Service {
    // Implementation of the ProcessRegistration RPC method
    Status ProcessRegistration(ServerContext* context, const RegistrationRequest* request, RegistrationResponse* response) override {
        // Extract registration and name from the request
        std::string registration = request->registration();
        std::string name = request->name();

        // Construct the response message
        std::string message = "Registration: " + registration + "\nName: " + name + "\nData processed successfully.";
        response->set_message(message);

        // Indicate that the RPC call was successful
        return Status::OK;
    }
};

// Function to run the gRPC server
void RunServer() {
    // Define the server address
    std::string server_address("0.0.0.0:8888");

    // Create an instance of the RegistrationServiceImpl
    RegistrationServiceImpl service;

    // Create a gRPC server builder
    ServerBuilder builder;

    // Add the server address and credentials to the builder
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    // Register the RegistrationServiceImpl with the builder
    builder.RegisterService(&service);

    // Build and start the server
    std::unique_ptr<Server> server(builder.BuildAndStart());

    // Print the server address
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to finish (blocking call)
    server->Wait();
}

// Main function
int main(int argc, char** argv) {
    // Run the gRPC server
    RunServer();

    // Exit the program
    return 0;
}
