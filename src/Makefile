

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++17
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic
LIBS = `$(LLVMCONFIG) --libs`

BUILDDIR = build

OBJS = $(BUILDDIR)/parser.o $(BUILDDIR)/lexer.o  ${BUILDDIR}/node.o

all: $(BUILDDIR)/parser

clean:
	$(RM) -rf $(OBJS)

parser.cpp: parser.ypp
	bison -d -o $@ $^

parser.hpp: parser.cpp 

lexer.cpp: lexer.l parser.hpp 
	flex -o $@ $^

$(BUILDDIR)/%.o: %.cpp 
	g++ -c $< ${CPPFLAGS} -o $@ 

$(BUILDDIR)/parser: $(OBJS)
	g++ -o $@ $(OBJS) $(LIBS) $(LDFLAGS)


