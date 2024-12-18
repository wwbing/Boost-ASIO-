// asio_tcp_server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <memory>
#include <set>
#include <thread>
#include <boost/asio.hpp>

using namespace std;
using namespace boost;

const int MAX_LENGTH = 1024;

typedef std::shared_ptr<asio::ip::tcp::socket> sock_ptr;
set<std::shared_ptr<thread>> thread_set;


//会话函数
void sync_session(sock_ptr sock) {
    try {
        while (1) {
            char data[MAX_LENGTH];
            memset(data, '\0', MAX_LENGTH);

            //服务器读取客户端的消息
            boost::system::error_code error;
            //size_t length = asio::read(sock, boost::asio::buffer(data, MAX_LENGTH), error);
            size_t length = sock->read_some(boost::asio::buffer(data, MAX_LENGTH), error);

            if (error == asio::error::eof) {
                cout << "连接被客户端断开" << endl;
                return;
            }
            else if(error){
                throw boost::system::system_error(error);
                return;
            }

            cout << "从客户端：" << sock->remote_endpoint().address().to_string() << "接收到： "<< data <<endl;

            //服务器回传给客户端
            asio::write(*sock, asio::buffer(data, length));
            cout << "回传客户端" << endl;

        }
    }
    catch (std::exception& e) {
        cerr << "Exception in thread: " << e.what() << endl;
    }
}

//连接函数
void sync_server(asio::io_context& ioc, unsigned short port) {
    //创建acceptor 绑定本地需要监听的地址
    asio::ip::tcp::acceptor a(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));

    while (1) {

        //创建socket 接收客户端的连接
        sock_ptr socket(new asio::ip::tcp::socket(ioc));
        a.accept(*socket);

        //开一个线程和这个客户端通信
        auto t = std::make_shared<std::thread>(sync_session, socket);

        //引用计数加1，这样t不会马上回收
        thread_set.insert(t);

    }

}

//同步通信服务器端
void sync_server_start() {

    cout << "Hello sync-server!\n";

    try {
        asio::io_context ioc;
        sync_server(ioc, 10086);

        for (auto& t : thread_set) {
            t->join();
        }

    }
    catch (std::exception& e) {
        cerr << "Exception in main: " << e.what() << endl;
    }
}


int main()
{
    sync_server_start();

    return 0;
}
