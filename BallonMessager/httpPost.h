#pragma once

#include <curl/curl.h>

#ifdef _DEBUG
#include <fstream>
const char * debugLogFile = "out.htm";
#endif

const char * serverPostUrl = "http://192.168.92.128/post.php";

class httpPost
{
public:
	httpPost(void);
	~httpPost(void);
	bool doPost(void);
private:
	// post���õ�ͷָ��,����curl_easy_setopt(curl, CURLOPT_HTTPPOST, post)�ĵ���������
	struct curl_httppost*post;
	// post�����βָ��,�������Ӳ������ݵ��ٶ�
	struct curl_httppost*last;

#ifdef _DEBUG
	std::ofstream ofs;

	//�����ݵ��д������������ʾ���صĿ������Ե�ű�ʾ��
	size_t OnData( void *ptr, size_t size, size_t nmemb, void *stream);
#endif
	
public:
	// Ϊpost����һ��field
	CURLFORMcode addField(const char* name, const char* value, const char* mime_type=NULL);
private:
	CURL* curl;
public:
	// initialize the post, if ok return true, otherwise return false
	bool init(void);
};
