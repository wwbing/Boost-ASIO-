#include <iostream>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include "asio_for_tcp.h"

void create_endpoint() {

    //客户端寻找服务端的端点：ip和port
    {
        std::string ip_Det = "127.4.8.1";
        unsigned short port_num = 3333;

        boost::system::error_code ec;

        boost::asio::ip::address ip_add = boost::asio::ip::address::from_string(ip_Det, ec); //from_string函数

        if (ec.value() != 0) {
            std::cout << "ip解析错误 ErrCode = " << ec.value() << "Message is: " << ec.message() << std::endl;
        }

        //asio里的endpoint就是指定ip+port的那个端点
        boost::asio::ip::tcp::endpoint enc_point(ip_add, port_num);
    }



    //服务器绑定本地地址
    {
        unsigned short port_num = 3333;
        boost::asio::ip::address ip_add = boost::asio::ip::address_v4::any(); //服务器绑定本机任何ip地址

        boost::asio::ip::tcp::endpoint enc_point(ip_add, port_num);
    }
}


void create_socket() {

    //socket套接字就两种，用于服务器监听客户端连接的socket、用于相互通信的socket



    //客户端  通信socket
    {
        boost::asio::io_context ioc;                                //创建socket必须有上下文io_context

        boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4(); //指定tcp使用ipv4的协议

        boost::asio::ip::tcp::socket socket(ioc);       //利用ioc创建通信socket

        //检测是否创建成功
        boost::system::error_code ec;
        socket.open(protocol, ec);               //打开socket，创建好的socket是可以打开的，如果不能打开证明创建的socket有问题
        if (ec.value() != 0) {
            std::cout << "socket打开错误 ErrCode = " << ec.value() << "Message is: " << ec.message() << std::endl;
        }
    }


    //服务器  监听socket。
    {
        boost::asio::io_context ioc;             //创建socket必须有上下文io_context

        boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4(); //指定tcp使用ipv4的协议

        boost::asio::ip::tcp::acceptor socket_for_listen(ioc); //利用ioc创建监听socket

        //检测是否创建成功
        boost::system::error_code ec;
        socket_for_listen.open(protocol, ec);
        if (ec.value() != 0) {
            std::cout << "socket打开错误 ErrCode = " << ec.value() << "Message is: " << ec.message() << std::endl;
        }
    }
}


void bind() {
    boost::asio::io_context ioc;

    //bind只发生在服务器的监听：于是地址是本机的所有地址 和 指定端口
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), 3333);

    //生成对应协议的acceptor
    boost::asio::ip::tcp::acceptor acceptor(ioc, ep.protocol());

    //bind,绑定操作，可以根据错误码判断是否成功
    boost::system::error_code ec;
    acceptor.bind(ep, ec);

}


void accept_new_connection() {

    //本机所有地址
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), 3333);

    //创建监听器：监听器里面有监听套接字，不需要额外创建
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::acceptor acceptor(ioc, ep.protocol());

    //监听器绑定端点
    acceptor.bind(ep);

    //监听
    acceptor.listen(30);        //参数是最大连接数量

    //接受连接
    boost::asio::ip::tcp::socket c_socket(ioc);
    acceptor.accept(c_socket);

    //随后使用c_socket通信

}


void client_connect() {

    //服务器的ip和port
    std::string ip_Det = "192.168.1.124";
    unsigned short port_num = 3333;

    //端点地址
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip_Det), port_num);

    //创建socket
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::socket socket(ioc, ep.protocol()); //默认的协议是ipv4

    //连接
    socket.connect(ep);

    //随后进行通信

}


void use_const_buffer() {

    /*
        首先搞清楚基本存储结构：
            传递的数据就是 一个地址数组 + N个数据数组，地址数组每个元素都挂着一个数据数组，存放的是改数据数组的首地址。
            并且每个数据数组的第一个元素是数据的长度
    */


    {
        //基本方法
        std::string buf = "HELLO WORLD";                                //原始数据
        boost::asio::const_buffer asio_buf(buf.c_str(), buf.length());  //构造数据数组

        std::vector<boost::asio::const_buffer> buffer_sequence;         //将数据数组放到地址数组内
        buffer_sequence.push_back(asio_buf);

        //接下来可以将buffer_sequence发送到网络中
    }

    {
        //另一种方法:一步到位.
        //asio::buffer() 接收 首地址 + 数据长度 参数，也可以直接传入一个常量数据
        boost::asio::const_buffers_1 output_buf = boost::asio::buffer("HELLO WORLD");

        //接下来可以将output_buf发送到网络中 
    }

    {
        //传递的数据是数组的情况
        const std::size_t BUF_SIZE_BYTES = 30;  //std::size_t:无符号整型
        std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]);

        auto input_buf = boost::asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);
    }


}


