#include <iostream>
#include <string>
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


    //服务端一口气做完linux下的：指定ip和端口、创建套接字、绑定
    {
        using namespace boost::asio;


        /*
            注意：ip::tcp::v4()是协议，
            而本机的全部ipv4地址写法是：ip::address_v4::any();

            endpoint有两个重载的构造函数
                一个接收：     ip地址 + port端口
                另一个接收：   protocol协议 + port端口(默认所有的本机地址)
        */

        io_context ioc;
        ip::tcp::endpoint endpoint(ip::tcp::v4(), 3333);
        ip::tcp::acceptor bind(ioc, endpoint);
    }


    return 0;
}