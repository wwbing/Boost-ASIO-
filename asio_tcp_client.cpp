// asio_tcp_client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost;

const int MAX_LENGTH = 1024;

int main()
{
    cout << "Hello sync-client!\n";


    try {
        //创建上下文服务
        asio::io_context ioc;
        
        //端点地址
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string("127.0.0.1"), 10086);

        //创建socket
        asio::ip::tcp::socket soc(ioc);

        system::error_code host_err = asio::error::host_not_found;

        //连接
        soc.connect(ep, host_err);

        if (host_err) {
            cout << "连接失败，错误码是： " << host_err.value() << "  错误信息是：" << host_err.message() << endl;
            return 0;
        }
        cout << "连接成功，输入信息发送给服务器" << endl;

        //准备数据
        char request[MAX_LENGTH];
        cin.getline(request, MAX_LENGTH);
        asio::mutable_buffers_1 req_sequence = asio::buffer(request, strlen(request));
      
        //发送
        size_t write_length = asio::write(soc, req_sequence);

        if (write_length) {
            cout << "已发送" << write_length << "字节的数据" << endl;
        }

        //读取服务器回复的数据
        char reply[MAX_LENGTH];
        asio::mutable_buffers_1 reply_sequence = asio::buffer(reply, MAX_LENGTH);

        size_t reply_length = asio::read(soc,reply_sequence);

        if (reply_length) {
            cout << "已接收" << reply_length << "字节的数据" << endl;
        }

    }
    catch (system::system_error& e) {

    }
    
    return 0;
}

