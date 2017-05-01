PAWNCC=LD_LIBRARY_PATH=compiler/:$(LD_LIBRARY_PATH) ./compiler/pawncc

NAME=WoG
PARAMS=-d2

all:
	$(PAWNCC) $(PARAMS) "-;+" "-(+" "-icompiler/includes" "-ogamemodes/$(NAME).amx" "sources/$(NAME).pwn"

clean:
	rm gamemodes/WoG.lst gamemodes/WoG.asm gamemodes/WoG.amx
