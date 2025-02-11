# Bibleajax program
# Feb 2010, James Skon
# August 2016, Bob Kasper
# updated February 2018 to use paths on cs2.mvnu.edu

# This is the Makefile for the Bible system demo program
# Copy this directory to a location within your home directory. 
# Change the USER name value below to your own user name.
# Then use "make" to build the server program,
# and deploy it to the live web server directory.
# To test the program, go to http://cs2.mvnu.edu/class/csc3004/USER/
# and click on the bibleajax.html link.

# TO DO: Must change this to your username
USER= kelkeppel

CC= g++
CFLAGS= -g -std=c++11

all:	bibleajax.cgi PutCGI PutHTML

# TO DO: add classes from Project 1 to be linked
# in the executable for bibleajax.cgi

bibleajax.cgi:	bibleajax.o Bible.o Ref.o Verse.o
	$(CC) $(CFLAGS) -o bibleajax.cgi bibleajax.o Bible.o Ref.o Verse.o -lcgicc
	# -l option links with cgicc library


bibleajax.o:	bibleajax.cpp
	$(CC) $(CFLAGS) -c bibleajax.cpp

# TO DO: copy actions to build classes from Project 1:
# Bible.o, Ref.o, Verse.o

# Ref Object
Ref.o : Ref.h Ref.cpp
	$(CC) $(CFLAGS) -c Ref.cpp

# Verse Object
Verse.o : Ref.h Verse.h Verse.cpp
	$(CC) $(CFLAGS) -c Verse.cpp

# Bible Object
Bible.o : Ref.h Verse.h Bible.h Bible.cpp
	$(CC) $(CFLAGS) -c Bible.cpp

PutCGI:	bibleajax.cgi
		chmod 755 bibleajax.cgi
		cp bibleajax.cgi /var/www/html/class/csc3004/$(USER)/cgi-bin

		echo "Current contents of your cgi-bin directory: "
		ls -l /var/www/html/class/csc3004/$(USER)/cgi-bin/

PutHTML:
		cp bibleajax.html /var/www/html/class/csc3004/$(USER)

		echo "Current contents of your HTML directory: "
		ls -l /var/www/html/class/csc3004/$(USER)

clean:		
		rm *.o core bibleajax.cgi