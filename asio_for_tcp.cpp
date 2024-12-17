#include <iostream>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include "asio_for_tcp.h"

void create_endpoint() {

    //�ͻ���Ѱ�ҷ���˵Ķ˵㣺ip��port
    {
        std::string ip_Det = "127.4.8.1";
        unsigned short port_num = 3333;

        boost::system::error_code ec;

        boost::asio::ip::address ip_add = boost::asio::ip::address::from_string(ip_Det, ec); //from_string����

        if (ec.value() != 0) {
            std::cout << "ip�������� ErrCode = " << ec.value() << "Message is: " << ec.message() << std::endl;
        }

        //asio���endpoint����ָ��ip+port���Ǹ��˵�
        boost::asio::ip::tcp::endpoint enc_point(ip_add, port_num);
    }



    //�������󶨱��ص�ַ
    {
        unsigned short port_num = 3333;
        boost::asio::ip::address ip_add = boost::asio::ip::address_v4::any(); //�������󶨱����κ�ip��ַ

        boost::asio::ip::tcp::endpoint enc_point(ip_add, port_num);
    }
}


void create_socket() {

    //socket�׽��־����֣����ڷ����������ͻ������ӵ�socket�������໥ͨ�ŵ�socket



    //�ͻ���  ͨ��socket
    {
        boost::asio::io_context ioc;                                //����socket������������io_context

        boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4(); //ָ��tcpʹ��ipv4��Э��

        boost::asio::ip::tcp::socket socket(ioc);       //����ioc����ͨ��socket

        //����Ƿ񴴽��ɹ�
        boost::system::error_code ec;
        socket.open(protocol, ec);               //��socket�������õ�socket�ǿ��Դ򿪵ģ�������ܴ�֤��������socket������
        if (ec.value() != 0) {
            std::cout << "socket�򿪴��� ErrCode = " << ec.value() << "Message is: " << ec.message() << std::endl;
        }
    }


    //������  ����socket��
    {
        boost::asio::io_context ioc;             //����socket������������io_context

        boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4(); //ָ��tcpʹ��ipv4��Э��

        boost::asio::ip::tcp::acceptor socket_for_listen(ioc); //����ioc��������socket

        //����Ƿ񴴽��ɹ�
        boost::system::error_code ec;
        socket_for_listen.open(protocol, ec);
        if (ec.value() != 0) {
            std::cout << "socket�򿪴��� ErrCode = " << ec.value() << "Message is: " << ec.message() << std::endl;
        }
    }
}


void bind() {
    boost::asio::io_context ioc;

    //bindֻ�����ڷ������ļ��������ǵ�ַ�Ǳ��������е�ַ �� ָ���˿�
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), 3333);

    //���ɶ�ӦЭ���acceptor
    boost::asio::ip::tcp::acceptor acceptor(ioc, ep.protocol());

    //bind,�󶨲��������Ը��ݴ������ж��Ƿ�ɹ�
    boost::system::error_code ec;
    acceptor.bind(ep, ec);

}


void accept_new_connection() {

    //�������е�ַ
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), 3333);

    //�����������������������м����׽��֣�����Ҫ���ⴴ��
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::acceptor acceptor(ioc, ep.protocol());

    //�������󶨶˵�
    acceptor.bind(ep);

    //����
    acceptor.listen(30);        //�����������������

    //��������
    boost::asio::ip::tcp::socket c_socket(ioc);
    acceptor.accept(c_socket);

    //���ʹ��c_socketͨ��

}


void client_connect() {

    //��������ip��port
    std::string ip_Det = "192.168.1.124";
    unsigned short port_num = 3333;

    //�˵��ַ
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip_Det), port_num);

    //����socket
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::socket socket(ioc, ep.protocol()); //Ĭ�ϵ�Э����ipv4

    //����
    socket.connect(ep);

    //������ͨ��

}


