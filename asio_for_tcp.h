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