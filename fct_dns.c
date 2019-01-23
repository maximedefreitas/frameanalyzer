#define REPONSE 0x8000
#define FILTRE_TYP 0x7800
#define STANDARD 0x0000
#define INVERSE 0x0800
#define STATUS_REQUETE 0x0100
#define AUTORITAIRE 0x0400
#define TRONC 0x0200
#define ASK_RECURSIVITE 0x0100
#define RECURSIVITE_OK 0x0080
#define REP_AUTHENTIFIE 0x0020
#define DATA_NON_AUTHENTIFIE 0x0010
#define FILTRE_RCODE 0x000f
#define NO_ERR 0
#define ERR_FORMAT 1
#define ERR_SERVER 2
#define NO_NAME 3
#define NO_IMPLEM 4
#define REFUS 5

#define LIMITE 100 //limite de caractere a lire si probleme


//derniere partie options dns
void dns_reponses(int type, char* msg, int* p, int* nameposition, const u_char* dnshead){

    int i;
    int var;

    for(i=0; i<type; i++){
        *p+=1;
        printf("        %s : ", msg);
        if(*nameposition == 0){
            *nameposition=*p;
            afficher_ascii(dnshead, p, LIMITE);
        }
        else{
            var=*nameposition;
            afficher_ascii(dnshead, nameposition, LIMITE);
            *nameposition=var;
        }
        printf("\n");
        *p+=4;
    }

}


void fct_dns(const u_char* packet, int size, int v){

    char o;
    int test, i, p=12, nameposition=0;
    int question, reponse, autorite, additionnel;


    const u_char* dnshead=packet+size;

    v_printf(" -> ", v==1);
    printf("DNS");
    v_printf(":\n", v==3);

    if(v==3){
        test=lecture_option(dnshead, 0, 2);
        printf("    ID de la transaction : 0x%.4x\n", test);
        test=lecture_option(dnshead, 2, 2);
        printf("    Flags : (0x%.4x)\n", test);
    }

    if(v>=2){
        if(test & REPONSE){
            printf("        Reponse ");
        }
        else{
            printf("        Requete ");
        }
        switch(test & FILTRE_TYP){
            case STANDARD:
                printf("standard");
                break;
            case INVERSE:
                printf("inverse");
                break;
            case STATUS_REQUETE:
                printf("(status)");
                break;
        }
        printf("\n");
    }

    if(v==3){

        if(test & AUTORITAIRE){
            printf("        Reponse autoritaire\n");
        }
        if(test & TRONC){
            printf("        Message tronque\n");
        }
        if(test & ASK_RECURSIVITE){
            printf("        Demande de recursivite\n");
        }
        if(test & REP_AUTHENTIFIE){
            printf("        Reponse authentifie\n");
        }
        if(test & DATA_NON_AUTHENTIFIE){
            printf("        Donnees non identifies\n");
        }
        printf("        Code de retour : ");
        switch(test & FILTRE_RCODE){
            case NO_ERR:
                printf("Pas d'erreur");
                break;
            case ERR_FORMAT:
                printf("Erreur de format requete");
                break;
            case ERR_SERVER:
                printf("Probleme sur le serveur");
                break;
            case NO_NAME:
                printf("Le nom n'existe pas");
                break;
            case NO_IMPLEM:
                printf("Non implemente");
                break;
            case REFUS:
                printf("Refus");
                break;
        }
        printf("\n");
        question=lecture_option(dnshead, 4, 2);
        printf("    Question : %i entree(s)\n", question);
        reponse=lecture_option(dnshead, 6, 2);
        printf("    Reponse : %i entree(s)\n", reponse);
        autorite=lecture_option(dnshead, 8, 2);
        printf("    Autorite : %i entree(s)\n", autorite);
        additionnel=lecture_option(dnshead, 10, 2);
        printf("    Additionnel : %i entree(s)\n", additionnel);

        printf("    Reponse(s):\n");

        dns_reponses(question, "Question", &p, &nameposition, dnshead);
        dns_reponses(reponse, "Reponse",  &p, &nameposition, dnshead);
        dns_reponses(autorite, "Autorite",  &p, &nameposition, dnshead);
        dns_reponses(additionnel, "Additionnel",  &p, &nameposition, dnshead);

    }
    
}