void use_const_buffer() {

    /*
        ���ȸ���������洢�ṹ��
            ���ݵ����ݾ��� һ����ַ���� + N���������飬��ַ����ÿ��Ԫ�ض�����һ���������飬��ŵ��Ǹ�����������׵�ַ��
            ����ÿ����������ĵ�һ��Ԫ�������ݵĳ���
    */


    {
        //��������
        std::string buf = "HELLO WORLD";                                //ԭʼ����
        boost::asio::const_buffer asio_buf(buf.c_str(), buf.length());  //������������

        std::vector<boost::asio::const_buffer> buffer_sequence;         //����������ŵ���ַ������
        buffer_sequence.push_back(asio_buf);

        //���������Խ�buffer_sequence���͵�������
    }

    {
        //��һ�ַ���:һ����λ.
        //asio::buffer() ���� �׵�ַ + ���ݳ��� ������Ҳ����ֱ�Ӵ���һ����������
        boost::asio::const_buffers_1 output_buf = boost::asio::buffer("HELLO WORLD");

        //���������Խ�output_buf���͵������� 
    }

    {
        //���ݵ���������������
        const std::size_t BUF_SIZE_BYTES = 30;  //std::size_t:�޷�������
        std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]);

        auto input_buf = boost::asio::buffer(static_cast<void*>(buf.get()), BUF_SIZE_BYTES);
    }


}


void client_write_some_or_send_or_write() {

    //�ͻ���ʹ��socket.write_some()��������
    {
        std::string ip = "192.168.1.1";
        int port = 3333;

        //�����������˵�Ŀ�ĵ�
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //����socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);

        //�ͻ������ӵ�ָ���ķ�����
        socket.connect(ep);

        std::string str = "Hello world";
        std::size_t total_bytes_written = 0;

        //ѭ������: ����ÿ��д����ֽ���
        while (total_bytes_written != str.length()) {

            //׼�����ݣ�����buffer��������buffer_sequence���ݣ���������ַ+���ݳ���
            boost::asio::const_buffers_1 data_sequence = boost::asio::buffer(str.c_str() + total_bytes_written, str.length() - total_bytes_written);

            //socket.write_some()����һ��buffer_sequence,���سɹ�д��ĳ���.û�з�����ɻ᷵�سɹ����͵��ֽ���
            total_bytes_written += socket.write_some(data_sequence);
        }
    }

    //�ͻ���ʹ��socket.send()��������
    {
        std::string ip = "192.168.1.1";
        int port = 3333;

        //�����������˵�Ŀ�ĵ�
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //����socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);

        //�ͻ������ӵ�ָ���ķ�����
        socket.connect(ep);

        std::string str = "Hello world";
        std::size_t total_bytes_written = 0;

        //׼�����ݣ�����buffer��������buffer_sequence���ݣ���������ַ+���ݳ���
        boost::asio::const_buffers_1 data_sequence = boost::asio::buffer(str.c_str() + total_bytes_written, str.length() - total_bytes_written);

        //socket.send()����һ��buffer_sequence. ֱ���������send�ŷ��أ�����һֱ�����ȴ��������.
        int send_length = socket.send(data_sequence);

        if (send_length < 0) {
            std::cout << "����ʧ��" << std::endl;
        }
    }


    //�ͻ���ʹ��ȫ�ֺ���write��������
    {
        std::string ip = "192.168.1.1";
        int port = 3333;

        //�����������˵�Ŀ�ĵ�
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //����socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);

        //�ͻ������ӵ�ָ���ķ�����
        socket.connect(ep);

        std::string str = "Hello world";
        std::size_t total_bytes_written = 0;

        //׼�����ݣ�����buffer��������buffer_sequence���ݣ���������ַ+���ݳ���
        boost::asio::const_buffers_1 data_sequence = boost::asio::buffer(str.c_str() + total_bytes_written, str.length() - total_bytes_written);

        //ʹ��ȫ��write�����������������ݡ�һֱ�����ȴ��������.������send��
        int write_length = boost::asio::write(socket, data_sequence);
    }

}


