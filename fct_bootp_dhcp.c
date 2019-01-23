#include "bootp.h"

#define IS_BRODCAST 0x8000
#define MAGIC_COOKIE 0x63825363
#define DHCP_OPTIONSIZE 64


void fct_bootp_dhcp(const u_char* packet, int size, int v){

    int i, n, test;
    u_char o;

    struct bootp* bootphead;
    bootphead=(struct bootp*)(packet+size);

    v_printf(" -> ", v==1);
    printf("BOOTP/DHCP");
    v_printf(":\n", v==3);

    if(v>=2){
        printf("    Type de message : ");
        switch(bootphead->bp_op){
            case BOOTREQUEST:
                printf("Requete");
                break;
            case BOOTREPLY:
                printf("Reponse");
                break;
        }
        printf(" (%i)\n", bootphead->bp_op);
    }

    if(v==3){

        printf("    Support : ");
        switch(bootphead->bp_htype){
            case HTYPE_ETHERNET:
                printf("Ethernet");
                break;
            case HTYPE_EXP_ETHERNET:
                printf("Ethernet Experimental");
                break;
            case HTYPE_AX25:
                printf("Amateur Radio AX.25");
                break;
            case HTYPE_PRONET:
                printf("Proteon ProNET Token Ring");
                break;
            case HTYPE_CHAOS:
                printf("Chaos");
                break;
            case HTYPE_IEEE802:
                printf("IEEE 802");
                break;
            case HTYPE_ARCNET:
                printf("ARCNET");
                break;;
            default:;
                printf("Autre");
                break;
        }
        printf(" (0x%.2x)\n", bootphead->bp_htype);
        printf("    Longueur adresse support : %i\n", bootphead->bp_hlen);
        printf("    Sauts : %i\n", bootphead->bp_hops);
        printf("    ID de transaction : 0x%.8x\n", ntohl(bootphead->bp_xid));
        printf("    Temps ecoule : %i seconde(s)\n", ntohs(bootphead->bp_secs));
        printf("    BOOTP Flags : ");
        if(ntohs(bootphead->bp_flags) & IS_BRODCAST == 1){
            printf("Broadcast");
        }
        else{
            printf("Unicast");
        }
        printf(" (0x%.4x)\n", ntohs(bootphead->bp_flags));
        printf("    IP Client : %s\n", inet_ntoa(bootphead->bp_ciaddr));
        printf("    Votre IP : %s\n", inet_ntoa(bootphead->bp_yiaddr));
        printf("    IP Serveur : %s\n", inet_ntoa(bootphead->bp_siaddr));
        printf("    IP Passerelle : %s\n", inet_ntoa(bootphead->bp_giaddr));
        printf("    @MAC Support Client : ");
        for(i=0;i<6;i++){
            printf("%.2x:",bootphead->bp_chaddr[i]);
        }
        printf("\n");
        printf("    Nom du Serveur Hote : ");
        if(bootphead->bp_sname[0] == NULLCHAR){
            printf("Pas de nom attribue\n");
        }
        else{
            printf("%s\n", bootphead->bp_sname);
        }
        printf("    Nom du fichier boot : ");
        if(bootphead->bp_file[0] == NULLCHAR){
            printf("Pas de nom attribue\n");
        }
        else{
            printf("%s\n", bootphead->bp_sname);
        }


        //test magic cookie
        if(lecture_option(bootphead->bp_vend, 0, 4)!=MAGIC_COOKIE){
            printf("    ERREUR LECTURE BOOTP\n");
            return;
        }

        //les optinons DHCP
        i=4;
        while(i<DHCP_OPTIONSIZE){
            o=bootphead->bp_vend[i];
            n=bootphead->bp_vend[i+1];
            switch(o){
                case TAG_SUBNET_MASK:
                    printf("    Option : (%i) Masque de sous reseau %i.%i.%i.%i\n", o
                        , bootphead->bp_vend[i+2], bootphead->bp_vend[i+3]
                        , bootphead->bp_vend[i+4], bootphead->bp_vend[i+5]);
                    break;
                case TAG_IP_LEASE:
                    test=lecture_option(bootphead->bp_vend, i+2, 4);
                    printf("    Option : (%i) Duree du bail (IP) %i secondes\n", o, test);
                    break;
                case TAG_DHCP_MESSAGE:{
                    printf("    Option : (%i) Message DHCP type ", o);
                    switch(bootphead->bp_vend[i+2]){
                        case DHCPDISCOVER:
                            printf("DISCOVER\n");
                            break;
                        case DHCPOFFER:
                            printf("OFFER\n");
                            break;
                        case DHCPREQUEST:
                            printf("REQUEST\n");
                            break;
                        case DHCPACK:
                            printf("ACK\n");
                            break;
                        case DHCPRELEASE:
                            printf("REALEASE\n");
                            break;
                        default:
                            printf("AUTRE\n");
                            break;
                    }
                    break;
                }
                case TAG_END:
                    printf("\n");
                    return;
                default:
                    printf("    Option : (%i) unknown\n", o);
            }
            i=i+n+2;
        }

    }

}