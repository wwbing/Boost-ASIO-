#pragma once

/*
    boost下的asio，本文主要学习网络编程的部分
*/


//利用ip和port找到的端点(目的地)
extern void create_endpoint();

//创建socket
extern void create_socket();

//创建监听器并bind到指定的地址
extern void bind();

//绑定好之后进行： 监听 接收 客户端的连接
extern void accept_new_connection();

//客户端连接到指定的ip + port
extern void client_connect();

//连接到域名解析服务器进行域名解析
extern void dns_connect();

//客户端发送数据（ socket.write_some()、socket.send()、asio::write(socket,data) ）
extern void client_write_some_or_send_or_write();

//客户端接收数据（ socket.read_some()、socket.receive()、asio::read(socket,data) ）
extern void client_read_some_or_receive_or_read();

//asio的网络传输数据流
extern void use_const_buffer();