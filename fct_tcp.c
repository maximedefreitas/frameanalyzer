#include <netinet/tcp.h>

#define TCP_BASE_LEN 4*5

//flags TCP
#define	FIN	0x01
#define	SYN	0x02
#define	RST	0x04
#define	PSH 0x08
#define	ACK	0x10
#define	URG	0x20

//option TCO
#define END_OPT 0
#define NO_OP 1
#define MAX_SEG_SIZE 2
#define WIN_SCALE 3
#define SACK_PERM 4
#define TIMESTAMPS 8

void fct_tcp(const u_char* packet, u_short* port_src, u_short* port_dest, int* size, int* app_size, int v){

    int opt_seek, test;

    struct tcphdr* tcphead;
    tcphead=(struct tcphdr*)(packet+*size);

    v_printf(" -> ", v==1);
    printf("TCP");
    v_printf(":\n", v==3);

    *port_src=ntohs(tcphead->th_sport);
    if(v>=2){
        printf("    Port Source : %i", *port_src);
    }
    v_printf("\n", v==3);
    *port_dest=ntohs(tcphead->th_dport);
    if(v>=2){
        printf("    Port Destination : %i", *port_dest);
    }
    v_printf("\n", v>=2);

    if(v==3){
        printf("    Numeros de sequence : 0x%.4x\n", ntohl(tcphead->th_seq));
        printf("    Numeros d'aquittement : 0x%.4x\n", ntohl(tcphead->th_ack));
        printf("    Longueur de l'entete : %i octets\n", tcphead->th_off*4);
    }

    *size=*size+tcphead->th_off*4;
    *app_size=*app_size-tcphead->th_off*4;
    
    if(v==3){
        printf("    Type : ");
        if(tcphead->th_flags & FIN){
            printf("FIN ");
        }
        if(tcphead->th_flags & SYN){
            printf("SYN ");
        }
        if(tcphead->th_flags & RST){
            printf("RST ");
        }
        if(tcphead->th_flags & PSH){
            printf("PSH ");
        }
        if(tcphead->th_flags & ACK){
            printf("ACK ");
        }
        if(tcphead->th_flags & URG){
            printf("URG ");
        }
        printf("\n");
        printf("    Fenetre : 0x%.4x\n", ntohs(tcphead->th_win));
        printf("    CheckSum : 0x%.4x\n", ntohs(tcphead->th_sum));
        if(tcphead->th_flags & URG){
            printf("    Pointeur Urg. : 0x%.4x\n", ntohs(tcphead->th_urp));
        }
    }

    opt_seek=TCP_BASE_LEN;
    if((tcphead->th_off*4)!=opt_seek){

        v_printf("    Option(s):\n", v==3);
        while(opt_seek<tcphead->th_off*4){
            test=lecture_option(packet, (*size)-(tcphead->th_off*4)+opt_seek, 1);
            switch(test){
                case END_OPT:
                    v_printf("        (0x00) Fin des Options\n", v==3);
                    return;
                    break;
                case NO_OP:
                    v_printf("        (0x01) Pas d'operation\n", v==3);
                    opt_seek++;
                    break;
                case MAX_SEG_SIZE:
                    v_printf("        (0x02) Taille maximal du segment\n", v==3);
                    opt_seek+=lecture_option(packet, (*size)-(tcphead->th_off*4)+opt_seek+1, 1);
                    break;
                case WIN_SCALE:
                    if(v==3){
                        printf("        (0x03) Taille fenetre: %i\n",
                            lecture_option(packet, (*size)-(tcphead->th_off*4)+opt_seek+2, 1));
                    }
                    opt_seek+=lecture_option(packet, (*size)-(tcphead->th_off*4)+opt_seek+1, 1);
                    break;
                case SACK_PERM:
                    v_printf("        (0x04) SACK Permis\n", v==3);
                    opt_seek+=lecture_option(packet, (*size)-(tcphead->th_off*4)+opt_seek+1, 1);
                    break;
                case TIMESTAMPS:
                    v_printf("        (0x08) Timestamps\n", v==3);
                    opt_seek+=lecture_option(packet, (*size)-(tcphead->th_off*4)+opt_seek+1, 1);
                    break;
                default:
                    if(v==3){
                        printf("        (0x%.2x) Non impemente\n", test);
                    }
                    opt_seek+=lecture_option(packet, (*size)-(tcphead->th_off*4)+opt_seek+1, 1);

            }

        }

    }

}