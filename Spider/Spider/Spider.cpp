#include<iostream>
#include<string>
using namespace std;
#include<queue>
#include<regex>
#include "http.h"
// 欢迎界面
void Welcome();
bool BeginCatch(string url);
int main(){
	Welcome();
	cout << "请输入要抓取的URL地址:>" << endl;
	string url;
	cin >> url;
	//cout << "\nurl = "<< url << endl;
	// 开始抓取
	BeginCatch(url);

	system("pause");
	return 0;
}

// 欢迎界面
void Welcome(){
	cout << "*********************" << endl;
	cout << "****   Spider   *****" << endl;
	cout << "*********************" << endl;
}

// 开始抓取
bool BeginCatch(string url){

	queue<string> q;
	q.push(url);
	while (!q.empty()){
		// 取出url
		string currentUrl = q.front();
		q.pop();

		
	
		// 获取网页数据
		CHttp http;
		std::string html;
		http.FetGet(currentUrl,html);
		http.Close();
		// 获取网页之后需要关掉

		//cout << html << endl;

		// 100-199 提示信息
		// 200-299 成功
		// 300-399 重定向
		// 400-499 客户端发生错误
		// 500-599 服务器发生错误
		

		vector<string> vecImage;
		// 解析网页内容
		// 正则表达式
		smatch mat;      // \s表示空白符 除开以下字符出现一次或多次(+)
		regex rex("https://[^\\s'\"<>()]+");
		// 迭代器
		string::const_iterator start = html.begin();
		string::const_iterator end = html.end();
		while (regex_search(start,end,rex)){
			string per(mat[0].first,mat[0].second);
			
			
			cout << per << endl;
		
			// 匹配到了图片链接
			if (per.find(".jpg") != string::npos ||
				per.find(".png") != string::npos ||
				per.find(".jpeg") != string::npos ){
				vecImage.push_back(per);
			}
			else{
				// 去除 w3c
				// w3c访问的速度很慢
				if (per.find("https://www.w3.org") == string::npos){
					q.push(per);
				}
			}
			start = mat[0].second;
		}

		//下载图片
		for (size_t i = 0; i < vecImage.size();i++){
			string filename = "C:\\Users\\zhenghong\\Pictures\\Camera Roll/"+vecImage[i].substr(vecImage[i].find_last_of('/')+1);
			CHttp httpDownload;
			if (false == httpDownload.Download(vecImage[i], filename))
				// 连接好了一次不再连接
				cout << "下载失败" << GetLastError()<< endl;
			else
				cout << "下载成功" <<endl;
		}
	}

	return true;
}



