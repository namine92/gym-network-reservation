#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main ( int argc, char* argv[])
{
int socketfd, portno, n;
struct sockaddr_in serv_addr; // type
struct hostent *server;
char buffer[256],idClient[256];

//********************************
puts("Demarrage de l'application.....\n");

	if (argc<3)
		{fprintf(stderr,"specifier les arguments de %s \n ", argv[0]);exit(-1);}
	
	portno=atoi(argv[2]);

	
//********************************
puts("Initialisation de la connexion avec votre serveur.....\n");
	server =gethostbyname(argv[1]);
	if (server==NULL) 
		{printf ("erreur de nom serveur"); exit(-1);}

bzero (idClient,256);
	puts ("\n Merci de vous authentifier en donnant votre numero: \n"); 	
	fgets(idClient,255,stdin);

//********************************
while(1){
socketfd=socket(AF_INET, SOCK_STREAM,0);
	if (socketfd <0)	
		{ printf ("erreur de creation"); exit(-1);}


	bzero ((char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_port=htons(portno);
	memcpy (&serv_addr.sin_addr, server->h_addr,server->h_length);

	if(connect(socketfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
	{puts ("erreur de connection\n"); exit(-1);}
//********************************


	
	n=write(socketfd, idClient, strlen(idClient));

        bzero (buffer,256);
	puts ("\nDonner le nombre de sportifs a enregistrer : \n"); 	
	fgets(buffer,255,stdin);
	n=write(socketfd, buffer, strlen(buffer));
	if (n<0)
		{puts ("erreur de ecriture\n"); exit(-1);}
//********************************
puts("\n en attente de la reponse du serveur.....\n");
	bzero(buffer,256);
	n=read(socketfd, buffer, 255);

//********************************

	if (n<0)
		{printf ("erreur de lecture\n"); exit(-1);}
	printf("\nreponse: : %s\n\n", buffer);
puts("\n***************************************************************");
puts("\n***************************************************************\n");

}
//********************************

return 0;
}
