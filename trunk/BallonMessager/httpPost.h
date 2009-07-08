#pragma once

#include <curl/curl.h>
#include <string>

#ifdef _DEBUG
#include <fstream>
#endif


class httpPost
{
public:
	httpPost(void);
	~httpPost(void);
	bool doPost(void);
private:
	// post���õ�ͷָ��,����curl_easy_setopt(curl, CURLOPT_HTTPPOST, post)�ĵ���������
	struct curl_httppost *m_pPost;
	// post�����βָ��,�������Ӳ������ݵ��ٶ�
	struct curl_httppost *m_pLast;

#ifdef _DEBUG
	std::ofstream ofs;
#endif

	//�����ݵ��д�������
	static size_t OnData( void *ptr, size_t size, size_t nmemb, void *pInstance);
	
public:
	// Ϊpost����һ��field
	bool addField(const char* name, const char* value, const char* mime_type=NULL);
private:
	CURL* m_pCurl;
public:
	// initialize the post, if ok return true, otherwise return false
	bool init(void);

	//ȡpost֮��,���������ص�����
	std::string getResult(void);
private:
	//post��result��д�����buffer����,����ͨ��getResult()ȡ��
	std::string resultBuffer;
};
