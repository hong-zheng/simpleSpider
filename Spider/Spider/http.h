#define _CRT_SECURE_NO_WARNINGS 
#ifndef HTTP_H
#define HTTP_H

#include<string>
#include<WinSock2.h> // 包含SOCKET
class CHttp{

public:
	std::string m_host;  // 主机名
	std::string m_object; // 路径
	SOCKET m_socket;    // 客户端套接字保存
public:
	CHttp(); // 构造函数
	~CHttp(); // 析构函数
	bool AnalyseURL(std::string url); // 解析URL
	bool Connect();  //连接服务器
	bool FetGet(std::string url,std::string& html);  // 获取网页
	bool Download(std::string url,std::string filename); // 下载
	bool Close(); // 关闭套接字
};

#endif