#include "header.hpp"
#include "proto/send_data.pb.h"
#include <vector>
#include <string>
using namespace  Citbrains::infosharemodule;
using namespace InfoShare;

class TestClient{
    Client client;
    SendData send_data, return_send_data_;//protocolbuf
    std::string send_string_data_, receive_string_data_, serialize_data_;
    int send_times_, return_string_times_;
    int64_t now_time_;
public:
    TestClient(std::string address, int32_t port) : client(address, port)
    {
    }

    void sendData()
    {
        for(int i=0;i<send_times_;++i)
        {
            std::string tmp_send_string_data = serialize_data_;
            client.send(std::move(tmp_send_string_data));
        }
            //client.send(std::move(send_string_data_));

        std::cout << "end send-------------------" << std::endl;

    }

    bool receiveReturnData(int32_t port)
    {
        std::vector<std::string> stringContainer; 
        Server server(port,[&stringContainer](std::string&& s){
            std::string moved_string(std::move(s));
            std::cout << moved_string;
            stringContainer.push_back(moved_string);
        });

        std::this_thread::sleep_for(6s);
        if(!stringContainer.empty()){
            receive_string_data_ = stringContainer.at(0);
            return_string_times_ = stringContainer.size();
        }

        return_send_data_.ParseFromString(receive_string_data_);

        return isCorrectSend();
    }

    bool isCorrectSend()
    {
        bool times = (return_send_data_.now_time() == now_time_);
        bool string = (send_string_data_ == return_send_data_.send());
        if(times && string) return true;
        else return false;
    }

    void runServer(int count, std::string tmp_send_string, int return_port)
    {
        send_string_data_ = tmp_send_string;
        serialize_data_ = tmp_send_string;
        send_times_ = count;

        std::cout << "send string data = " << send_string_data_ << std::endl;
        std::cout << "before serialize_data = " << serialize_data_ << std::endl;
        std::cout << "send times = " << send_times_ << std::endl;
        setSerializeData();
        serializeSendData();
        //std::cout << "after serialize_data = " << serialize_data_ << std::endl;
        sendData();
        bool is_correct = receiveReturnData(return_port);

        if(is_correct) std::cout << "\n-----------------------------------------send correct-------------------------------------\n" << std::endl;
        else std::cout << "\n---------------------------send false-----------------------------------------\n" << std::endl;
    }

    void receiveDataPrint()
    {
        if(!receive_string_data_.empty())
        {
            std::cout << receive_string_data_ << std::endl;
        }else {
            std::cout << "Not data" << std::endl;
        }
    }

    void serializeSendData()//TODO:エラー処理
    {
        send_data.SerializeToString(&serialize_data_);
    }

    void setSerializeData()
    {
        send_data.set_now_time(getTime());
        send_data.set_send(send_string_data_);
    }

    int64_t getTime()
    {
        now_time_ = static_cast<int64_t> (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        std::cout << "now_time = " << now_time_ << std::endl;
        return now_time_;
    }
};


int main()
{
    TestClient test_client("127.0.0.1", 7110);
    //TestClient t3("127.0.0.1", 7110);

    test_client.runServer(5, "1YokooRiku", 7111);
    //t3.runServer(2, "testdata", 7113);

    test_client.receiveDataPrint();
    //std::cout << "t3 ------------\n";
    //t3.receiveDataPrint();

    return 0;
}