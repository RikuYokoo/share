#include "header.hpp"

int main(int argc, char const *argv[])
{

    Client client("127.0.0.1",7110);

    
    for(int i = 0;i < 5;++i)
        client.send(std::string("RikuYokoo"));
    std::string end("end");
    // client.send(std::move(end));
    std::cout << "now end status ->"<< end << "running\n";

    std::cout << "\n\nend !!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;


//-----------------------------送ったやつを受け取る-----------
    boost::asio::io_service io_service;
    std::vector<std::string> stringContainer; 
   // std::this_thread::sleep_for(1s);

    InfoShareServer server(7111,[&stringContainer](std::string&& s){
        std::string moved_string(std::move(s));
        std::cout << moved_string;
        stringContainer.push_back(moved_string);
    });

    std::this_thread::sleep_for(7s);
    std::cout << "\nreturn server!!!!!!!!!!!!!\n\n";
//    std::this_thread::sleep_for(5s);
    server.terminate();
    std::cout << "terminated ";
    std::this_thread::sleep_for(3s);
    std::cout << "-------------output-------------" << std::endl;
    std::cout << "\n\n" << std::boolalpha << stringContainer.empty() << std::endl;
    for(const auto& itr:stringContainer)
        std::cout << itr << std::endl;
    return 0;
}
