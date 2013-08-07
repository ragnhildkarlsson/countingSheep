C=gcc
CFLAGS=-I /usr/local/include -c
LDFLAGS=-L /usr/local/lib -lsqlite3
OBJDIR=obj
BINDIR=bin
SRCDIR=src
SRCFILES=esqlite.c
SRC=$(addprefix $(SRCDIR)/,$(SRCFILES))
BIN=$(addprefix $(BINDIR)/,esqlite)
OBJ := $(addprefix $(OBJDIR)/,$(SRCFILES:.c=.o))

all: $(BIN)

$(BIN): $(OBJ) 
	$(CC) $(LDFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	 $(CC) $(CFLAGS) $< -o $@

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	 mkdir $(OBJDIR)

$(BIN): | $(BINDIR)

$(BINDIR):
	 mkdir $(BINDIR)

clean:
	rm $(OBJDIR)/*.o
