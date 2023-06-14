#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "registration.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using registration::RegistrationRequest;
using registration::RegistrationResponse;
using registration::RegistrationService;

// Client class for making RPC calls to the Registration service
class RegistrationClient {
public:
    RegistrationClient(std::shared_ptr<Channel> channel)
        : stub_(RegistrationService::NewStub(channel)) {}

    // Method to process the registration request
    std::string ProcessRegistration(const std::string& registration, const std::string& name) {
        // Create the request message
        RegistrationRequest request;
        request.set_registration(registration);
        request.set_name(name);

        // Create the response object
        RegistrationResponse response;
        ClientContext context;

        // Make the RPC call to the server
        Status status = stub_->ProcessRegistration(&context, request, &response);

        if (status.ok()) {
            // Return the response message if the RPC call is successful
            return response.message();
        } else {
            // Return an error message if the RPC call fails
            return "RPC failed.";
        }
    }

private:
    std::unique_ptr<RegistrationService::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string server_address("localhost:8888");

    // Create a RegistrationClient instance with the gRPC channel
    RegistrationClient client(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    std::string registration, name;
    std::cout << "Enter registration number: ";
    std::cin >> registration;
    std::cout << "Enter name: ";
    std::cin >> name;

    // Make an RPC call to the server
    std::string response = client.ProcessRegistration(registration, name);
    std::cout << "Response from server: " << response << std::endl;

    return 0;
}
