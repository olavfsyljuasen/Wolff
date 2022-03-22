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

potts3chain_L10.o : $(POTTSSOURCES)
	$(CCC) $(CCFLAGS) -DPOTTSQ=3 -DPBC -DCHAINLATTICE  -DLINEARSIZE=10 -c -o $@ $<	

potts3chaincorr_L10.o : $(POTTSSOURCES)
	$(CCC) $(CCFLAGS) -DPOTTSQ=3 -DPBC -DCHAINLATTICE  -DLINEARSIZE=10 -DCORRELATION -c -o $@ $<	



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








