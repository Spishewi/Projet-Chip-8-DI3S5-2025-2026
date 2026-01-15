# CHIP-8-S5

## Description



## Structure du code

Toutes les fonctions ont été réparties dans 3 fichiers :
- `cpu.c` comprend l'initialisation du CPU, le fetch-decode-execute ainsi que les 35 instructions, et les timers ;
- `ram.c` comprend les sprites des caractères et toutes les fonctions pour interagir avec la RAM ;
- `rom.c` comprend la fonction pour charger une ROM dans la RAM ;
Nous avons également 3 fichiers d'entêtes cpu.h, ram.h et rom.h qui comprennent les déclarations de fonctions et les structures de la RAM et du CPU.
Pour finir, il y a un fichier `main.c`, et un `makefile` qui fonctionne avec les 2 OS Windows et Linux.

## Réalisation des consignes

Les 35 instructions ont été implémentées et fonctionnent. La totalité des ROMs de tests sont opérationnelles et validées :
- ROM 1 CHIP8 logo : fonctionne ;
- ROM 2 IBM logo : fonctionne ;
- ROM 3 corax+ + : tous les tests sont validés ;
- ROM 4 flags : tous les tests sont validés ;
- ROM 5 quirks : 1 CHIP-8 se lance comme prévu et est validé ;
- ROM 6 keypad : toutes les touches prévues sont détectées, les 3 tests sont validés. Cependant, la réactivité des touches est lente ;
- ROM 7 beep : fonctionne avec le test et manuellement ;
- Plusieurs jeux ont été testés notamment : Pong, Tetris et Tron. Les jeux fonctionnent comme ils devraient et l'affichage des scores est opérationnel.

## comment faire fonctionner le projet

### TODO

## Authors and acknowledgment
Show your appreciation to those who have contributed to the project.

## License
For open source projects, say how it is licensed.