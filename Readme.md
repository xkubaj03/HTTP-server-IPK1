# HTTP server
## Josef Kuba

## Popis:
Úkolem projektu bylo vytvoøit server komunikující pomocí HTTP protokolu, který bude odpovídat na jednoduché požadavky(hostname, cpu-name a load).

## Použití
Server spustíme pomocí pøíkazu: ./hinfosvc [èíslo_portu] [&]
Èíslo portu je povinný parametr.
Ampersand & je voliterlný parametr kterým spustíme server na pozadí. 
V tomto pøípadì se server nevypíná ctrl + c, ale pomocí pøíkazu kill [èíslo_procesu].
Èíslo procesu je vypsáno po spuštìní serveru.

Klient mùže využít tøi typy dotazù: hostname, cpu-name a load.
Hostname vrací doménové jméno, cpu-name vrací název procesoru a load vrací zátìž procesoru.

### Terminál:
(12345 je èíslo portu na kterém server bìží)
GET http://servername:12345/hostname
curl http://servername:12345/hostname
Vrací napø: merlin.fit.vutbr.cz
GET http://servername:12345/cpu-name
curl http://servername:12345/cpu-name
Vrací napø: Intel(R) Xeon(R) CPU E5-2640 0 @ 2.50GHz
GET http://servername:12345/load
curl http://servername:12345/load
Vrací napø: 20%
### Webový prohlížeè:

http://servername:12345/hostname
Vrací napø: merlin.fit.vutbr.cz
http://servername:12345/cpu-name
Vrací napø: Intel(R) Xeon(R) CPU E5-2640 0 @ 2.50GHz
http://servername:12345/load 
Vrací napø: 20%