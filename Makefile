INCLUDEDIR=./include
CXX=g++
CFLAGS=-I$(INCLUDEDIR)

OUTDIR=out

_DEPS=lexer.h components/IoC.h components/TokenC.h
	DEPS=$(patsubst %,$(INCLUDEDIR)/%,$(_DEPS))

$(OUTDIR)/%.o: lexer/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) $(DBGFLAG)

$(OUTDIR)/%.o: tests/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) $(DBGFLAG)

TESTOBJS=lexer.o IoC.o lexerTest.o TokenC.o 

lexerTest: $(patsubst %,$(OUTDIR)/%,$(TESTOBJS))
	$(CXX) -o $(OUTDIR)/$@ $^ $(CFLAGS) $(DBGFLAG)

clean:
	rm out/*