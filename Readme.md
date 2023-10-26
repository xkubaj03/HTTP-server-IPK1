# HTTP server
## Josef Kuba

## Popis:
�kolem projektu bylo vytvo�it server komunikuj�c� pomoc� HTTP protokolu, kter� bude odpov�dat na jednoduch� po�adavky(hostname, cpu-name a load).

## Pou�it�
Server spust�me pomoc� p��kazu: ./hinfosvc [��slo_portu] [&]
��slo portu je povinn� parametr.
Ampersand & je voliterln� parametr kter�m spust�me server na pozad�. 
V tomto p��pad� se server nevyp�n� ctrl + c, ale pomoc� p��kazu kill [��slo_procesu].
��slo procesu je vyps�no po spu�t�n� serveru.

Klient m��e vyu��t t�i typy dotaz�: hostname, cpu-name a load.
Hostname vrac� dom�nov� jm�no, cpu-name vrac� n�zev procesoru a load vrac� z�t� procesoru.

### Termin�l:
(12345 je ��slo portu na kter�m server b��)
GET http://servername:12345/hostname
curl http://servername:12345/hostname
Vrac� nap�: merlin.fit.vutbr.cz
GET http://servername:12345/cpu-name
curl http://servername:12345/cpu-name
Vrac� nap�: Intel(R) Xeon(R) CPU E5-2640 0 @ 2.50GHz
GET http://servername:12345/load
curl http://servername:12345/load
Vrac� nap�: 20%
### Webov� prohl�e�:

http://servername:12345/hostname
Vrac� nap�: merlin.fit.vutbr.cz
http://servername:12345/cpu-name
Vrac� nap�: Intel(R) Xeon(R) CPU E5-2640 0 @ 2.50GHz
http://servername:12345/load 
Vrac� nap�: 20%