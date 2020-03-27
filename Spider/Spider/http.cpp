#define _CRT_SECURE_NO_WARNINGS 
#include<iostream>
#include<string>
#include<WinSock2.h>
// SOCKET���������������ļ�����Ҫ������İ�������ͷ�ļ���Ӧ��
#include "http.h"
#pragma comment(lib,"ws2_32.lib")
// ���캯��
CHttp::CHttp(){
	// �����׽��ֿ�
	WSADATA wd;
	// ��ʼ���׽��ֿ�
	WSAStartup(MAKEWORD(2, 2), &wd);

	// �����׽���
	// ʹ�õ���tcpЭ��-->SOCK_STREAM��ʾtcpЭ��
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
}
// ��������
CHttp::~CHttp(){
	// �����׽���
	WSACleanup();
}
// ����URL
bool CHttp::AnalyseURL(std::string url){


	// Ѱ��http�����Ƿ�����Ҽ� 
	if (std::string::npos == url.find("https://"))
		return false;

	// �ж�url�ĳ��ȣ�<= 8������
	if (url.length() <= 8){
		return false;
	}

	// ����
	int npos = url.find('/', 8);
	if (std::string::npos == npos){
		m_host = url.substr(8);
		m_object = "/";
	}else{
		m_host = url.substr(8,npos-8); // ����
		m_object = url.substr(npos); // ��Դ
	}
	if (m_host.empty()){
		return false;
	}


	return true;
}

//���ӷ�����
bool CHttp::Connect(){
	// ip��ַ
	// �˿ں�

	// ����IP��ַ
	// m_host.c_str()-->����C�����ַ���
	hostent* p = gethostbyname(m_host.c_str());
	if (NULL == p){
		return false;
	}

	// ���ӷ�����
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	// http ������Ĭ�϶˿�Ϊ80�˿�
	sa.sin_port = htons(80);
	memcpy(&sa.sin_addr,p->h_addr,4);
	if (SOCKET_ERROR == connect(m_socket, (sockaddr*)&sa, sizeof(sa)))
		return false;
	return true;
}

// ��ȡ��ҳ
bool CHttp::FetGet(std::string url, std::string& html){ // ��������
	
	// ����url
	if (false == AnalyseURL(url))
		return false;
	if (false == Connect()){
		return false;
	}

	// ����Get�����������
	
	// Fiddler �����ݰ�����:>

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

	// ���ܴӷ�������������
	char ch = 0;
	while (recv(m_socket,&ch,1,0)){
		html = html + ch;
	}
	return true;
}


// �ر��׽���
bool CHttp::Close(){
	closesocket(m_socket);
	m_host.empty();
	m_object.empty();
	m_socket = 0;
	return true;
}

// �����ļ�
bool CHttp::Download(std::string url, std::string filename){
	
	std::string get;
	get = get + "GET " + m_object + " HTTP/1.1\r\n";
	get = get + "Host: " + m_host + "\r\n";
	get = get + "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.149 Safari/537.36\r\n";
	get = get + "Connection : Close\r\n";
	get = get + "\r\n";

	if (SOCKET_ERROR == send(m_socket, get.c_str(), get.length(), 0))
		return false;

	// ���ļ�
	FILE* fp = fopen(filename.c_str(),"wb");
	if (fp  == NULL){
		return false;
	}

	// ����ͷ��Ϣ
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

	// ���ܴӷ�������������
	while (recv(m_socket, &ch, 1, 0)){
		fwrite(&ch,1,1,fp);
	}									
	
	// �ر��ļ�
	fclose(fp);
	return true;
}

