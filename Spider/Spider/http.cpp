#define _CRT_SECURE_NO_WARNINGS 
#include<iostream>
#include<string>
#include<WinSock2.h>
// SOCKET在上面所包含的文件里，因此要先上面的包含再在头文件中应用
#include "http.h"
#pragma comment(lib,"ws2_32.lib")
// 构造函数
CHttp::CHttp(){
	// 加载套接字库
	WSADATA wd;
	// 初始化套接字库
	WSAStartup(MAKEWORD(2, 2), &wd);

	// 创建套接字
	// 使用的是tcp协议-->SOCK_STREAM表示tcp协议
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
}
// 析构函数
CHttp::~CHttp(){
	// 清理套接字
	WSACleanup();
}
// 解析URL
bool CHttp::AnalyseURL(std::string url){


	// 寻找http，看是否可以找见 
	if (std::string::npos == url.find("https://"))
		return false;

	// 判断url的长度，<= 8不正常
	if (url.length() <= 8){
		return false;
	}

	// 域名
	int npos = url.find('/', 8);
	if (std::string::npos == npos){
		m_host = url.substr(8);
		m_object = "/";
	}else{
		m_host = url.substr(8,npos-8); // 主机
		m_object = url.substr(npos); // 资源
	}
	if (m_host.empty()){
		return false;
	}


	return true;
}

//连接服务器
bool CHttp::Connect(){
	// ip地址
	// 端口号

	// 解析IP地址
	// m_host.c_str()-->整成C语言字符串
	hostent* p = gethostbyname(m_host.c_str());
	if (NULL == p){
		return false;
	}

	// 连接服务器
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	// http 服务器默认端口为80端口
	sa.sin_port = htons(80);
	memcpy(&sa.sin_addr,p->h_addr,4);
	if (SOCKET_ERROR == connect(m_socket, (sockaddr*)&sa, sizeof(sa)))
		return false;
	return true;
}

// 获取网页
bool CHttp::FetGet(std::string url, std::string& html){ // 引用类型
	
	// 解析url
	if (false == AnalyseURL(url))
		return false;
	if (false == Connect()){
		return false;
	}

	// 发送Get请求向服务器
	
	// Fiddler 爬数据包如下:>

	//GET http ://image.baidu.com/search/acjson?tn=resultjson_com&ipn=rj&ct=201326592&is=&fp=result&queryWord=%E6%80%A7%E6%84%9F&cl=2&lm=-1&ie=utf-8&oe=utf-8&adpicid=&st=&z=&ic=&hd=&latest=&copyright=&word=%E6%80%A7%E6%84%9F&s=&se=&tab=&width=&height=&face=&istype=&qc=&nc=&fr=&expermode=&force=&pn=60&rn=30&gsm=3c&1585276391654= HTTP/1.1
	//Host : image.baidu.com
	//Connection : keep - alive
	//Accept : text / plain, */*; q=0.01
	//X-Requested-With: XMLHttpRequest
	//User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36
	//Referer: http://image.baidu.com/search/index?tn=baiduimage&ct=201326592&lm=-1&cl=2&ie=gb18030&word=%D0%D4%B8%D0&fr=ala&ala=1&alatpl=adress&pos=0&hs=2&xthttps=000000
	//Accept-Encoding: gzip, deflate
	//Accept-Language: zh-CN,zh;q=0.9
	//Cookie: winWH=%5E6_438x425; BDIMGISLOGIN=0; BDqhfp=%25D0%25D4%25B8%25D0%26%26NaN-1undefined%26%260%26%261; BIDUPSID=C38ADDFFF2EA191967D97E0349E4F8BE; PSTM=1567147659; BAIDUID=573D400F804D85EF55F289A01806B4F3:FG=1; BDUSS=hKTUxVTG9RZlVSS2VsbEtWMi1uVTBTNUdUTU1yNkowQzBqbHpoZFFBS2o2WDFlRVFBQUFBJCQAAAAAAAAAAAEAAAD40xDzAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAKNcVl6jXFZeeV; BDORZ=B490B5EBF6F3CD402E515D22BCDA1598; indexPageSugList=%5B%22%E7%83%AD%E8%A3%A4%20%E7%9F%AD%E8%A3%A4%22%2C%22%E7%BE%8E%E5%A5%B3%20%E7%83%AD%E8%A3%A4%22%2C%22%E7%BE%8E%E5%A5%B3%22%2C%22%E5%85%B0%E5%8D%9A%E5%9F%BA%E5%B0%BC%22%2C%22%E5%85%B0%E5%8D%9A%E5%9F%BA%E5%B0%BC%E5%9B%BE%E7%89%87%22%2C%22%E6%80%A7%E6%84%9F%20%22%2C%22%E6%A0%87%E8%AF%86%E7%AC%A6%22%2C%22%20%E7%BE%8E%E5%A5%B3.bmp%E6%A0%BC%E5%BC%8F%E5%9B%BE%E7%89%87%22%2C%22bitmap%20%E7%BE%8E%E5%A5%B3%22%5D; yjs_js_security_passport=0c4748155da0072c037c6c3d6efc269567c46426_1585204880_js; delPer=0; PSINO=7; BDRCVFR[feWj1Vr5u3D]=mk3SLVN4HKm; BDRCVFR[dG2JNJb_ajR]=mk3SLVN4HKm; BDRCVFR[tox4WRQ4-Km]=mk3SLVN4HKm; BDRCVFR[X_XKQks0S63]=mk3SLVN4HKm; H_PS_PSSID=1447_31169_21127_30839_31187_30824_31085_22160; userFrom=ala
	//

	std::string get;
	get = get + "GET "+m_object+" HTTP/1.1\r\n";
	get = get + "Host: " + m_host + "\r\n";
	get = get + "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36\r\n";
	get = get + "Connection : Close\r\n";
	get = get + "\r\n";

	if (SOCKET_ERROR == send(m_socket, get.c_str(), get.length(), 0))
		return false;

	// 接受从服务器来的数据
	char ch = 0;
	while (recv(m_socket,&ch,1,0)){
		html = html + ch;
	}
	return true;
}


// 关闭套接字
bool CHttp::Close(){
	closesocket(m_socket);
	m_host.empty();
	m_object.empty();
	m_socket = 0;
	return true;
}

// 下载文件
bool CHttp::Download(std::string url, std::string filename){
	
	std::string get;
	get = get + "GET " + m_object + " HTTP/1.1\r\n";
	get = get + "Host: " + m_host + "\r\n";
	get = get + "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36\r\n";
	get = get + "Connection : Close\r\n";
	get = get + "\r\n";

	if (SOCKET_ERROR == send(m_socket, get.c_str(), get.length(), 0))
		return false;

	// 打开文件
	FILE* fp = fopen(filename.c_str(),"wb");
	if (fp  == NULL){
		return false;
	}

	// 接受头信息
	char ch = 0;
	while (recv(m_socket, &ch, 1, 0)){
		if (ch == '\r'){
			recv(m_socket, &ch, 1, 0);
			if (ch == '\n'){
				recv(m_socket, &ch, 1, 0);
				if (ch == '\r'){
					recv(m_socket, &ch, 1, 0);
					if (ch == '\n'){
						break;
					}

				}
			}

		}
	}

	// 接受从服务器来的数据
	while (recv(m_socket, &ch, 1, 0)){
		fwrite(&ch,1,1,fp);
	}									
	
	// 关闭文件
	fclose(fp);
	return true;
}

