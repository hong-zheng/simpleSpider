#include<iostream>
#include<string>
using namespace std;
#include<queue>
#include<regex>
#include "http.h"
// ��ӭ����
void Welcome();
bool BeginCatch(string url);
int main(){
	Welcome();
	cout << "������Ҫץȡ��URL��ַ:>" << endl;
	string url;
	cin >> url;
	//cout << "\nurl = "<< url << endl;
	// ��ʼץȡ
	BeginCatch(url);

	system("pause");
	return 0;
}

// ��ӭ����
void Welcome(){
	cout << "*********************" << endl;
	cout << "****   Spider   *****" << endl;
	cout << "*********************" << endl;
}

// ��ʼץȡ
bool BeginCatch(string url){

	queue<string> q;
	q.push(url);
	while (!q.empty()){
		// ȡ��url
		string currentUrl = q.front();
		q.pop();

		
	
		// ��ȡ��ҳ����
		CHttp http;
		std::string html;
		http.FetGet(currentUrl,html);
		http.Close();
		// ��ȡ��ҳ֮����Ҫ�ص�

		//cout << html << endl;

		// 100-199 ��ʾ��Ϣ
		// 200-299 �ɹ�
		// 300-399 �ض���
		// 400-499 �ͻ��˷�������
		// 500-599 ��������������
		

		vector<string> vecImage;
		// ������ҳ����
		// ������ʽ
		smatch mat;      // \s��ʾ�հ׷� ���������ַ�����һ�λ���(+)
		regex rex("https://[^\\s'\"<>()]+");
		// ������
		string::const_iterator start = html.begin();
		string::const_iterator end = html.end();
		while (regex_search(start,end,rex)){
			string per(mat[0].first,mat[0].second);
			
			
			cout << per << endl;
		
			// ƥ�䵽��ͼƬ����
			if (per.find(".jpg") != string::npos ||
				per.find(".png") != string::npos ||
				per.find(".jpeg") != string::npos ){
				vecImage.push_back(per);
			}
			else{
				// ȥ�� w3c
				// w3c���ʵ��ٶȺ���
				if (per.find("https://www.w3.org") == string::npos){
					q.push(per);
				}
			}
			start = mat[0].second;
		}

		//����ͼƬ
		for (size_t i = 0; i < vecImage.size();i++){
			string filename = "C:\\Users\\zhenghong\\Pictures\\Camera Roll/"+vecImage[i].substr(vecImage[i].find_last_of('/')+1);
			CHttp httpDownload;
			if (false == httpDownload.Download(vecImage[i], filename))
				// ���Ӻ���һ�β�������
				cout << "����ʧ��" << GetLastError()<< endl;
			else
				cout << "���سɹ�" <<endl;
		}
	}

	return true;
}



