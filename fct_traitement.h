#include <arpa/inet.h>
#include <netinet/ip.h>

#define NULLCHAR 0x00
#define LIM_ASCII 0x1F

//fonction pour la verbosite
void v_printf(char* msg, int v){
    if(v){
        printf("%s", msg);
    }
}

//fonction pour lire les options dhcp
int lecture_option(const u_int8_t* options, int pos, int nb){
    int res=options[pos], i;
    for(i=pos+1; i<pos+nb; i++){
        res*=256;
        res+=options[i];
    }
    return res;
}

//fonction pour afficher des caracteres de la table ascii
void afficher_ascii(const u_int8_t* option, int* pos, int app_size){
    int i=1;
    char o=option[*pos];
    while(o != NULLCHAR && i<=app_size){
        o=option[*pos];
        if(o==0x0d && option[(*pos)+1]==0x0d){
            printf("\n    ");
            *pos+=1;
        }
        else if(o==0x0a){
            printf("\n    ");
        }
        else if(o<=LIM_ASCII && o!=0x0d && o!=0x0a && o!=NULLCHAR){
            printf(".");
        }
        else{
            printf("%c", o);
        }
        *pos+=1;
        i++;
    }
    *pos+=1;
}


#include "fct_imap.c"
#include "fct_pop.c"
#include "fct_smtp.c"
#include "fct_ftp.c"
#include "fct_telnet.c"
#include "fct_http.c"
#include "fct_dns.c"
#include "fct_bootp_dhcp.c"
#include "fct_tcp.c"
#include "fct_udp.c"
#include "fct_sctp.c"
#include "fct_arp.c"
#include "fct_ipv6.c"
#include "fct_ip.c"
#include "fct_ethernet.c"


//traitement ethernet
int fct_ethernet(const u_char* packet, int* size, int v);

//traitement IP
int fct_ip(const u_char* packet, int* size, int* app_size, int v);

//traitement IPv6
int fct_ipv6(const u_char* packet, int* size, int* app_size, int v);

//traitement ARP
int fct_arp(const u_char* packet, int* size, int v);

//traitement TCP
void fct_tcp(const u_char* packet, u_short* port_src, u_short* port_dest, int* size, int* app_size, int v);

//traitement UDP
void fct_udp(const u_char* packet, u_short* port_src, u_short* port_dest, int* size, int v);

//traitement SCTP
void fct_sctp(const u_char* packet, u_short* port_src, u_short* port_dest, int* size, int* app_size, int v);

//traitement BOOTP/DHCP
void fct_bootp_dhcp(const u_char* packet, int size, int v);

//traitement DNS
void fct_dns(const u_char* packet, int size, int v);

//traitement HTTP
void fct_http(const u_char* packet, int size, int app_size, int v);

//traitement Telnet
void fct_telnet(const u_char* packet, int size, int app_size, int v);

//traitement FTP
void fct_ftp(const u_char* packet, int size, int app_size, int v);

//traitement SMTP
void fct_smtp(const u_char* packet, int size, int app_size, int v);

//traitement POP
void fct_pop(const u_char* packet, int size, int app_size, int v);

//traitement IMAP
void fct_imap(const u_char* packet, int size, int app_size, int v);