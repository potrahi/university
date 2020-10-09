#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <byteswap.h>
#include <time.h>

#define PORT 8010
#define MAXLINE 255

// Driver code
int main()
{

    while (1)
    {
        int sockfd; 
        char buffer[MAXLINE];
        struct sockaddr_in servaddr, cliaddr;
        // Creating socket file descriptor
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        {
            perror("socket creation failed");       
            exit(EXIT_FAILURE); 
        }

        memset(&servaddr, 0, sizeof(servaddr));         
        memset(&cliaddr, 0, sizeof(cliaddr));           

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;      
        servaddr.sin_port = htons(PORT);         

        // Bind the socket with the server address
        if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
        {
            perror("bind failed");  
            exit(EXIT_FAILURE); 
        }
        int len = 0, n = 0, j = 9, k = 0;       
        len = sizeof(cliaddr);  
        uint32_t time[4] = {0}; 
        unsigned char ip[4] = {0}, buf[80] = {0}, message[MAXLINE] = {0}; 
        unsigned char mes_len = 0; 
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);  
        buffer[n] = '\0';   
        while (buffer[j] != '\0')  
        {
            message[k] = buffer[j]; 
            j++;    
            k++;    
        }
        mes_len = buffer[8];    
        printf("\n<message>: %s (%d)", message, mes_len);    
        sleep(1);   
        printf("\n<IP>:");  
        for (int i = 0; i < 4; i++)     
        {
            ip[i] = buffer[i];      
            printf("%d", ip[i]);    
            if (i == 3)         
                break;  
            else
                printf(".");    
            time[i] = htonl(buffer[i + 4]);  
        }
        printf("\n");       
        time_t sec = 0;      
        sec = (time[3] >> 24) | (time[2] >> 16) | (time[1] >> 8) | (time[0]); 
        struct tm ptm = *localtime(&sec);           
        strftime(buf, sizeof(buf), "%c", &ptm);      
        printf("<datetime>: Dnešný dátum: %s\n", buf);  
        close(sockfd);  
    }
    return 0;
}
