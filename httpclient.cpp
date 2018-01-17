#include <iostream>
#include <string>
#include <curl/curl.h>
#include "httpclient.h"


HTTPClient::HTTPClient()
{
	init(0);
}

HTTPClient::HTTPClient(int keepalive)
{
	init(keepalive);
}

void HTTPClient::init(int keepalive){
	pthread_mutex_init(&m_mutex,NULL);
	curl_global_init(CURL_GLOBAL_DEFAULT);
	m_curl = curl_easy_init();
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1L); //禁止产生信号
	//https证书忽略
	curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
    //返回数据写入句柄
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, HTTPClient::writer);
	if(keepalive==1){
		curl_easy_setopt(m_curl,CURLOPT_TIMEOUT,0L);
		curl_easy_setopt(m_curl,CURLOPT_TCP_KEEPALIVE, 1L);
		curl_easy_setopt(m_curl,CURLOPT_TCP_KEEPIDLE, 120L);
		curl_easy_setopt(m_curl,CURLOPT_TCP_KEEPINTVL, 60L);
	}else{
		curl_easy_setopt(m_curl,CURLOPT_TIMEOUT,45L);
		curl_easy_setopt(m_curl,CURLOPT_FORBID_REUSE, 1L);
	}
}

HTTPClient::~HTTPClient()
{
	pthread_mutex_destroy(&m_mutex); 
	curl_global_cleanup();
}

HTTPResponse *HTTPClient::Get(string url)
{
	pthread_mutex_lock(&m_mutex);
	HTTPResponse *resp = new HTTPResponse();
	resp->code=200;
	resp->buff.clear();

	struct curl_slist *http_header = NULL;
	http_header = curl_slist_append(http_header, "Accept: *");
	http_header = curl_slist_append(http_header, "User-Agent: HB-HTTPCLI");
    
    curl_easy_setopt(m_curl,CURLOPT_URL,url.c_str());
	curl_easy_setopt(m_curl,CURLOPT_HTTPHEADER, http_header);//设置HTTP HEADER
	curl_easy_setopt(m_curl,CURLOPT_WRITEDATA, &resp->buff);
	CURLcode code = curl_easy_perform(m_curl);
	if(code !=CURLE_OK)
    {
    	resp->code = code;
    }
	curl_slist_free_all(http_header);//http_header需要释放
	pthread_mutex_unlock(&m_mutex); 
	return resp;
}

HTTPResponse *HTTPClient::Post(string url,string contentType,string data)
{
	pthread_mutex_lock(&m_mutex);  
	HTTPResponse *resp = new HTTPResponse();
	resp->code=200;

	struct curl_slist *http_header = NULL;
	http_header = curl_slist_append(http_header, "Accept: *");
	string tp=string("Content-Type: ")+contentType;
	http_header = curl_slist_append(http_header, tp.c_str());
	http_header = curl_slist_append(http_header, "Charset: utf-8");
	http_header = curl_slist_append(http_header, "User-Agent: HB-HTTPCLI");

	curl_easy_setopt(m_curl,CURLOPT_URL,url.c_str());
	curl_easy_setopt(m_curl,CURLOPT_HTTPHEADER, http_header);//设置HTTP HEADER
	curl_easy_setopt(m_curl,CURLOPT_POST,1L);
 	curl_easy_setopt(m_curl,CURLOPT_POSTFIELDS, data.c_str());

	curl_easy_setopt(m_curl,CURLOPT_WRITEDATA, &resp->buff);
	CURLcode code = curl_easy_perform(m_curl);
	if(code !=CURLE_OK)
    {
    	resp->code = code;
    }
	curl_slist_free_all(http_header);//http_header需要释放
	pthread_mutex_unlock(&m_mutex); 
	return resp;
}

HTTPResponse *HTTPClient::PostForm(string url,string data)
{
	pthread_mutex_lock(&m_mutex);  
	HTTPResponse *resp = new HTTPResponse();
	resp->code=200;

	struct curl_slist *http_header = NULL;
	http_header = curl_slist_append(http_header, "Accept: *");
	http_header = curl_slist_append(http_header, "application/x-www-form-urlencoded");
	http_header = curl_slist_append(http_header, "Charset: utf-8");
	http_header = curl_slist_append(http_header, "User-Agent: HB-HTTPCLI");

	curl_easy_setopt(m_curl,CURLOPT_URL,url.c_str());
	curl_easy_setopt(m_curl,CURLOPT_HTTPHEADER, http_header);//设置HTTP HEADER
	curl_easy_setopt(m_curl,CURLOPT_POST,1L);
 	curl_easy_setopt(m_curl,CURLOPT_POSTFIELDS, data.c_str());
	curl_easy_setopt(m_curl,CURLOPT_WRITEDATA, &resp->buff);
	CURLcode code = curl_easy_perform(m_curl);
	if(code !=CURLE_OK)
    {
    	resp->code = code;
    }
	curl_slist_free_all(http_header);//http_header需要释放
	pthread_mutex_unlock(&m_mutex); 
	return resp;
}

int HTTPClient::writer(char *data, size_t size, size_t nmemb,std::string *writerData)
{
  if(writerData == NULL)
    return 0;
 
  writerData->append(data, size*nmemb);
 
  return size * nmemb;
}