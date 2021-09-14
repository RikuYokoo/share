#include "header.hpp"
#include <vector>
#include <string>
using namespace  Citbrains::infosharemodule;

class TestClient{
    Client client;
    std::string send_string_data_, receive_string_data_;
    int send_times_, return_string_times_;
public:
    TestClient(std::string address, int32_t port) : client(address, port)
    {
    }

    void sendData()
    {
        for(int i=0;i<send_times_;++i)
        {
            std::string tmp_send_string_data = send_string_data_;
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

        return isCorrectSend();
    }

    bool isCorrectSend()
    {
        bool times = (return_string_times_ == send_times_);
        bool string = (send_string_data_ == receive_string_data_);
        if(times && string) return true;
        else return false;
    }

    void runServer(int count, std::string tmp_send_string, int return_port)
    {
        send_string_data_ = tmp_send_string;
        send_times_ = count;

        std::cout << "send string data = " << send_string_data_ << std::endl;
        std::cout << "send times = " << send_times_ << std::endl;
        
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
};


int main()
{
    TestClient test_client("127.0.0.1", 7110);

    test_client.runServer(5, "YokooRiku", 7111);

    test_client.receiveDataPrint();

    return 0;
}