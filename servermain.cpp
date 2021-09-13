#include "header.hpp"
#include <vector>
int main(int argc, char const *argv[])
{
    boost::asio::io_service io_service;
    std::vector<std::string> stringContainer; 
    InfoShareServer server(7110,[&stringContainer](std::string&& s){
        std::string moved_string(std::move(s));
        std::cout << moved_string;
        stringContainer.push_back(moved_string);
    });
    std::cout << "server clieted\n";
    std::this_thread::sleep_for(3s);
    server.terminate();
    std::cout << "terminated ";
    std::this_thread::sleep_for(3s);
    for(const auto& itr:stringContainer)
        std::cout << itr << std::endl;


    //-----------送る
    std::cout << "wait 1s" << std::endl;
    std::this_thread::sleep_for(1s);
    std::cout << "\nend wait 1s\n" << std::endl;
    Client client("127.0.0.1",7111);

    
    for(int i = 0;i < stringContainer.size();++i)
        client.send(std::move(stringContainer.at(i)));
    std::string end("return end");
    // client.send(std::move(end));
    std::cout << "now return end status -> "<< end << " running\n";

    std::cout << "\n\nend !!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    return 0;
}
