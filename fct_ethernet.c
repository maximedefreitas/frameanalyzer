#include <netinet/if_ether.h>


int fct_ethernet(const u_char* packet, int* size, int v){

    int i;

    struct ether_header* ehead;
    ehead=(struct ether_header*)packet;

    *size+=sizeof(struct ether_header);


    printf("ETHERNET");
    v_printf(":\n", v==3);

    v_printf("    @MAC Destination : ", v==3);
    v_printf("  @MAC Des: ", v==2);
    for(i=0;i<6;i++){
        if(v>=2){
            printf("%.2x:",ehead->ether_dhost[i]);
        }
    }
    v_printf("\n", v==3);  
                                                                                                                     
    v_printf("    @MAC Source : ", v==3);
    v_printf("  @MAC Src: ", v==2);
    for(i=0;i<6;i++){
        if(v>=2){
            printf("%.2x:",ehead->ether_shost[i]);
        }
    }
    v_printf("\n", v>=2);
    return ntohs(ehead->ether_type);

}