void client_read_some_or_receive_or_read() {


    //�ͻ���ʹ��socket.read_some������ȡ��������
    {

        std::string ip = "192.168.1.1";
        int port = 3333;

        //�����������˵�Ŀ�ĵ�
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //����socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);

        //�ͻ������ӵ�ָ���ķ�����
        socket.connect(ep);

        const unsigned char MESSAGE_SIZE = 7;
        char buf[MESSAGE_SIZE];

        std::size_t total_bytes_read = 0;
        while (total_bytes_read != MESSAGE_SIZE) {

            //׼��������������buffer��������buffer_sequence���ݣ���������ַ+���ݳ���
            boost::asio::mutable_buffers_1 local_sequence = boost::asio::buffer(buf + total_bytes_read, MESSAGE_SIZE - total_bytes_read);

            //û�ѱ��صĻ���sequence������һֱ��:read_some()��������û����Ҳ�᷵�ص�ǰ�Ѿ���ȡ���ֽ���
            total_bytes_read += socket.read_some(local_sequence);

        }

        //�����Ľ��תΪ�ַ���
        std::string res(buf, total_bytes_read);
    }

    //�ͻ���ʹ��socket.receive������ȡ��������
    {
        std::string ip = "192.168.1.1";
        int port = 3333;

        //�����������˵�Ŀ�ĵ�
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //����socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);

        //�ͻ������ӵ�ָ���ķ�����
        socket.connect(ep);

        const unsigned char MESSAGE_SIZE = 7;
        char buf[MESSAGE_SIZE];

        //׼��������������buffer��������buffer_sequence���ݣ���������ַ+���ݳ���
        boost::asio::mutable_buffers_1 local_sequence = boost::asio::buffer(buf, MESSAGE_SIZE);

        //�ѻ����������ٷ��أ�����������  ����ֵΪ0������ ����ֵΪMESSAGE_SIZE���Ѿ�����
        int receive_length = socket.receive(local_sequence);

        if (receive_length < 0) {
            std::cout << "��ȡ����" << std::endl;
        }

        //�����Ľ��תΪ�ַ���
        std::string res(buf, MESSAGE_SIZE);

    }

    //�ͻ���ʹ��asio::read(socket, data_sequence)��ȡ��������
    {
        std::string ip = "192.168.1.1";
        int port = 3333;

        //�����������˵�Ŀ�ĵ�
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ip), port);

        //����socket
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket socket(ioc);

        //�ͻ������ӵ�ָ���ķ�����
        socket.connect(ep);

        const unsigned char MESSAGE_SIZE = 7;
        char buf[MESSAGE_SIZE];

        //׼��������������buffer��������buffer_sequence���ݣ���������ַ+���ݳ���
        boost::asio::mutable_buffers_1 local_sequence = boost::asio::buffer(buf, MESSAGE_SIZE);

        //�ѻ����������ٷ��أ�����������  ����ֵΪ0������ ����ֵΪMESSAGE_SIZE���Ѿ�����
        int read_length = boost::asio::read(socket, local_sequence);

        if (read_length < 0) {
            std::cout << "��ȡ����" << std::endl;
        }

        //�����Ľ��תΪ�ַ���
        std::string res(buf, MESSAGE_SIZE);
    }
}


void dns_connect() {

    //�����͵�ַ
    std::string host = "www.baidu.com";
    std::string port = "3333";

    //dns����
    boost::asio::ip::tcp::resolver::query dns_query(host, port, boost::asio::ip::tcp::resolver::query::numeric_service);

    //dns�������Ĵ���
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::resolver resolver(ioc);

    //����:���ص������������ж��ƥ��Ľ��
    boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(dns_query);

    //���ӵ�Ŀ��
    boost::asio::ip::tcp::socket socket(ioc);   //����tcp����Ҫsocket
    boost::asio::connect(socket, it);
}
