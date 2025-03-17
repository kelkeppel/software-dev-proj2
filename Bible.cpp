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
#include <map>
using namespace std;

Bible::Bible() { // Default constructor
	infile = "/home/class/csc3004/Bibles/web-complete";
	buildRefIndex();
}

// Constructor – pass bible filename
Bible::Bible(const string s) {
	infile = s;
	buildRefIndex();
 }

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

	//get the verse
	int requestedPos = refIndex[ref];

	//jump to that position
	instream.open(infile);
	instream.clear();
	instream.seekg(requestedPos, std::ios_base::beg);

	//THIS IS THE PROBLEM CHILD LINE
	getline(instream, currentLine);
	aVerse = Verse(currentLine);

	
	//set boolean variables to determine when we've found the correct reference
	bool chapterFound = false;
	bool verseFound = false;

	return(aVerse);
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

//create accessor for the ref index
std::map<Ref, int> Bible::getRefIndex() {
	return refIndex;
}

void Bible::buildRefIndex() {
	instream.open(infile);
	int position = instream.tellg();
	cout << "tellg position: " << position << endl;
	string currentLine;

	while (getline(instream, currentLine)) {
		//getline(infile, currentLine);

		//parse the reference for each line
		//afraid to mess something up by using a destructive function on the current line (even though it's theoretically fine, shouldn't affect file)
		string copyOfCurrentLine = currentLine;
		Ref currentRef = Ref(copyOfCurrentLine);

		refIndex.insert({ currentRef, position });

		//get the next line's position
			position = instream.tellg();

	}
}
