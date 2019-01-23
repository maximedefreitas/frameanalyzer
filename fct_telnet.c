#define COMMANDE 0xff

//type de commande
#define WILL 251
#define WONT 252
#define DO 253
#define DONT 254

//option
#define BIN_TRANS 0
#define ECHO 1
#define RECONNECT 2
#define SUPP_GO_AHEAD 3
#define STATUS 5
#define TERM_TYPE 24
#define NEG_WIN_SIZE 31
#define TERM_SPEED 32
#define REM_FLOW_CTR 33
#define LINEMODE 34
#define X_DIS_LOC 35
#define AUT_OPT 37
#define NEW_ENV_OPT 39

void fct_telnet(const u_char* packet, int size, int app_size, int v){

    int i=0;

    v_printf(" -> ", v==1);
    printf("Telnet");
    v_printf(":\n", v==3);

    if(v==3){

        while(app_size-i>0){

            if(packet[size+i]==COMMANDE){

                printf("    ");

                //type de commande
                switch(packet[size+i+1]){
                    case WILL:
                        printf("WILL ");
                        break;
                    case WONT:
                        printf("WONT ");
                        break;
                    case DO:
                        printf("DO ");
                        break;
                    case DONT:
                        printf("DONT ");
                        break;
                    default:
                        printf("unknown ");
                }

                //option
                switch(packet[size+i+2]){
                    case BIN_TRANS:
                        printf("Binary Transmition\n");
                        break;
                    case ECHO:
                        printf("Echo\n");
                        break;
                    case RECONNECT:
                        printf("Reconnection\n");
                        break;
                    case SUPP_GO_AHEAD:
                        printf("Suppress Go Ahead\n");
                        break;
                    case STATUS:
                        printf("Status\n");
                        break;
                    case TERM_TYPE:
                        printf("Termial Type\n");
                        break;
                    case NEG_WIN_SIZE:
                        printf("Negotiate About Window Size\n");
                        break;
                    case TERM_SPEED:
                        printf("Terminal Speed\n");
                        break;
                    case REM_FLOW_CTR:
                        printf("Remote Flow Control\n");
                        break;
                    case LINEMODE:
                        printf("Linemode\n");
                        break;
                    case X_DIS_LOC:
                        printf("X Display Location\n");
                        break;
                    case AUT_OPT:
                        printf("Authentication Option\n");
                        break;
                    case NEW_ENV_OPT:
                        printf("New Environment Option\n");
                        break;
                    default:
                        printf("unknown\n");

                }

                i+=3;

            }
            else{
                printf("    ");
                afficher_ascii(packet, &size, app_size);
                return;
            }

        }

    }

}