

all : $(RCFILE)

$(RCFILE) : $(INTDIR)/*.obj
	NextVersion $(RCFILE)
