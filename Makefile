# Filename:             Makefile
# Date:                 10/17/2022
# Author:               Ethan Arnold
# Email:                EMA190000@utdallas.edu
# Version:              1.0
# Copyright:            2022, All Rights Reserved
#
# Description:
#
#       Part 3 of a project that will display a file's
#       Entry location, file flags, parent entry number,
# 	and file name
#

# Set up needed values to support make's implicit commands
# for compiling C source files

# The compiler to use
CC = gcc

# Project name.  This is used in building the file name for the backup target      
PROJECTNAME = ForensicsProject

# Enter the name for the executable to be created                                  
EXEC = main


SRCS = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJS := $(patsubst %.c,%.o,$(SRCS))

# These targets don't create files that Make should inspect
.PHONY: all clean backup

# The default target.  Its a good idea to use 'all' as the default                 
# so that the commands 'make' and 'make all' do the same thing.                    
all:	$(EXEC)

# Clean up files that we can regenerate or that we don't care about.               
clean:
	rm -f $(OBJS) *.d *~ $(EXEC)

# Pattern for .d files.
#%.d:%.c
#	@echo Updating .d Dependency File
#	@set -e; rm -f $@; \
#	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
#	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
#	rm -f $@.$$$$


#  This is a rule to link the files
$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) -lm


# This allows make to "restart" if it had to change (or create) any
# .d files.  The "restart" allows make to re-read the dependency
# files after that were created or changed.
#Makefile: $(SRCS:.c=.d)

# Backup Target. Invoked through "make backup"
backup: clean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename `pwd`))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME)
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!


# Include the dependency files created by the PreProcessor.  The
# dash in front of the command keeps the system from complaining
# if the files do not exist.  This rule is used in conjunction 
# with the "Makefile" target above.
#-include $(SRCS:.c=.d)