void client_write_some_or_send_or_write() {

    //客户端使用socket.write_some()发送数据
    {
        std::string ip = "192.168.1.1";
        int port = 3333;
        
        //创建服务器端点目的地
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //创建socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);
        
        //客户端连接到指定的服务器
        socket.connect(ep);

        std::string str = "Hello world";
        std::size_t total_bytes_written = 0;

        //循环发送: 返回每次写入的字节数
        while (total_bytes_written != str.length()) {

            //准备数据：利用buffer函数构造buffer_sequence数据，参数：地址+数据长度
            boost::asio::const_buffers_1 data_sequence = boost::asio::buffer(str.c_str() + total_bytes_written, str.length() - total_bytes_written);

            //socket.write_some()接收一个buffer_sequence,返回成功写入的长度.没有发送完成会返回成功发送的字节数
            total_bytes_written += socket.write_some(data_sequence);
        }
    }

    //客户端使用socket.send()发送数据
    {
        std::string ip = "192.168.1.1";
        int port = 3333;

        //创建服务器端点目的地
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //创建socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);

        //客户端连接到指定的服务器
        socket.connect(ep);

        std::string str = "Hello world";
        std::size_t total_bytes_written = 0;

        //准备数据：利用buffer函数构造buffer_sequence数据，参数：地址+数据长度
        boost::asio::const_buffers_1 data_sequence = boost::asio::buffer(str.c_str() + total_bytes_written, str.length() - total_bytes_written);        
        
        //socket.send()接收一个buffer_sequence. 直到发送完成send才返回，否则一直阻塞等待发送完成.
        int send_length = socket.send(data_sequence);

        if (send_length < 0) {
            std::cout << "发送失败" << std::endl;
        }
    }


    //客户端使用全局函数write发送数据
    {
        std::string ip = "192.168.1.1";
        int port = 3333;

        //创建服务器端点目的地
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //创建socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);

        //客户端连接到指定的服务器
        socket.connect(ep);

        std::string str = "Hello world";
        std::size_t total_bytes_written = 0;

        //准备数据：利用buffer函数构造buffer_sequence数据，参数：地址+数据长度
        boost::asio::const_buffers_1 data_sequence = boost::asio::buffer(str.c_str() + total_bytes_written, str.length() - total_bytes_written);

        //使用全局write（）函数，发送数据。一直阻塞等待发送完成.（类似send）
        int write_length = boost::asio::write(socket, data_sequence);
    }

}


void client_read_some_or_receive_or_read() {


    //客户端使用socket.read_some（）读取网络数据
    {

        std::string ip = "192.168.1.1";
        int port = 3333;

        //创建服务器端点目的地
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //创建socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);

        //客户端连接到指定的服务器
        socket.connect(ep);

        const unsigned char MESSAGE_SIZE = 7;
        char buf[MESSAGE_SIZE];

        std::size_t total_bytes_read = 0;
        while (total_bytes_read != MESSAGE_SIZE) {

            //准备缓冲区：利用buffer函数构造buffer_sequence数据，参数：地址+数据长度
            boost::asio::mutable_buffers_1 local_sequence = boost::asio::buffer(buf + total_bytes_read, MESSAGE_SIZE - total_bytes_read);

            //没把本地的缓冲sequence读满就一直读:read_some()非阻塞，没读完也会返回当前已经读取的字节数
            total_bytes_read += socket.read_some(local_sequence);

        }

        //将最后的结果转为字符串
        std::string res(buf, total_bytes_read);
    }

    //客户端使用socket.receive（）读取网络数据
    {
        std::string ip = "192.168.1.1";
        int port = 3333;

        //创建服务器端点目的地
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //创建socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);

        //客户端连接到指定的服务器
        socket.connect(ep);

        const unsigned char MESSAGE_SIZE = 7;
        char buf[MESSAGE_SIZE];

        //准备缓冲区：利用buffer函数构造buffer_sequence数据，参数：地址+数据长度
        boost::asio::mutable_buffers_1 local_sequence = boost::asio::buffer(buf, MESSAGE_SIZE);

        //把缓冲区读满再返回，否则阻塞。  返回值为0：出错， 返回值为MESSAGE_SIZE：已经读满
        int receive_length = socket.receive(local_sequence);

        if (receive_length < 0) {
            std::cout << "读取错误" << std::endl;
        }

        //将最后的结果转为字符串
        std::string res(buf, MESSAGE_SIZE);

    }

    //客户端使用asio::read(socket, data_sequence)读取网络数据
    {
        std::string ip = "192.168.1.1";
        int port = 3333;

        //创建服务器端点目的地
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //创建socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);

        //客户端连接到指定的服务器
        socket.connect(ep);

        const unsigned char MESSAGE_SIZE = 7;
        char buf[MESSAGE_SIZE];

        //准备缓冲区：利用buffer函数构造buffer_sequence数据，参数：地址+数据长度
        boost::asio::mutable_buffers_1 local_sequence = boost::asio::buffer(buf, MESSAGE_SIZE);

        //把缓冲区读满再返回，否则阻塞。  返回值为0：出错， 返回值为MESSAGE_SIZE：已经读满
        int read_length = boost::asio::read(socket, local_sequence);

        if (read_length < 0) {
            std::cout << "读取错误" << std::endl;
        }

        //将最后的结果转为字符串
        std::string res(buf, MESSAGE_SIZE);
    }
}


void dns_connect() {

    //域名和地址
    std::string host = "www.baidu.com";
    std::string port = "3333";

    //dns请求
    boost::asio::ip::tcp::resolver::query dns_query(host, port, boost::asio::ip::tcp::resolver::query::numeric_service);

    //dns解析器的创建
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::resolver resolver(ioc);

    //解析:返回迭代器，可能有多个匹配的结果
    boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(dns_query);

    //连接到目标
    boost::asio::ip::tcp::socket socket(ioc);   //基于tcp，需要socket
    boost::asio::connect(socket, it);
}



int main()
{
    return 0;
}