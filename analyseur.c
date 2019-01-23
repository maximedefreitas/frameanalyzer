#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pcap.h>


//test encapsulation par protocole ethernet
#define	IS_IP 0x0800
#define IS_IPv6 0x86dd
#define IS_ARP 0x0806
//test encapsulation protocol par IP
#define IS_TCP 0x06
#define IS_UDP 0x11
#define IS_SCTP 0x84

//test port
#define DHCP1 67
#define DHCP2 68
#define DNS 53
#define HTTP 80
#define TELNET 23
#define FTP1 20
#define FTP2 21
#define SMTP 25
#define POP 110
#define IMAP 143

#include "fct_traitement.h"

#define MAXBYTES 1518
#define TIMEOUT 1000


int comp=1;
struct in_addr ip_addr;
struct in_addr ip_mask;

int verbosite=3;

//fonction de callback
void got_packet(u_char* args, const struct pcap_pkthdr* header, const u_char* packet){
   
    int test;
    u_short port_src, port_dest;
    int size=0;
    int app_size; //taille des donnees applicatives

    //affichage
    printf("------------------------------------------------------------------------\n");
    printf("                            Paquet n°%i\n", comp);
    printf("------------------------------------------------------------------------\n\n");

    comp++;

    //ETHERNET
    test=fct_ethernet(packet, &size, verbosite);

    switch(test){
        //IP
        case IS_IP:
            test=fct_ip(packet, &size, &app_size, verbosite);
            break;

        //IPv6
        case IS_IPv6:
            test=fct_ipv6(packet, &size, &app_size, verbosite);
            break;
        //ARP
        case IS_ARP:
            test=fct_arp(packet, &size, verbosite);
            break;
    }

    switch(test){
        //TCP
        case IS_TCP:
            fct_tcp(packet, &port_src, &port_dest, &size, &app_size, verbosite);
            break;
        //UDP
        case IS_UDP:
            fct_udp(packet, &port_src, &port_dest, &size, verbosite);
            app_size-=8;
            break;
        //SCTP
        case IS_SCTP:
            fct_sctp(packet, &port_src, &port_dest, &size, &app_size, verbosite);
            break;
    }

        if(app_size>0){ //si il y a des donnees de la couche applicative

            //BOOTP/DHCP (port 67 et 68)
            if((port_src==DHCP1 && port_dest==DHCP2) || (port_src==DHCP2 && port_dest==DHCP1)){
                fct_bootp_dhcp(packet, size, verbosite);
            }

            //DNS (port 53)
            if((port_src==DNS) || (port_dest==DNS)){
                fct_dns(packet, size, verbosite);
            }

            //HTTP (port 80)
            if((port_src==HTTP) || (port_dest==HTTP)){
                fct_http(packet, size, app_size, verbosite);
            }

            //Telnet (port 23)
            if((port_src==TELNET) || (port_dest==TELNET)){
                fct_telnet(packet, size, app_size, verbosite);
            }

            //FTP (port 20 et 21)
            if((port_src==FTP1) || (port_dest==FTP1)
            || (port_src==FTP2) || (port_dest==FTP2)){
                fct_ftp(packet, size, app_size, verbosite);
            }

            //SMTP (port 25)
            if((port_src==SMTP) || (port_dest==SMTP)){
                fct_smtp(packet, size, app_size, verbosite);
            }

            //POP (port 110)
            if((port_src==POP) || (port_dest==POP)){
                fct_pop(packet, size, app_size, verbosite);
            }

            //IMAP (port 143)
            if((port_src==IMAP) || (port_dest==IMAP)){
                fct_imap(packet, size, app_size, verbosite);
            }

        }


    printf("\n\n");

}

int main(int argc, char *argv[])
{
    char* interface;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* flux;	

    bpf_u_int32 netaddr;		
    bpf_u_int32 netmask;

    int opt;
    int i=0;
    char* fichier;


    while ((opt = getopt(argc, argv, "i:o:f:v:")) != -1) {
        switch (opt) {
        case 'i':
            i=1;
            interface=optarg;
            break;
        case 'o':
            fichier=optarg;
            break;
        case 'v':
            verbosite=atoi(optarg);
            if(verbosite>3 || verbosite<1){
                fprintf(stderr, "verbosite entre 1 et 3\n");
                exit(EXIT_FAILURE);
            }
            break;
        default: 
            fprintf(stderr, "\nutilisation:\n    -i <interface>\n    -o <fichier>\n    -v <1...3>\n");
            exit(EXIT_FAILURE);
        }
    }

    

    //ouverture sur une interface
    if(i==1){

        //recherche de l'adresse IP et masque sous-reseau
        if(pcap_lookupnet(interface, &netaddr, &netmask, errbuf)==-1){
            fprintf(stderr, "Impossible de trouver adresse et masque associes a %s: %s\n", interface, errbuf);
            return(1);
        }
        ip_addr.s_addr=netaddr;
        ip_mask.s_addr=netmask;

        //Ouverture de la capture 
        flux=pcap_open_live(interface, MAXBYTES, 1, TIMEOUT, errbuf);
        if(flux==NULL){
            fprintf(stderr, "Ouverture impossible %s: %s\n", interface, errbuf);
            return(1);
        }

    }
    //ouverture avec un fichier
    else{
        flux=pcap_open_offline(fichier, errbuf);
        if(flux==NULL){
            fprintf(stderr, "Ouverture fichier impossible: %s\n", errbuf);
            return(1);
        }
    }

    //capture d'un packet
    if(pcap_loop(flux, -1, got_packet, NULL)<0){
        fprintf(stderr, "Erreur : aucun packet capture");
    }

    printf("------------------------------------------------------------------------\n");

    //close the session
    pcap_close(flux);
    return(0);
}