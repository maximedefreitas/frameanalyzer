#include <netinet/udp.h>


void fct_udp(const u_char* packet, u_short* port_src, u_short* port_dest, int* size, int v){

    struct udphdr* udphead;
    udphead=(struct udphdr*)(packet+*size);

    *size+=sizeof(struct udphdr);

    v_printf(" -> ", v==1);
    printf("UDP");
    v_printf(":\n", v==3);

    *port_src=ntohs(udphead->uh_sport);
    if(v>=2){
        printf("    Port Source : %i", *port_src);
    }
    v_printf("\n", v==3);
    *port_dest=ntohs(udphead->uh_dport);
    if(v>=2){
    printf("    Port Destination : %i", *port_dest);
    }
    v_printf("\n", v>=2);

    if(v==3){
        printf("    Longeur du message (entête et donnees) : %i octets\n", ntohs(udphead->uh_ulen));
        printf("    CheckSum : 0x%.4x\n", ntohs(udphead->uh_sum));
    }

}
