#include "ipv6.h"

#define IPV6_BASE_LEN 4*10

//les option ipv6
#define SAUT_A_SAUT 0
#define DEST 60
#define ROUTAGE 43
#define FRAG 44
#define AH 51
#define ESP 50


//lit les options IPv6 par recurence
int next_ipv6(const u_char* packet, int next, int* size, int v){

    int entete_suiv=lecture_option(packet, (*size), 1);
    int l=lecture_option(packet, (*size)+1, 1);
    switch(next){
        case SAUT_A_SAUT:
            (*size)+=l;
            if(v==3){
                printf("        (0x%.x2) Sauts apres sauts", next);
            }
            return next_ipv6(packet, entete_suiv, size, v);
            break;
        case DEST:
            (*size)+=l;
            if(v==3){
                printf("        (0x%.x2) Destination", next);
            }
            return next_ipv6(packet, entete_suiv, size, v);
            break;
        case ROUTAGE:
            (*size)+=l;
            if(v==3){
                printf("        (0x%.x2) Routage", next);
            }
            return next_ipv6(packet, entete_suiv, size, v);
            break;
        case FRAG:
            (*size)+=l;
            if(v==3){
                printf("        (0x%.x2) Fragmentation", next);
            }
            return next_ipv6(packet, entete_suiv, size, v);
            break;
        case AH:
            (*size)+=l;
            if(v==3){
                printf("        (0x%.x2) AH", next);
            }
            return next_ipv6(packet, entete_suiv, size, v);
            break;
        case ESP:
            (*size)+=l;
            if(v==3){
                printf("        (0x%.x2) ESP", next);
            }
            return next_ipv6(packet, entete_suiv, size, v);
            break;
        default:
            return next;
    }

}

//fonction principale
int fct_ipv6(const u_char* packet, int* size, int* app_size, int v){

    int i;
    char* str;
    unsigned char next;

    struct ipv6hdr* ipv6head;
    ipv6head=(struct ipv6hdr*)(packet+*size);

    *size+=IPV6_BASE_LEN;

    v_printf(" -> ", v==1);
    printf("IPv%u", ipv6head->ipv6_version);
    v_printf(":\n", v==3);

    if(v==3){
        printf("    Classe : (0x%.2x)\n", ipv6head->ipv6_priority);
        printf("    Label : (0x");
        for(i=0; i<3; i++){
            printf("%.2x", ipv6head->ipv6_flow_lbl[i]);
        }
        printf(")\n");
    }

    (*app_size)=ntohs(ipv6head->ipv6_len);

    if(v==3){
        printf("    Longueur apres entete : %i octets\n", *app_size);
    }

    next=ipv6head->ipv6_nextheader;

    if(v==3){
        printf("    Suivant : ");    
        switch(next){
            case IS_TCP:
                printf(" TCP (0x%.2x)", next);
                break;
            case IS_UDP:
                printf(" UDP (0x%.2x)", next);
                break;
            default:
                printf(" Options ou inconnu (0x%.2x)", next);
                break; 
        }
        printf("\n");
        printf("    Saut maximum : %i\n", ipv6head->ipv6_hoplimit);
    }

    inet_ntop(AF_INET6, &(ipv6head->ipv6_src), str, INET6_ADDRSTRLEN);

    if(v>=2){
        printf("    IPv6 Source : %s", str);
    }
    v_printf("\n", v==3);
    inet_ntop(AF_INET6, &(ipv6head->ipv6_dst), str, INET6_ADDRSTRLEN);
    if(v>=2){
        printf("    IPv6 Destination : %s\n", str);
    }

    return next_ipv6(packet, next, size, v);
}
