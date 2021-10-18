LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++17 -O3
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic
LIBS = `$(LLVMCONFIG) --libs`

BUILDDIR = build

OBJS = $(BUILDDIR)/parser.o $(BUILDDIR)/lexer.o  ${BUILDDIR}/node.o ${BUILDDIR}/codegen.o

all: $(BUILDDIR)/lol-compiler

clean:
	$(RM) -rf $(OBJS)

src/parser.cpp: src/parser.ypp
	bison -d -o $@ $^

src/parser.hpp: src/parser.cpp

src/lexer.cpp: src/lexer.l src/parser.hpp
	flex -o $@ $^

src/node.cpp: src/node.hpp src/decl.hpp

src/codegen.cpp: src/node.hpp src/decl.hpp src/codegen.hpp

$(BUILDDIR)/%.o: src/%.cpp
	g++ -c $< ${CPPFLAGS} -o $@ 

$(BUILDDIR)/lol-compiler: $(OBJS)
	g++ -o $@ $(OBJS) $(LIBS) $(LDFLAGS)


