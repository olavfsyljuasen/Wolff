DATADIR= /scratch/sylju
HOMEDIR= /mn/felt/u1/sylju
ABELDIR= $(HOME)
SAGADIR= $(HOME)
OSXDIR = $(HOME)

%.x64 : force
	cp Makefile.x64 Makefile.local
	@echo `git describe`
	make -f makefile $*.exec

%.osx : force
	cp Makefile.osx Makefile.local
	@echo `git describe`
	make -f makefile $*.exec


%.abel : force
	cp Makefile.abel Makefile.local
	@echo `git describe`
	make -f makefile $*.exec

%.saga : force
	cp Makefile.saga Makefile.local
	@echo `git describe`
	make -f makefile $*.exec

%.fox  : force
	cp Makefile.fox Makefile.local
	@echo `git describe`
	make -f makefile $*.exec

# The envirmoment variable MYMAKEFILE contains the name of the appropriate Makefile.local
%    :  force
	cp $(MYMAKEFILE) Makefile.local 
	make -f makefile $*




force : ;


potts_x64  :  potts3chain_L16.x64 potts3chaincorr_L16.x64 potts3square_L8.x64 potts3square_L16.x64 potts3square_L32.x64 potts3square_L64.x64

potts_fox  :  potts3chain_L16.fox potts3chaincorr_L16.fox potts3square_L8.fox potts3square_L16.fox potts3square_L32.fox potts3square_L64.fox




