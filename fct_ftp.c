void fct_ftp(const u_char* packet, int size, int app_size, int v){
    
    v_printf(" -> ", v==1);
    printf("FTP");
    v_printf(":\n    ", v==3);

    if(v==3){
        afficher_ascii(packet, &size, app_size);
    }

}