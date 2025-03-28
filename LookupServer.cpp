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
#include "fifo.h"
using namespace std;

string requestPipe = "request";
string replyPipe = "reply";

int main(int argc, char** argv) {
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

	//create the pipes
	Fifo requestFifo(requestPipe);
	Fifo replyFifo(replyPipe);
	while (true) {

		//open and get the input from the pipe
		requestFifo.openread();
		string message = requestFifo.recv();

		//parse the input
		string delim = ":";
		b = atoi(GetNextToken(message, delim).c_str());
		c = atoi(GetNextToken(message, delim).c_str());
		v = atoi(GetNextToken(message, delim).c_str());
		verseCount = atoi(GetNextToken(message, delim).c_str());

		requestFifo.fifoclose();

		// Create a reference from the numbers
		Ref ref(b, c, v);

		//display the reference
		cout << "Reference: ";
		ref.display();
		cout << endl;

		//create the map from the Bible file
		map<Ref, int> indexMap = webBible.getRefIndex();

		//send the verse and it's status into the reply pipe
		//open the pipe and create a string that will hold the message
		replyFifo.openwrite();
		string replyMessage = "";

		//if there's just one verse, get that verse and assign it to the string
		if (verseCount == 1) {
			verse = webBible.lookup(ref, result);

			//if the lookup status is SUCCESS, then continue making the message
			if (result == SUCCESS) {
				replyMessage = verse.getVerse();
				//create the reply formatting with the error status, refrence, and verse
				replyMessage = webBible.error(result) + ":" + ref.display() + ":" + to_string(c) + ":" + to_string(v) + ":" + replyMessage;
			}
			else {
				replyMessage = webBible.error(result);
			}
		}
		//if there's more than one verse, append each of them
		else {
			verse = webBible.lookup(ref, result);

			//if lookup is a success, then continue building the return message
			if (result == SUCCESS) {
				for (int i = 0; i < verseCount; i++) {
					replyMessage = replyMessage + verse.getVerse();

					verse = webBible.nextVerse(result);
				}
				//create the reply formatting with the error status, refrence, and verses
				replyMessage = webBible.error(result) + ":" + ref.display() + ":" + to_string(c) + ":" + to_string(v) + ":" + replyMessage;
			}
			//if there was an error, then just return that
			else {
				replyMessage = result + ":" + webBible.error(result);
			}
		}

		replyFifo.send(replyMessage);
		replyFifo.fifoclose();
	}
}
