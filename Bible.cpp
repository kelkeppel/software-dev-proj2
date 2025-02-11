// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
}

// Constructor – pass bible filename
Bible::Bible(const string s) { infile = s; }

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status) { 
    // TODO: scan the file to retrieve the line that holds ref ...
    // update the status variable
	status = OTHER; // placeholder until retrieval is attempted
	// create and return the verse object
	Verse aVerse;   // default verse, to be replaced by a Verse object
	                // that is constructed from a line in the file.

	//loop to keep reading in the line until the numbers at the start of the line
	//match up with our reference
	bool keepSearching = true;
	string currentLine;
	//open the file
	instream.open(infile);

	//verse to return if no verse is found
	Verse defaultVerse = Verse("No Verse Found");

	//check if book number is within valid bounds
	if (!(ref.getBook() > 0 && ref.getBook() <= 66)) {
		status = NO_BOOK;
		return defaultVerse;
	}

	//check that the chap and verse aren't negative
	if (ref.getChap() <= 0) {
		status = NO_CHAPTER;
		return defaultVerse;
	}

	if (ref.getVerse() <= 0) {
		status = NO_VERSE;
		return defaultVerse;
	}


	
	//set boolean variables to determine when we've found the correct reference
	bool chapterFound = false;
	bool verseFound = false;

	//loop through the file
	while (keepSearching) {
		//get the current line from the file
		getline(instream, currentLine);
		aVerse = Verse(currentLine);

		//if the chapters and the book matches up, set chapFound = true
		//if it goes to the next book and chapFound is still false, return no_chapter

		//if the verse, chapter, and book match up, set verseFound = true, return the verse
		//if it goes to the next chapter while verseFound is false, then return no_verse

		//if it somehow gets to the eof without checking any of these conditions, return other status.


		//if the current verse has reached the book that our reference is in
		if (aVerse.getRef().getBook() == ref.getBook()) {
			//if the current verse is in the right chapter
			if (aVerse.getRef().getChap() == ref.getChap()) {
				chapterFound = true;
				//if it found the right verse
				if (aVerse.getRef().getVerse() == ref.getVerse()) {
					verseFound = true;
					status = SUCCESS;
					keepSearching = false;
					return aVerse;
				}

			}
			//if it went past the correct chapter and didn't find the verse
			if (aVerse.getRef().getChap() > ref.getChap()) {
				//if verseFound is still false after the point it should have been found
				//then set status to no_verse and return
				if (verseFound == false) {
					status = NO_VERSE;
					keepSearching = false;
					return defaultVerse;
				}
			}
		}
		//if it went past the book that the chapter should have been found in,
		//then set status to no_chapter and return
		if ((aVerse.getRef().getBook() > ref.getBook())) {
			if (chapterFound == false) {
				status = NO_CHAPTER;
				keepSearching = false;
				return defaultVerse;
			}
		}
	}
		
		return(defaultVerse);
	}



// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status) {
	string nextLine;
	Verse nextVerse;

	//if the file isn't open, then open it
	if (instream.is_open() == false) {
		instream.open(infile);
	}
	
	//check that it hasn't reached the end
	if (!instream.eof()) {
		//get the next line
		getline(instream, nextLine);
		//make it into a verse
		nextVerse = Verse(nextLine);
	}

	return nextVerse;
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status) {
	string message;
	if (status == SUCCESS) {
		message = "Retrieval Succeeded";
	}
	else if (status == NO_BOOK) {
		message = "There is no such book";
	}
	else if (status == NO_CHAPTER) {
		message = "There is no such chapter";
	}
	else if (status == NO_VERSE) {
		message = "There is no such verse";
	}
	else if (status == OTHER) {
		message = "Something went wrong with refrence retrieval";
	}

	return message;
}

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}
	
// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
Ref Bible::next(const Ref ref, LookupResult& status) {
	Ref tempRef = ref;

	int book = tempRef.getBook();
	int chap = tempRef.getChap();
	int verse = tempRef.getVerse();

	//add 1 to the verse to get the next one
	book++;
	Verse nextVerse = lookup(Ref(book, chap, verse), status);
	
	//if the lookup succeeds, then return the reference of that verse
	if (status == SUCCESS) {
		return nextVerse.getRef();
	}
	
	//if the lookup causes an error where it can't find the verse,
	//then add 1 to the chapter and set the verse = 1
	if (status == NO_VERSE) {
		verse = 1;
		chap++;
	}
	//try again
	nextVerse = lookup(Ref(book, chap, verse), status);

	//if that causes an error where the chapter can't be found,
	//then add 1 to the book and set chapter = 1 and verse = 1
	if (status == NO_CHAPTER) {
		verse = 1;
		chap = 1;
		book++;;
	}
	//try again
	nextVerse = lookup(Ref(book, chap, verse), status);

	//if that causes an error where the book can't be found,
	//then there is no next verse (end of the file)
	if (status == NO_BOOK) {
		//just return the last verse in the Bible
		book = 66;
		chap = 22;
		verse = 21;
	}

	return nextVerse.getRef();
}

// OPTIONAL: Return the reference before the given ref
Ref Bible::prev(const Ref ref, LookupResult& status) {
}
