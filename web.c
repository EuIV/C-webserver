#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
int main(){
	int sock_descr=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	const struct sockaddr_in addr_sock = {
		AF_INET,
		htons(25565),
		INADDR_ANY
	};
	if(bind(sock_descr, &addr_sock, sizeof(addr_sock))){
		fprintf(stderr,"%s","errbind\n");
		return 1;
	}

	if(listen(sock_descr, 100)){
		fprintf(stderr,"%s","errlisten\n");
		return 1;
	}
	while(1){
		struct sockaddr_in newsockaddr;
		socklen_t lennm = sizeof(newsockaddr);
		int newsock = accept(sock_descr, &newsockaddr, &lennm);
		char xesq[1001];
		read(newsock, &xesq[0], 999);
		xesq[1000] = 0;
		printf("%s",&xesq[0]);
	}	

}
