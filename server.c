#if defined WIN32
#include <winsock.h>


#else
// MAC /UNIX

#define closesocket close
#include <sys/socket.h> //per socket(), bind() connect()
#include <arpa/inet.h> // per close()
#include <unistd.h> // per sockaddrin
#include <string.h>
#include <netdb.h>
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define PORT 3015
#define QLEN 7
char stringa[50] = "";
struct Struct
{
    float a;
    float b;
    float operazione;
};
struct Struct msgStruct;

int main(){
    //INIZIALIZZASIONE LIBRERIA
#if defined WIN32
    //wsa data serve per inizializzare la libreria winsock in ambiente windows
    WSADATA wsaData;
    WORD wVersionRequested;
    wVersionRequested = MAKEWORD(2, 2);

    int iResult = WSAStartup(wVersionRequested, &wsaData);
    if (iResult != 0) {
        printf("errore inizializzaione libreria winsock\n\n");
    }



#endif


    int mysocket;
    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;
    int clientAddrLen;


    //creazione socket
    if((mysocket = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP))<0)
    {
        printf("errore creazione socket\n\n");
    }else
        printf("server in asscolto\n");




    //CREAZIONE INDIRIZZO SERVER

    servaddr.sin_family= AF_INET;
    servaddr.sin_port = htons(3015);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");




    //BIND SOCKET


    //LA FUNZIONE BIND ASSOCIA ALLA SOCKET L'INDIRIZZO IN MODO DA POTER ESSERE CONTATTATO DAL CLIENT
    if((bind(mysocket, (const struct sockaddr *) &servaddr, sizeof(servaddr))) < 0)
    {
        printf("funzione bind fallita\n\n");

    }



    while(1){

        char *msginv;

        char msgric[500];
        int sizemsgricevuto =0;


        //CON LA FUNZIONE RECVFROM E' USATA PER PERCHE' STIAMO IMPLEMENTANDO UNA SOCKET UDP QUINDI NON ORIENTATA ALLA CONNESSIONE
        //PRENDE IN INPUT LA PROPRIA SOCKET IL BUFFER DOVE SALVARE I DATI IN INGRESSO, LA DIMENSIONE DEL BUFFER LA STRUTTURA DOVE SALVARE I DATI DEL HOST CHE HA INVIATO I DATI E LA SUA DIMENSIONE PASSATA PER INDIRIZZO

        clientAddrLen = sizeof(clientaddr);

        sizemsgricevuto = recvfrom(mysocket, msgric, 499, 0, (struct sockaddr *) &clientaddr,
                                   &clientAddrLen);

        struct in_addr indirizzo;
        indirizzo.s_addr = clientaddr.sin_addr.s_addr;

        struct hostent *host;
        host = gethostbyaddr((const char *) &indirizzo, 4, AF_INET);
        char *namehost = host->h_name;

        if(sizemsgricevuto>0) { //controllo se ricezione di messaggio


            printf("MESAGGIO RICEVUTO DAL CLIENT: %s ,CON NOME: %s\n\n",msgric, namehost);

            msginv = "OK";

            if (sendto(mysocket, msginv, 499, 0, (const struct sockaddr *) &clientaddr, sizeof(clientaddr)) !=
                499) {
                printf("invio messaggio 2 non riuscito\n\n");
            }
            sizemsgricevuto = 0;


            clientAddrLen = sizeof(clientaddr);

            //QUESTA FUNZIOINE RICEVE DAL CLIENT UNA STRINGA
            sizemsgricevuto = recvfrom(mysocket, (char *) &stringa, sizeof(stringa), 0,
                                       (struct sockaddr *) &clientaddr, &clientAddrLen);
            if (sizemsgricevuto > 0) {


                printf("LA Stringa inviata è : = %s\n\n", stringa);
                if (stringa[0] == 0) {
                    printf("\nla stringa inviata dal client è vuota\n");
                    if (sendto(mysocket, "ATTENZIONE: la stringa non contiene nessuna vocale!", 499, 0, (const struct sockaddr *) &clientaddr, sizeof(clientaddr)) !=
                        499) {
                        printf("invio messaggio 2 non riuscito\n\n");
                    }
                }
                else {
                    int i = 0;
                    while (stringa[i]) {
                        stringa[i] = toupper(stringa[i]);
                        i++;
                    }

                    printf("\ninvio stringa con uppercase %s\n", stringa);


                    //INVIO DELLA STRUTTURA CON IL RISULTATO
                    if (sendto(mysocket, stringa, 499, 0, (const struct sockaddr *) &clientaddr, sizeof(clientaddr)) !=
                        499) {
                        printf("invio messaggio 2 non riuscito\n\n");
                    }

                }

                /*if (sendto(mysocket, (const char *) &msgStruct, sizeof(struct Struct), 0,
                           (const struct sockaddr *) &clientaddr, sizeof(clientaddr)) !=
                    sizeof(struct Struct)) {
                    printf("invio messaggio 3 non riuscito\n\n");
                }*/
            }
        }






    }






#if defined WIN32


    system("pause");
#endif




    return 0;

};