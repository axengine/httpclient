#include <iostream>
#include <string>
#include <curl/curl.h>
#include <pthread.h>
using namespace std;

typedef struct tagResponse
{
	int code;//HTTP CODE 或者 错误代码
	string buff;//内容或错误信息
}HTTPResponse;

class HTTPClient
{
public:
	HTTPClient();
	HTTPClient(int keepalive);
	~HTTPClient();
    HTTPResponse *Get(string url);
    HTTPResponse *Post(string url,string contentType,string data);
    HTTPResponse *PostForm(string url,string data);
private:
	void init(int keepalive);
	pthread_mutex_t m_mutex;
	CURL *m_curl;
	static int writer(char *data, size_t size, size_t nmemb,std::string *writerData);
};