
Installation:
    -> sudo apt-get install libpcap-dev
            (bibliotheque pcap)
    -> sudo make

Execution:
    -> sudo ./analyseur +optionq
        -i <nom d'une interface>
        -o <nom de fichier>
        -v <1..3>
            3 niveaux de verbosite:
                1: en une ligne
                2: une ligne par protocole
                3: affichage complet


Protocoles implementes:
    -ethernet
    -IPv4
    -IPv6
    -ARP
    -TCP
    -UDP
    -SCTP (chunks non implementes)
    -BOOTP/DHCP
    -DNS (bugs observe avec utilisation d'une interface)
    -HTTP
    -Telnet
    -FTP
    -SMTP
    -POP
    -IMAP