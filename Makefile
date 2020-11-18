INCLUDEDIR=include
CXX=clang++
CFLAGS=-I$(INCLUDEDIR)

OUTDIR=out

_DEPS=*.h components/*.h ast/*.h
	DEPS=$(patsubst %,$(INCLUDEDIR)/%,$(_DEPS))

$(OUTDIR)/%.o: lexer/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) $(DBGFLAG)

$(OUTDIR)/%.o: tests/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) $(DBGFLAG)
	
$(OUTDIR)/%.o: ast/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) $(DBGFLAG)

TESTOBJS=lexer.o IoC.o lexerTest.o TokenC.o 

lexerTest: $(patsubst %,$(OUTDIR)/%,$(TESTOBJS))
	$(CXX) -o $(OUTDIR)/$@ $^ $(CFLAGS) $(DBGFLAG)
	
1SCANNER=lexer.o IoC.o TokenC.o 01_scanner.o 
	
1scanner: $(patsubst %,$(OUTDIR)/%,$(1SCANNER))
	$(CXX) -o $(OUTDIR)/$@ $^ $(CFLAGS) $(DBGFLAG)

2PARSER=lexer.o IoC.o TokenC.o 02_parser.o ast.o interpreter.o parser.o TokenHandlerC.o

2parser: $(patsubst %,$(OUTDIR)/%,$(2PARSER))
	$(CXX) -o $(OUTDIR)/$@ $^ $(CFLAGS) $(DBGFLAG)

clean:
	rm out/*