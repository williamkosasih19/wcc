INCLUDEDIR=include
CXX=clang++
CFLAGS=-I$(INCLUDEDIR)

OUTDIR=out

_DEPS=*.h components/*.h ast/*.h
	DEPS=$(patsubst %,$(INCLUDEDIR)/%,$(_DEPS))

$(OUTDIR)/%.o: */%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS) $(DBGFLAG)
	
$(OUTDIR)/%.o: %.cpp $(DEPS)
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
	
4ASSEMBLY=lexer.o IoC.o TokenC.o ast.o interpreter.o parser.o TokenHandlerC.o codegen.o 04_assembly.o common.o
4assembly: $(patsubst %,$(OUTDIR)/%,$(4ASSEMBLY))
	$(CXX) -o $(OUTDIR)/$@ $^ $(CFLAGS) $(DBGFLAG)
	
6VARIABLES=lexer.o IoC.o TokenC.o ast.o interpreter.o parser.o TokenHandlerC.o codegen.o 04_assembly.o common.o symbolTable.o
6variables: $(patsubst %,$(OUTDIR)/%,$(6VARIABLES))
	$(CXX) -o $(OUTDIR)/$@ $^ $(CFLAGS) $(DBGFLAG)
	./out/6variables tests/6variables/input01 > out/6variables_asm1.s
	./out/6variables tests/6variables/input02 > out/6variables_asm2.s
	./out/6variables tests/6variables/input02 > out/6variables_asm3.s
	cc out/6variables_asm1.s -o out/6_variables_bin_1
	cc out/6variables_asm2.s -o out/6_variables_bin_2
	cc out/6variables_asm3.s -o out/6_variables_bin_3
	
7comparisons: $(patsubst %,$(OUTDIR)/%,$(6VARIABLES))
	$(CXX) -o $(OUTDIR)/$@ $^ $(CFLAGS) $(DBGFLAG)
	./out/7comparisons tests/7comparisons/input01 > out/7comparisons_asm1.s
	./out/7comparisons tests/7comparisons/input02 > out/7comparisons_asm2.s
	./out/7comparisons tests/7comparisons/input02 > out/7comparisons_asm3.s
	./out/7comparisons tests/7comparisons/input04 > out/7comparisons_asm4.s
	cc out/7comparisons_asm1.s -o out/7comparisons_bin_1
	cc out/7comparisons_asm2.s -o out/7comparisons_bin_2
	cc out/7comparisons_asm3.s -o out/7comparisons_bin_3
	cc out/7comparisons_asm4.s -o out/7comparisons_bin_4
	
clean:
	rm out/*	