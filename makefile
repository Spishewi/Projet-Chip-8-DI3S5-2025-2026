# pour utiliser ce makefile, le plus simple est d'utiliser une des 3 commandes :

# make build (crée les executables)
# make clean (supprime les fichiers objets temporaires)
# make run (build + lance l'executable)

# pour lancer une ROM en particulier, la manière la plus simple est de faire :
# make run ROM="la/rom.ch8"
# ou le mettre le chemin de la rom en argument de l'executable généré avec "make build"


# definitions des différentes commandes que l'ont peux faire
# en réalité on n'utilise seulement clean, run et build
.PHONY: clean run build build-linux build-windows


# Selection de l'OS. On regarde si on est sur windows ou linux.
# En fonction du système que nous utilisons, on traduit les commandes dont on aura besoin.
ifeq ($(OS),Windows_NT)
# On rajoute .exe au nom de l'executable
    EXE_EXT := .exe
# On défini la commande pour supprimer un dossier
    RM_DIR  := rmdir /S /Q
# On défini la fonction pour créer un dossier
    MKDIR_P = if not exist "$(1)" mkdir "$(1)"
# On défini les flags d'édition des liens pour gcc
	LDFLAGS := -Llib/mingw64/ -lprovided -lmingw32 -lSDL2main -lSDL2 -lm
else
# On ne met pas d'extension à l'executable
    EXE_EXT :=
# On défini la fonction pour supprimer un dossier
    RM_DIR  := rm -rf
# On défini la fonction pour créer un dossier
    MKDIR_P = mkdir -p "$(1)"
# On défini les flags d'édition des liens pour gcc
	LDFLAGS := -Llib/x86_64/ -lprovided -lSDL2 -lm
endif

# On défini le nom des dossiers que nous allons utiliser
# le dossier de code source
SRC_DIR := src
# le dossier pour les fichiers objets
OBJ_DIR := bin/obj
# le dossier pour les binaires
BIN_DIR := bin

# flags gcc commun pour la compilation des fichiers
CFLAGS  := -Wall -DDEBUG -I include/

# chemin de l'executable que l'on va générer
EXECUTABLE := $(BIN_DIR)/executable$(EXE_EXT)

# rom a lancer par défaut quand on ne défini rien
DEFAULT_ROM := ./roms/1-chip8-logo.ch8

# rom a lancer. Sera redéfini si besoin au moment de make run
ROM := DEFAULT_ROM

# fonction pour trouver tout les fichiers récursivement
# (cette fonction a été trouvé sur stack-overflow)
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# récupération des fichiers c de manière récursive
SOURCES := $(call rwildcard,$(SRC_DIR)/,*.c)

# définition des fichiers objets à partir de la liste des fichiers sources
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# définition de la commande de build (suivant l'OS qu'on utilise)
build: $(EXECUTABLE)

# Pour un executable donné, on link tout ses fichiers objets
$(EXECUTABLE): $(OBJECTS)
# on crée le dossier s'il n'existe pas
	$(call MKDIR_P,$(BIN_DIR))
# on link l'executable
	gcc $(OBJECTS) $(LDFLAGS) -o $@


# pour chaque fichier source .c, on génère son fichier objet .o correspondant
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# on crée le dossier s'il n'existe pas
	$(call MKDIR_P,$(dir $@))
# on compile le fichier source en fichier executable
	gcc $(CFLAGS) -c $< -o $@

# on clean (supprime tout ce qui est dans le dossier de fichier objet)
clean:
	$(RM_DIR) "$(OBJ_DIR)"

# on compile le projet puis on la lance
run: build
	$(EXECUTABLE) $(ROM)
