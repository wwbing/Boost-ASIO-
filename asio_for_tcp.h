#pragma once

/*
    boost�µ�asio��������Ҫѧϰ�����̵Ĳ���
*/


//����ip��port�ҵ��Ķ˵�(Ŀ�ĵ�)
extern void create_endpoint();

//����socket
extern void create_socket();

//������������bind��ָ���ĵ�ַ
extern void bind();

//�󶨺�֮����У� ���� ���� �ͻ��˵�����
extern void accept_new_connection();

//�ͻ������ӵ�ָ����ip + port
extern void client_connect();

//���ӵ���������������������������
extern void dns_connect();

//�ͻ��˷������ݣ� socket.write_some()��socket.send()��asio::write(socket,data) ��
extern void client_write_some_or_send_or_write();

//�ͻ��˽������ݣ� socket.read_some()��socket.receive()��asio::read(socket,data) ��
extern void client_read_some_or_receive_or_read();

//asio�����紫��������
extern void use_const_buffer();