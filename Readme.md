# HTTP server
## Josef Kuba

## Popis:
Úkolem projektu bylo vytvořit server komunikující pomocí HTTP protokolu, který bude odpovídat na jednoduché požadavky(hostname, cpu-name a load).

## Použití
Server spustíme pomocí příkazu: ./hinfosvc [číslo_portu] [&]
Číslo portu je povinný parametr.
Ampersand & je voliterlný parametr kterým spustíme server na pozadí. 
V tomto případě se server nevypíná ctrl + c, ale pomocí příkazu kill [číslo_procesu].
Číslo procesu je vypsáno po spuštění serveru.

Klient může využít tři typy dotazů: hostname, cpu-name a load.
Hostname vrací doménové jméno, cpu-name vrací název procesoru a load vrací zátěž procesoru.

### Terminál:
(12345 je číslo portu na kterém server běží)  

GET http://servername:12345/hostname  
curl http://servername:12345/hostname  
Vrací např: merlin.fit.vutbr.cz  

GET http://servername:12345/cpu-name  
curl http://servername:12345/cpu-name  
Vrací např: Intel(R) Xeon(R) CPU E5-2640 0 @ 2.50GHz  

GET http://servername:12345/load  
curl http://servername:12345/load  
Vrací např: 20%  
### Webový prohlížeč:

http://servername:12345/hostname  
Vrací např: merlin.fit.vutbr.cz  

http://servername:12345/cpu-name  
Vrací např: Intel(R) Xeon(R) CPU E5-2640 0 @ 2.50GHz  

http://servername:12345/load   
Vrací např: 20%  
