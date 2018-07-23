#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char* GetLocalIp()  
{  

    int sock_get_ip;  
    char* ipaddr =(char*)malloc(50);  

    struct   sockaddr_in *sin;  
    struct   ifreq ifr_ip;     

    printf("Hello world\n");
    if ((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) == -1)  
    {  
        printf("socket create failse...GetLocalIp!/n");  
        return NULL;  
    }  

    memset(&ifr_ip, 0, sizeof(ifr_ip));     
 
    
    //这里改网卡的名字
    strncpy(ifr_ip.ifr_name, "ens33", sizeof(ifr_ip.ifr_name) - 1);     

    printf("Hello world\n");
    if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip ) < 0  )     
    {     
        return NULL;     
    }       
    printf("Hello world\n");
    sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;     
    printf("Hello world\n");
    strcpy(ipaddr,inet_ntoa(sin->sin_addr));         

    printf("local ip:%s /n",ipaddr);      
    close( sock_get_ip  );  

    return  ipaddr;  
}  

int main()
{
    char* s =GetLocalIp();

    printf("%s\n",s);
    return 0;
}

