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

class RegistrationClient {
public:
    RegistrationClient(std::shared_ptr<Channel> channel)
        : stub_(RegistrationService::NewStub(channel)) {}

    std::string ProcessRegistration(const std::string& registration, const std::string& name) {
        RegistrationRequest request;
        request.set_registration(registration);
        request.set_name(name);

        RegistrationResponse response;
        ClientContext context;

        Status status = stub_->ProcessRegistration(&context, request, &response);

        if (status.ok()) {
            return response.message();
        } else {
            return "RPC failed.";
        }
    }

private:
    std::unique_ptr<RegistrationService::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string server_address("localhost:8888");
    RegistrationClient client(grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));

    std::string registration, name;
    std::cout << "Enter registration number: ";
    std::cin >> registration;
    std::cout << "Enter name: ";
    std::cin >> name;

    std::string response = client.ProcessRegistration(registration, name);
    std::cout << "Response from server: " << response << std::endl;

    return 0;
}
