#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define DESIRED_ADDRESS "pcfeib425t.vsb.cz"
#define DESIRED_PORT 8000 /* the port client will be connecting to */
#define BUFSIZE 255      /* max number of bytes we can get at once */

int main(int argc, char *argv[])
{
    struct sockaddr_in addr = {0};  // vytvoreni struktury socketu 
    struct hostent *he;         // vytvoreni hostu

    if ((he = gethostbyname(argv[1])) == NULL)  // nacteni IP adresy z argumentu
    { /* get the host info */
        herror("gethostbyname");    // vypise se pokud nebude zadana IP adresa
        exit(1);
    }

    addr.sin_family = AF_INET;          // zadani typu soketu
    addr.sin_port = htons(DESIRED_PORT); /*converts short to short with network byte order*/
    addr.sin_addr = *((struct in_addr *)he->h_addr);    // ulozeni IP adresy do soketu
    bzero(&(addr.sin_zero), 8); /* zero the rest of the struct */

    do
    {
        unsigned char buf[BUFSIZE] = {0};   // buffer na message
        int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);   // vytvorim soket
        if (sock == -1) // a pokud vrati -1 
        {
            perror("Socket creation error");    je chyba
            return EXIT_FAILURE;
        }
        if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) // pripojim se do serveru apokud se vari -1
        {
            perror("Connection error"); // je to chyba
            close(sock);            // a uzavre se soket
            return EXIT_FAILURE;
        }

        printf("<message>: ");      // info message
        fgets(buf, BUFSIZE, stdin);     // prevezme messge z stdin a ulozi do budderu
        if (send(sock, buf, BUFSIZE, 0) /*write may be also used*/ == -1)   // posle to na soket a pokud vrati 0
        {
            perror("Send error");   // je to chyba
            close(sock);    // a soket se uzavre
            return EXIT_FAILURE;
        }
        bzero(buf, BUFSIZE);    // vynuluje se zbytek bufferu
        close(sock);    // a soket se uzavre
    } while (1);    // proces se zopakuje
    return EXIT_SUCCESS;
}
