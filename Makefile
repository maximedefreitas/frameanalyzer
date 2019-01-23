analyseur: delete analyseur.c
	gcc analyseur.c -o analyseur -lpcap

delete:
	rm -rf analyseur
