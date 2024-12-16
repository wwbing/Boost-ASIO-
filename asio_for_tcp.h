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