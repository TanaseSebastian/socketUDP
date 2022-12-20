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


#include <stdio.h>
#include <stdlib.h>




//struct Struct msgStruct;
int firstTime = 1;
char stringa[50] = "";
char *indirizzo = "";
int porta = 0;
char* aquisizioneStringa()
{
    puts("\n");

    printf("INSERIRE UNA STRINGA:");
    memset(stringa, 0, sizeof(stringa));
    scanf("%s",stringa);
    return stringa;
}




int numeroporta()
{ int num =0;
    printf("inserisci numero porta :");
    scanf("%d",&num);


    return num;
}


void svolgiEsercizio() {

    if(firstTime){
    //TROVARE INDIRIZZO IP DEL SERVER
    struct hostent *host = NULL;
    char nomehost[30];

    printf("INSERIRE NOME HOST DA CONTATTARE:\n");
    scanf("%s", nomehost);
    host = gethostbyname(nomehost);

    struct in_addr *indirizzos = (struct in_addr *) host->h_addr_list[0];//ASSOCIAZIONE AL PUNTATORE ALLA STRUTTURA IN_ADDR L'ELEMENTO 0 DELLA LISTA "H_ADDR" NEL QUALE SI TROVA L'INDIRIZZO IP
    char *ip = inet_ntoa(*indirizzos);//LA FUNZIONE INET_NTOA CONVERE IP DALLA RAPPRESENTAZIONE RETE A QUELLA DELL'HOST
    indirizzo = ip;
    if (firstTime)printf("INDIRIZZO IP DELL'HOST INSERITO TROVATO CON DNS : %s\n", ip);

    int num = 0;
    num = numeroporta();
    porta = num;
}
    int mysocket;
    struct sockaddr_in servaddr; //STRUTTURA PER DATI SOCKET SERVER SERVE PER FARE INVIARE DATI
    struct sockaddr_in fromaddr; //STRUTTURA PER SALVARE LA SOCKET DELL'HOST CHE CI INVIA DATI
    int fromsize;
    //creazione socket
    if ((mysocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        printf("errore creazione socket\n\n");
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(porta);
    servaddr.sin_addr.s_addr = inet_addr((const char *) indirizzo);

    if (strcmp(indirizzo, "127.0.0.1") == 0 && porta == 3015) { //CONTROLLO CHE NUMERO PORTA INSERITO E NOME HOST SONO GIUSTI

        char *msg = "hello";
        char msgric[500];
        int sizemsric = 0;
        int esercizio = 1;

        //CON LA FUNZIONE SENDTO PRENDE IN INPUT LA NOSTRA SOCKET IL PUNTATORE ALLA STRUTTURA O STRINGA DA INVIARE , IL PUNTATORE ALL'INDIRIZZO DELL ' HOST
        //AL QUALE VOGLIAMO INVIARE I DATI E LA LUNGHEZZA DEI DATI DELLA SUA STRUTTURA.

        //QUESTA FUNZIONE VIENE INSERITA IN UN CONTROLLO IF PERCHE' RITORNA IL NUMERO DEI BYTE INVIATI I QUALI VENGONO CONFRONTATI CON LA LUNGHEZZA DEL BUFFER MASSIMO DEL MESSAGGIO


        if (sendto(mysocket, msg, 499, 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)) != 499) {
            printf("invio non riuscito\n\n");
        }else printf("invio messaggio al client di benvenuto riuscito");
        fromsize = sizeof(fromaddr); // A FROMSIZE VIENE ASSOCIATO LA LUNGHEZZA DEI DATI DELLA STRUTTURA "FROMADDDR"

        /*
         * LA FUNZIONE RECVFROM PRENDE IN INPUT LA PROPRIA SOCKET IL PUNTATORE ALLA STRUTTURA O STRINGA DOVE DEVE SALVARE I DATI LA GRANDEZZA MASSIMA DELLA STESSA E
         * INFINE PRENDE IL PUNTATORE ALLA STRUTTURA NELLA QUALE DEVE SALVARE I DATI DELLA SOCKET DELL'HOST CHE HA INVIATO I DATI IN MODO DA INVERTIRLI NEL CASO DI RISPOSTA
         */




        sizemsric = recvfrom(mysocket, msgric, 499, 0, (struct sockaddr *) &fromaddr, &fromsize);

        if (sizemsric > 0) {

            printf("\n\nRISPOSTA DA PARTE DEL CLIENT: %s\n", msgric);
            sizemsric = 0;
        }
            aquisizioneStringa();
            char stringa2[sizeof(stringa)];
            memset(stringa2, 0, sizeof(stringa2));
            int indice = 0;
            char vocali[5] = "aeiou";
            for (int i = 0; i < sizeof(stringa); i++) {
                for (int j = 0; j < sizeof(vocali); j++) {
                    if (stringa[i] == vocali[j]) {
                        //creo una stringa contenente tutte le vocali della parola
                        stringa2[indice] = stringa[i];
                        indice++;
                    }
                }
        }
        if (sendto(mysocket, stringa2, 499, 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)) !=
            499) {
            printf("invio messaggio non riuscito\n\n");
        }


        memset(msgric, 0, sizeof(msgric));
        sizemsric = recvfrom(mysocket, msgric, 499, 0, (struct sockaddr *) &fromaddr, &fromsize);

        printf("\nRisposta del server: ");
        if (sizemsric > 0) {

            int i = 0;
            while (msgric[i] != 0) {
                printf("%c ", msgric[i]);
                i++;
            }
            sizemsric = 0;
        }
    } else {
        printf("\n\nIL NOME DELL'HOST O IL NUMERO DI PORTA INSERITI  E' SBAGLIATO...RIPROVA\n\n");
        svolgiEsercizio();
    }
    closesocket(mysocket);
}

int main() {
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
    char lettera;
    while(1){
        svolgiEsercizio();
        firstTime = 0;
        printf("\n\nESERCIZIO FINITO CORRETTAMENTE\n\nINSERIRE 'S' PER CONTINUARE 'N' PER CONCLUDERE IL PROGRAMMA....\n\n");
        scanf(" %c", &lettera);
        //printf("\nlettera contenuto: %c", lettera);
        if (lettera == 's'){
            printf("\n bene ricontattiamo il server...\n");
        }else{
            break;
        }
    }
    return 0;

}