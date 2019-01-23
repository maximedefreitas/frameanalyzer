//test flag IP
#define NFRAG 0x4000
#define MFRAG 0x2000


int fct_ip(const u_char* packet, int* size, int* app_size, int v){

    int test;

    struct ip* iphead;
    iphead=(struct ip*)(packet+*size);

    v_printf(" -> ", v==1);
    printf("IPv%u", iphead->ip_v);
    v_printf(":\n", v==3);

    *app_size=iphead->ip_hl*4;
    *size+=*app_size;

    if(v==3){
        printf("    Longueur de l'entête : %i octets", *app_size);
        printf("    Type of Service : 0x%.2x\n", iphead->ip_tos);
        printf("    Longueur totale : %i octets\n", ntohs(iphead->ip_len));
        printf("    Flags : 0x%.4x\n", ntohs(iphead->ip_id));
        printf("    Indicateur : 0x%.4x", ntohs(iphead->ip_off));
    }

    *app_size=ntohs(iphead->ip_len)-*app_size;

    if(v==3){
        switch(ntohs(iphead->ip_off)){
            case NFRAG:
                printf(" (Dont't Fragment)\n");
                break;
            case MFRAG:
                printf(" (More Fragment)\n");
                break;
            default:
                printf("\n");
                break;
        }
        printf("    Duree de vie : %i\n", iphead->ip_ttl);
    }

    switch(iphead->ip_p){
        case IS_TCP:
            v_printf("    Protocole : TCP\n", v==3);
            test=IS_TCP;
            break;
        case IS_UDP:
            v_printf("    Protocole : UDP\n", v==3);
            test=IS_UDP;
            break;
        case IS_SCTP:
            v_printf("    Protocole : SCTP\n", v==3);
            test=IS_SCTP;
            break;
        default:
            v_printf("    Protocole : inconnu\n", v==3);
            break;
    }

    if(v==3){
        printf("    CheckSum : 0x%.4x\n", ntohs(iphead->ip_sum));
    }
    if(v>=2){
        printf("    IP source : %s", inet_ntoa(iphead->ip_src));
    }
    if(v==3){
        printf("\n");
    }
    if(v>=2){
        printf("    IP destination : %s\n", inet_ntoa(iphead->ip_dst));
    }

    return test;

}