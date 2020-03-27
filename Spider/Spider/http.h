#define _CRT_SECURE_NO_WARNINGS 
#ifndef HTTP_H
#define HTTP_H

#include<string>
#include<WinSock2.h> // ����SOCKET
class CHttp{

public:
	std::string m_host;  // ������
	std::string m_object; // ·��
	SOCKET m_socket;    // �ͻ����׽��ֱ���
public:
	CHttp(); // ���캯��
	~CHttp(); // ��������
	bool AnalyseURL(std::string url); // ����URL
	bool Connect();  //���ӷ�����
	bool FetGet(std::string url,std::string& html);  // ��ȡ��ҳ
	bool Download(std::string url,std::string filename); // ����
	bool Close(); // �ر��׽���
};

#endif