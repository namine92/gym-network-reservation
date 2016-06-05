#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<pthread.h>

FILE *f;
int capacite,numc,nbrs,capaciteGen;
pthread_mutex_t S = PTHREAD_MUTEX_INITIALIZER ;

void *saveInscription(void *socket)
{
    int sock = *(int*)socket;
    int n;
    char *message , client_message[500],nombreSportif[256],numClient[256];
bzero (client_message,500);
bzero (numClient,256);
bzero (nombreSportif,256);
n=read(sock, numClient, 255);
n=read(sock, nombreSportif, 255);
nbrs = atoi(nombreSportif);
numc = atoi(numClient);
if( nbrs !=0){
printf("Traitement de la requette du client %d.......\n",numc);

	pthread_mutex_lock(&S);
f=fopen("historique.txt","a");
	if (capacite-nbrs>=0)
	{
    		capacite-=nbrs;
		fprintf (f,"\n%d %d succes",numc,nbrs);
		sprintf (client_message," membres inscrits avec succes");
                printf("Requette terminee avec succes!!\n");

	}
	else
	{
                fprintf (f,"\n%d %d echec",numc,nbrs);
		sprintf (client_message," desole il ne vous reste plus de place toutes les %d places sont pleine",capaciteGen);
                printf("Requette echoue!!\n");

	}

        	write(sock , client_message , strlen(client_message));

		fclose(f);
	pthread_mutex_unlock(&S);






    free(socket);
}

    return 0;
}




int main ( int argc, char* argv[])
{
int idClient = 1;
int socketfd, newsocketfd, portno, clilen, n , *new_sock;
struct sockaddr_in serv_addr, cli_addr; // type
struct hostent *server;
char buffer[256], bufferID[256];
//********************************
puts("Demarrage du serveur......\n");


if (argc<3)
	{fprintf(stderr,"specifier le port et le nombre de places disponibles");exit(-1);}
portno=atoi(argv[1]);
capacite=atoi(argv[2]);
capaciteGen=atoi(argv[2]);

socketfd=socket(AF_INET, SOCK_STREAM,0);
if (socketfd <0)	
	{ puts ("erreur de creation"); exit(-1);}

//********************************
puts("Initialisation des outils de connexion\n");

	bzero ((char*)&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s_addr= htonl(INADDR_ANY);
	serv_addr.sin_port=htons(portno);
	if(bind(socketfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
		{printf ("erreur de bind\n"); exit(-1);}
//********************************
puts("Serveur demarrer avec succes\n");
puts("En attente d'une nouvelle connexion d'un client.......\n");
while(1){
listen(socketfd,5);

//********************************



clilen=sizeof(cli_addr);
newsocketfd=accept(socketfd, (struct sockaddr *) & cli_addr, &clilen);
if (newsocketfd <0)	
	{ puts ("erreur de acceptation\n"); exit(-1);}

//********************************

   pthread_t commandeThread;
        new_sock = malloc(sizeof(int));
        *new_sock = newsocketfd;  
        if( pthread_create( &commandeThread , NULL ,  saveInscription , (void*) new_sock) < 0)
        {
            puts("erreur de la creation du thread");
             exit(-1);
        }
     }    

//********************************
printf("**************          Fin du serveur         **************\n");

return 0;
}
