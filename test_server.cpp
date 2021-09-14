#include "header.hpp"
#include <vector>
#include <string>
using namespace  Citbrains::infosharemodule;

class TestServer{
    //Server server;
    std::string return_ip_address_;
    int32_t return_port_;
    std::vector<std::string> stringContainer; 

    void sendReceiveData()
    {
        Client client(return_ip_address_, return_port_);
        int size = stringContainer.size();
        for(int i=0;i<size;i++)
        {
            client.send(std::move(stringContainer.at(i)));
        }

        std::cout << "return send end!!!!!!!!!!!!!!!!!" << std::endl;
    }

public:
    TestServer(int32_t port)
    {
        boost::asio::io_service io_service;
        std::vector<std::string> tmp_receiv_string_data;
        Server server(port, [&tmp_receiv_string_data](std::string&& s){
            std::string moved_string(std::move(s));
            std::cout << moved_string;
            tmp_receiv_string_data.push_back(moved_string);
        });
        std::cout << "server clieted\n";
        std::this_thread::sleep_for(5s);

        stringContainer = tmp_receiv_string_data;
    }

    void returnReceiveData(std::string address, int32_t port)
    {
        return_ip_address_ = address;
        return_port_ = port;

        sendReceiveData();
    }

    /*
    void sendReceiveData()
    {
        Client client(return_ip_address_, return_port_);
        int size = stringContainer.size();
        for(int i=0;i<size;i++)
        {
            client.send(std::move(stringContainer.at(i)));
        }

        std::cout << "return send end!!!!!!!!!!!!!!!!!" << std::endl;
    }
    */
};

int main()
{
    TestServer test_server(7110);

    //std::this_thread::sleep_for(3s);

    test_server.returnReceiveData("127.0.0.1", 7111);

    std::cout << "-----------------------end------------------------------" << std::endl;

    return 0;


}
