#pragma once

#include <curl/curl.h>

class httpPost
{
public:
	httpPost(void);
	~httpPost(void);
	void doPost(void);
private:
	// post���õ�ͷָ��,����curl_easy_setopt(curl, CURLOPT_HTTPPOST, post)�ĵ���������
	struct curl_httppost*post;
	// post�����βָ��,�������Ӳ������ݵ��ٶ�
	struct curl_httppost*last;
public:
	// Ϊpost����һ��field
	CURLFORMcode addField(const char* name, const char* value, const char* mime_type);
};
