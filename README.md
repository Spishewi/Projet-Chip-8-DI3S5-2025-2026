# CHIP-8-S5

## Description

L'objectif de ce projet était d'écrire un émulateur CHIP 8 permettant à n’importe quel fichier CHIP 8 ROM d’être lu et exécuté.

## Lancement

Pour démarrer l'émulateur, il faut utiliser le `makefile`. Le plus simple est d'utiliser une des 3 commandes :

- `make build` (crée les executables)
- `make clean` (supprime les fichiers objets temporaires)
- `make run` (build + lance l'executable)

pour lancer une ROM en particulier, la manière la plus simple est de faire :
`make run ROM="la/rom.ch8"`
ou mettre le chemin de la rom en argument de l'executable généré avec `make build`

## Architecture de fichiers

- `include/`
  - `display/`
  - `keyboard/`
  - `misc/`
  - `speaker/`
  - `system/`
    - `cpu.h`
    - `ram.h`
    - `rom.h`
- `lib/`
- `roms/`
- `src/`
  - `system/`
    - `cpu.c`
    - `ram.c`
    - `rom.c`
  - `main.c`
- `LICENSE`
- `makefile`
- `README.md` <-- vous-êtes ici

## Structure du code

Toutes les fonctions ont été réparties dans 3 fichiers sources principaux :
- `cpu.c` comprend l'initialisation du CPU, le fetch-decode-execute ainsi que les 35 instructions, et les timers ;
- `ram.c` comprend les sprites des caractères et toutes les fonctions pour interagir avec la RAM ;
- `rom.c` comprend la fonction pour charger une ROM dans la RAM

Nous avons également 3 fichiers d'entêtes `cpu.h`, `ram.h` et `rom.h` qui comprennent respectivement les déclarations des types et prototypes du CPU, de la RAM, et du chargement de la ROM.

Pour finir, il y a un fichier `main.c` permettant de relier tout les modules, et un `makefile` qui fonctionne avec **Windows** et **Linux**.

## Réalisation des consignes

Les 35 instructions ont été implémentées et fonctionnent. La totalité des ROMs de tests sont opérationnelles et validées :
- ROM 1 CHIP8 logo : fonctionne ;
- ROM 2 IBM logo : fonctionne ;
- ROM 3 corax+ + : tous les tests sont validés ;
- ROM 4 flags : tous les tests sont validés ;
- ROM 5 quirks : "1. CHIP-8" se lance comme prévu et est validé ;
- ROM 6 keypad : toutes les touches prévues sont détectées, les 3 tests sont validés. Cependant, la réactivité des touches est lente ;
- ROM 7 beep : fonctionne avec le test et manuellement ;
- Plusieurs jeux ont été testés notamment : Pong, Tetris et Tron. Les jeux fonctionnent comme ils devraient et l'affichage des scores est opérationnel.

## Auteurs
**Aurèle AUMONT--VESNIER** et **Ethan NOMBELLA**

## Remerciements
Nous voulions remercier **M. BOCQUILLON Ronan** et **M. MONMARCHE Nicolas** de nous avoir accompagnés dans ce projet.

## License

Copyright (C) 2026 Aurèle AUMONT--VESNIER et Ethan NOMBELLA.

chip-8-s5 est un logiciel libre ; vous pouvez le redistribuer et/ou le modifier sous les termes de la GNU General Public License telle que publiée par la Free Software Foundation ; soit la version 3 de la Licence, ou (à votre choix) toute version ultérieure.

chip-8-s5 est distribué dans l’espoir qu’il sera utile, mais SANS AUCUNE GARANTIE ; sans même la garantie implicite de COMMERCIABILITÉ ou APTITUDE À UN USAGE PARTICULIER. Voir la GNU General Public License pour plus de détails.

Vous devriez avoir reçu une copie de la GNU General Public License avec ce programme. Sinon, consultez <http://.gnu.org/licenses/>