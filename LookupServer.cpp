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

string receivePipe = "request";
string sendPipe = "reply";

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
	Fifo receiveFifo(receivePipe);
	Fifo sendFifo(sendPipe);
	while (true) {

		//open and get the input from the pipe
		receiveFifo.openread();
		string message = receiveFifo.recv();

		//parse the input
		string delim = ":";
		b = atoi(GetNextToken(message, delim).c_str());
		c = atoi(GetNextToken(message, delim).c_str());
		v = atoi(GetNextToken(message, delim).c_str());
		verseCount = atoi(GetNextToken(message, delim).c_str());

		receiveFifo.fifoclose();

		// Create a reference from the numbers
		Ref ref(b, c, v);

		//display the reference
		cout << "Reference: ";
		ref.display();
		cout << endl;

		map<Ref, int> indexMap = webBible.getRefIndex();

		//output the position associated with that reference
		//cout << "The position associated with that reference is: " << indexMap[ref] << endl;
		//cout << "The number of references in the entire index is: " << indexMap.size() << endl;

		//find last index's position
		//maps are automatically sorted, so the last position is actually first
		//map<Ref, int>::iterator mapIter = indexMap.begin();
		//int lastPos = mapIter->second;
		//cout << "The byte offset of the last verse added to the index is: " << lastPos << endl;


		//send the verse and it's status into the reply pipe
		//open the pipe and create a string that will hold the message
		sendFifo.openwrite();
		string replyMessage = "";

		//if there's just one verse, get that verse and assign it to the string
		if (verseCount == 1) {
			verse = webBible.lookup(ref, result);
			replyMessage = verse.getVerse();
			replyMessage = webBible.error(result) + ":" + ref.display() + ":" + to_string(c) + ":" + to_string(v) + ":" + replyMessage;
		}
		//if there's more than one verse, append each of them
		else {
			verse = webBible.lookup(ref, result);

			for (int i = 0; i < verseCount; i++) {
				replyMessage = replyMessage + verse.getVerse();

				verse = webBible.nextVerse(result);
			}
			replyMessage = webBible.error(result) + ":" + ref.display() + ":" + to_string(c) + ":" + to_string(v) + ":" + replyMessage;
		}

		sendFifo.send(replyMessage);
		sendFifo.fifoclose();
	}
}
