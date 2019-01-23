void fct_sctp(const u_char* packet, u_short* port_src, u_short* port_dest, int* size, int* app_size, int v){

    int test;
    const u_char* sctphead=packet+(*size);

    v_printf(" -> ", v==1);
    printf("SCTP");
    v_printf(":\n", v==3);

    (*port_src)=lecture_option(sctphead, 0, 2);
    (*port_dest)=lecture_option(sctphead, 2, 2);

    if(v>=2){
        printf("    Port Source: %i", *port_src);
    }
    v_printf("\n", v==3);
    if(v>=2){
        printf("    Port Destination: %i", *port_dest);
    }
    v_printf("\n", v==3);

    if(v==3){
        printf("    Tag de Verification: (0x%.4x)\n", lecture_option(sctphead, 4, 4));
        printf("    Checksum: (0x%.4x)\n", lecture_option(sctphead, 8, 4));
    }

}