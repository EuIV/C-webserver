#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <errno.h>
char *readFile(const char *path){
	char * buf = malloc(1025);
	int seek_bytes = 1024;
	int file = open(path,O_RDONLY);
	for(ssize_t bytes_read = read(file,buf,1024);bytes_read > 0; bytes_read = read(file,buf,1024)){
		lseek(file, seek_bytes, SEEK_SET);
		seek_bytes+=1024;
		buf=realloc(buf,seek_bytes+1);
	}
	*(buf+seek_bytes+1)=0;
	close(file);
	return buf;
}
int main(){
	int input_socket=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	const struct sockaddr_in addr_sock = {
		AF_INET,
		htons(8080),
		INADDR_ANY
	};
	if(bind(input_socket, &addr_sock, sizeof(addr_sock))){
		fprintf(stderr,"%s","errbind\n");
		return 1;
	}

	if(listen(input_socket, 100)){
		fprintf(stderr,"%s","errlisten\n");
		return 1;
	}
	while(1){
		struct sockaddr_in client_adress;
		socklen_t client_adress_len = sizeof(client_adress);
		int accepted_socket = accept(input_socket, &client_adress, &client_adress_len);
		char client_headers[1001];
		read(accepted_socket, &client_headers[0], 999);
		client_headers[1000] = 0;
		printf("%s",&client_headers[0]);
		char headers[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n";
		write(accepted_socket, &headers, sizeof(headers));
		char*read_file=readFile("./www/index.html");
		write(accepted_socket, read_file, strlen(read_file)+1);
		write(accepted_socket, "\r\n\r\n", 4);
		close(accepted_socket);
	}
}
