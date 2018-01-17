#include "httpclient.h"
#include <unistd.h>

int main(){
	HTTPClient *cli = new HTTPClient();
	HTTPResponse *resp = cli->Get(string("https://api.haibeiclub.com:3101/"));
	cout<<resp->code<<endl;
	cout<<resp->buff<<endl;
	delete(resp);
	delete(cli);
}