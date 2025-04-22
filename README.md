# PriseConnectée_ProjetBTS
Code C pour l'esp32 du projet de fin de BTS CIEL

## Contexte 
Le projet de fin de deuxième année de BTS-CIEL doit intégrer une solution de type 'capteur connecté/IOT', cela signifie qu'il faut que ce projet inclue un capteur et un microcontroleur pour transmettre les données, celles-ci serront ensuite envoyée vers une base de donnée pour être traitées, soit pour être affiché sur un tableau de bord soit pour être envoyé sous forme de rapport par mails.

### Problématique
Dans un soucis écologique et économique nous voudrions améliorer la durée de vie de nos PC portable. En effet un grand nombre de PC portable restent branchés sur secteur lorsque leurs batterie est pleine, ce qui a pour effet de réduir la durée de vie de celle-ci. 

Afin de remédier à ce problème j'ai décidé de develloper une solution qui permettrait de controler la charge d'un PC, la couper lorsque la batterie est pleine et la relancer lorsque celle-ci descend sous le seuille des 50%. Lors de la charge du PC la consommation électrique sera mesuré et envoyé par mail à l'utilisateur du PC.

### Projet 
Vous retrouverez sur ce repository le code du microcontroleur qui est en charge de gérer le relai qui contrôle la charge, l'ampermetre qui récupère les données ainsi que le traitement des données (publication via MQTT).

Pour ce projet j'ai choisie un ESP32, un microcontroleur grandement utilisé dans le monde de l'IOT, il est multicoeur et peut posséder une carte WIFI, deux atouts nécessaire à la réalisation du projet. Le code de l'application embarqué sur ce microcontroleur est écris en C grace au framework ESP-IDF.