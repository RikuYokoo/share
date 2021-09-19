#include "header.hpp"
#include "proto/send_data.pb.h"
#include <vector>
#include <string>
#include <algorithm>
using namespace  Citbrains::infosharemodule;
using namespace InfoShare;
class TestServer{
    //Server server;
    SendData send_data;
    std::string return_ip_address_, serial_string_;
    int32_t return_port_;
    std::vector<std::string> stringContainer; 
    int64_t recv_time_, now_time_;

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
        std::vector<int64_t> tmp_now_time;
        Server server(port, [&tmp_receiv_string_data, &tmp_now_time](std::string&& s){
            std::string moved_string(std::move(s));
            std::cout << moved_string;
            tmp_now_time.push_back(static_cast<int64_t> (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()));
            tmp_receiv_string_data.push_back(moved_string);
        });
        std::cout << "server clieted\n";
        std::this_thread::sleep_for(5s);

        stringContainer = tmp_receiv_string_data;
        serial_string_ = tmp_receiv_string_data.at(0);
        //recv_time_ = *min_element(tmp_now_time.begin(), tmp_now_time.end());
        recv_time_ = *max_element(tmp_now_time.begin(), tmp_now_time.end());

    }

    void returnReceiveData(std::string address, int32_t port)
    {
        return_ip_address_ = address;
        return_port_ = port;

        sendReceiveData();
    }

    void praseSerialdata()
    {
        send_data.ParseFromString(serial_string_);
    }

    void isCheck()
    {
        std::cout << "ischeck now_time = " << send_data.now_time() << std::endl;
        std::cout << "ischeck send_data = " << send_data.send() << std::endl;
    }

    int64_t getTime()
    {
        now_time_ = static_cast<int64_t> (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        return now_time_;
    }

    int64_t calcComunicateTime()
    {
        return recv_time_ - send_data.now_time();
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
    //TestServer t3(7112);

    //std::this_thread::sleep_for(3s);

    test_server.returnReceiveData("127.0.0.1", 7111);
    //t3.returnReceiveData("127.0.0.1", 7113);
    std::cout << "aaa-----------------------end------------------------------" << std::endl;

    test_server.praseSerialdata();
    test_server.isCheck();

    std::cout << "通信時間 : " << test_server.calcComunicateTime() << "[ms]" << std::endl;
    
    
    std::cout << "-------------end--------------" << std::endl;

    return 0;


}
