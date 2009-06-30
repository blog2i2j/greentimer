// TestCUrl.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <curl/curl.h>
using namespace std;

//�յ�������Ϣ����ʾ���ؽ���
int OnProgress(void *clientp, double dltotal, double
        dlnow, double ultotal, double ulnow)
{
	int iLen = dlnow*60/dltotal;
	for(int i=0;i<60;i++)
	{
		if(i<=iLen) cout<<"/";
		else cout<<"-";
	}
	cout<<endl;
    return 0;
}

//�����ݵ��д������������ʾ���صĿ������Ե�ű�ʾ��
size_t OnData( void *ptr, size_t size, size_t nmemb, void *stream)
{
	//cout<<(char *)ptr<<endl;
	/*for(int i=0;i<2*size*nmemb/512;i++)
	{
		cout<<".";
	}
	cout<<endl;*/

	ofstream &ofs = *(ofstream *)stream;
	ofs.write((char *)ptr,size*nmemb);
	return size*nmemb;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc!=3)
	{
		cout<<"������url�ͱ����ַ����������"<<endl;
		return 0;
	}
	char *strUrl = argv[1];
	char *strSavePath = argv[2];

	ofstream ofs(strSavePath,ios_base::binary);
	CURL *curl;
	CURLcode res;

	//curl��ʼ��
	curl = curl_easy_init();
	if(!curl) {
		return 0;
	}

	//����Ҫ���ص�URL
	curl_easy_setopt(curl, CURLOPT_URL, strUrl);

	//�������ݵ���ص�����
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs); //���Ǵ��ݸ�OnData�ĵ�һ������

	//�������ؽ��Ȼص�����
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS,FALSE);  //�������˹���
	curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, OnProgress);

	cout<<"��ʼ���أ�"<<strUrl<<endl;
	cout<<"����λ�ã�"<<strSavePath<<endl;

	//����ִ�����ز������������һ���߳��е��ã����Դﵽ��������Ŀ�ģ�
	res = curl_easy_perform(curl);
	if(0!=res)
	{
		return 0;
	}

	//�˳�CURL
	curl_easy_cleanup(curl);

	ofs.flush();
	cout<<"������ɣ�"<<strSavePath<<endl;
	return 0;
}

