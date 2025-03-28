/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * Mount Vernon Nazarene University
 *
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 *     refers to the actual string entered in the form's "verse" field.
 */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "fifo.h"
using namespace std;

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
using namespace cgicc;

//include stuff for Bible files
#include "Ref.h"
#include "Verse.h"
#include "Bible.h"

string requestPipe = "request";
string replyPipe = "reply";

int main() {
	/* A CGI program must send a response header with content type
	 * back to the web client before any other output.
	 * For an AJAX request, our response is not a complete HTML document,
	 * so the response type is just plain text to insert into the web page.
	 */
	cout << "Content-Type: text/plain\n\n";

	Cgicc cgi;  // create object used to access CGI request data

	// GET THE INPUT DATA
	// browser sends us a string of field name/value pairs from HTML form
	// retrieve the value for each appropriate field name
	form_iterator st = cgi.getElement("search_type");
	form_iterator book = cgi.getElement("book");
	form_iterator chapter = cgi.getElement("chapter");
	form_iterator verse = cgi.getElement("verse");
	form_iterator nv = cgi.getElement("num_verse");

	// Convert and check input data
	bool validInput = false;

	bool chapterValid = false;
	//move declaration outside of statement so we can use variable elsewhere
	int chapterNum;

	if (chapter != cgi.getElements().end()) {
		chapterNum = chapter->getIntegerValue();
		if (chapterNum > 150) {
			cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
		}
		else if (chapterNum <= 0) {
			cout << "<p>The chapter must be a positive number.</p>" << endl;
		}
		else
			chapterValid = true;
	}

	/* TO DO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you! */

	//check that the book number is within bounds
	//technically this is redundant since there's error checking within the lookup function
	//but for the sake of black box programming we'll do what error checking we can outside of the function
	int bookNum = book->getIntegerValue();
	bool bookValid = false;

	if (book != cgi.getElements().end()) {
		if (bookNum < 1) {
			cout << "<p> Book number cannot be less than 1 </p>" << endl;
		}
		else if (bookNum > 66) {
			cout << "<p> Book number cannot be higher than 66 </p>" << endl;
		}
		else {
			bookValid = true;
		}
	}

	int verseNum = verse->getIntegerValue();
	bool verseValid = false;

	if (verse != cgi.getElements().end()) {
		if (verseNum < 1) {
			cout << "<p> Verse number cannot be less than 1 </p>" << endl;
		}
		else if (verseNum > 176) {
			cout << "<p> Verse number cannot be higher than 176 </p>" << endl;
		}
		else {
			verseValid = true;
		}
	}

	//get the number of verses to retrieve and display
	int numVerses = nv->getIntegerValue();

	/* TO DO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
	 *        TO LOOK UP THE REQUESTED VERSES
	 */
	 

	//create the pipe variables
	Fifo requestFifo(requestPipe);
	Fifo replyFifo(replyPipe);

	//write the reference we want to the request pipe
	requestFifo.openwrite();
	string requestMessage = "";
	//the format we designed for the request was Book:Chapter:Verse:numOfVerses
	string colon = ":";
	requestMessage = to_string(bookNum) + colon + to_string(chapterNum) + colon + to_string(verseNum) + colon + to_string(numVerses);
	//write the message into the pipe
	requestFifo.send(requestMessage);
	requestFifo.fifoclose();

	//get the response from the server with the verse(s)
	replyFifo.openread();
	string reply = replyFifo.recv();
	replyFifo.fifoclose();
	//parse the first part of the message (result status) and store it
	//string delim = ":";
	//string resultStatus = GetNextToken(reply, delim);
	//display the rest of the message to the user

	/* SEND BACK THE RESULTS
	 * Finally we send the result back to the client on the standard output stream
	 * in HTML text format.
	 * This string will be inserted as is inside a container on the web page,
	 * so we must include HTML formatting commands to make things look presentable!
	 */

	cout << "Search Type: <b>" << **st << "</b>" << endl;
	cout << "<p>Your result: " << endl;
	//the reply message from the pipe contains the reference and each verse, so we can display as is
	cout << reply << endl;
	cout << "</p>" << endl;
	return 0;
}
