include Makefile.local


POTTSSOURCES =  wolff.C



%.run  : %.exec
	./$<

%.exec : %.o 
	$(CCC) $(LINKOPTS) -o $@ $^ $(LIBDIR) $(LIBS)
	@echo $*.`git describe`$(EXTENSION)
	mv $*.exec $(BINDIR)/$*.`git describe`$(EXTENSION)


potts3square_L8.o : $(POTTSSOURCES)
	$(CCC) $(CCFLAGS) -DPOTTSQ=3 -DPBC -DSQUARELATTICE -DLINEARSIZE=8 -c -o $@ $<

potts3square_L16.o : $(POTTSSOURCES)
	$(CCC) $(CCFLAGS) -DPOTTSQ=3 -DPBC -DSQUARELATTICE -DLINEARSIZE=16 -c -o $@ $<

potts3square_L32.o : $(POTTSSOURCES)
	$(CCC) $(CCFLAGS) -DPOTTSQ=3 -DPBC -DSQUARELATTICE -DLINEARSIZE=32 -c -o $@ $<

potts3square_L64.o : $(POTTSSOURCES)
	$(CCC) $(CCFLAGS) -DPOTTSQ=3 -DPBC -DSQUARELATTICE -DLINEARSIZE=64 -c -o $@ $<

potts3chain_L16.o : $(POTTSSOURCES)
	$(CCC) $(CCFLAGS) -DPOTTSQ=3 -DPBC -DCHAINLATTICE  -DLINEARSIZE=16 -c -o $@ $<	

potts3chaincorr_L16.o : $(POTTSSOURCES)
	$(CCC) $(CCFLAGS) -DPOTTSQ=3 -DPBC -DCHAINLATTICE  -DLINEARSIZE=16 -DCORRELATIONS -c -o $@ $<	



spotless:	
	make clean
	rm -f *.ps

clean	:
	rm -f *.dvi
	rm -f *.aux
	rm -f *.log
	rm -f *~
	rm -f core
	rm -f *.o
	rm -f *.exec
	rm -f *.d








