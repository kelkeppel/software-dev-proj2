// biblereader.cpp
// main function for Project 1

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main (int argc, char **argv) {
	// Create Bible object to process the raw text file
	Bible webBible("/home/class/csc3004/Bibles/web-complete");
	
	Verse verse;
	int b, c, v;
	//by default it'll only return 1 verse
	int verseCount = 1;
	LookupResult result;
	cout << "Using Bible from: ";
	webBible.display();
	// Prompt for input: get reference to lookup
	// This is good for testing your classes quickly for the first milestone.
	// TODO: your final program should get input from command line arguments instead.
	
	if (argc == 5) {
		//enter optional parameter of how many verses to show
		verseCount = atoi(argv[4]);
		b = atoi(argv[1]);
		c = atoi(argv[2]);
		v = atoi(argv[3]);
	}

	else if (argc == 4) {
		b = atoi(argv[1]);
		c = atoi(argv[2]);
		v = atoi(argv[3]);
	}

	else if (argc == 3) {
		cout << "Error: Verse number missing" << flush;
		return -1;
	}
	else if (argc == 2) {
		cout << "Error: Chapter number missing" << flush;
		return -1;
	}
	else if (argc == 1) {
		cout << "Enter 3 integers for book, chapter and verse to find: " << flush;
		cin >> b >> c >> v;
	}


	// Create a reference from the numbers
	Ref ref(b, c, v);

	//KELSEY TO DO: add for loop to display verse and get next verse
	//figure out how the hyeck to format that
	cout << "Reference: ";
	ref.display();
	cout << endl;

	if (verseCount == 1) {
		verse = webBible.lookup(ref, result);
		verse.display();
		cout << endl;
	}
	else {
		verse = webBible.lookup(ref, result);
		for (int i = 0; i < verseCount; i++) {
			verse.display();

			verse = webBible.nextVerse(result);
			cout << endl;
		}
	}

	cout << "Result status: " << webBible.error(result) << endl;
	cout << endl;
}
