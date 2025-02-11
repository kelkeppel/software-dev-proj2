// Verse class function definitions
// Computer Science, MVNU

#include "Verse.h"
#include <iostream>
using namespace std;

Verse::Verse() {  	// Default constructor
    verseText = "Uninitialized Verse!";
    verseRef = Ref();
} 

// REQUIRED: Parse constructor - pass verse string from file
Verse::Verse(const string s) {
	// use Ref constructor to create verseRef
	// store the remainder of the string in verseText
    
    //we're using tokens to seperate the line into the ref and the text.
    //then storing those in respective variables
    string seperator = " ";
    //make local copy of the string
    string line = s;
    verseRef = GetNextToken(line, seperator);
    verseText = line;
}  	

// REQUIRED: Accessors
string Verse::getVerse() {
    return verseText;
}

Ref Verse::getRef() {
    return verseRef;
}

// display reference and verse
void Verse::display() {
    verseRef.display();
    cout << " " << verseText;
 }
