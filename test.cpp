#include "httpclient.h"
#include <unistd.h>

int main(){
	HTTPClient *cli = new HTTPClient(1);
	HTTPResponse *resp = cli->Get(string("https://api.haibeiclub.com:3101/"));
	cout<<resp->code<<endl;
	cout<<resp->buff<<endl;
	free(resp);
	
	sleep(120);
	cout<<"broken"<<endl;
	sleep(10000);
}