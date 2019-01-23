//type de materiel
#define IS_ETHERNET 1
#define IS_IEE802 6

//operation
#define REQUEST 1
#define REPLY 2

int fct_arp(const u_char* packet, int* size, int v){

    int test, i;

    const u_char* arphead=packet+(*size);

    (*size)+=28; //taiile de l'entete arp en octet

    v_printf(" -> ", v==1);
    printf("ARP");
    v_printf(":\n", v==3);

    if(v>=2){

        test=lecture_option(arphead, 0, 2);
        printf("    Type de materiel: ");
        switch(test){
            case IS_ETHERNET:
                printf("Ethernet");
                break;
            case IS_IEE802:
                printf("IEEE 802");
                break;
            default:
                printf("autre");
                break;
        }
        v_printf("\n", v==3);
        test=lecture_option(arphead, 2, 2);
        printf("    Type de protocole: ");
        switch(test){
            case IS_IP:
                printf("IPv4");
                break;
            default:
                printf("autre");
                break;
        }
        printf("\n");

    }

    if(v==3){

        test=lecture_option(arphead, 4, 1);
        printf("    Longueur de l'adresse physique: %i\n", test);
        test=lecture_option(arphead, 5, 1);
        printf("    Longueur de l'adresse logique: %i\n", test);
        test=lecture_option(arphead, 6, 2);
        printf("    Operation: ");
        switch(test){
            case REQUEST:
                printf("Request");
                break;
            case REPLY:
                printf("Reply");
                break;
            default:
                printf("autre");
                break;
        }
        printf("\n");
        printf("    Adresse physique emetteur: ");
        for(i=0; i<6; i++){
            test=lecture_option(arphead, 8+i, 1);
            printf("%.2x:", test);
        }
        printf("\n");
        printf("    Adresse IP emetteur: ");
        for(i=0; i<4; i++){
            test=lecture_option(arphead, 14+i, 1);
            printf("%i.", test);
        }
        printf("\n");
        printf("    Adresse physique cible: ");
        for(i=0; i<6; i++){
            test=lecture_option(arphead, 18+i, 1);
            printf("%.2x:", test);
        }
        printf("\n");
        printf("    Adresse IP cible: ");
        for(i=0; i<4; i++){
            test=lecture_option(arphead, 24+i, 1);
            printf("%i.", test);
        }
        printf("\n");

    }

    return 0;

}
