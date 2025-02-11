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
  
  /* TO DO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
   *        TO LOOK UP THE REQUESTED VERSES
   */
  //declare Bible object
  Bible webBible("/home/class/csc3004/Bibles/web-complete");

  //get the verse number
  int verseNum = verse->getIntegerValue();

  Ref ref = Ref(bookNum, chapterNum, verseNum);
  LookupResult result;

  //create a verse to hold result
  Verse retrievedVerse;

  //most of the error checking is done within the lookup function itself
  //so after checking a few basic conditions, we'll call the function and see what it returns
  if (bookValid == true && chapterValid == true) {
	  retrievedVerse = webBible.lookup(ref, result);

	  if (result == SUCCESS) {
		  validInput = true;
	  }
	  else if (result == NO_BOOK) {
		  validInput = false;
		  cout << "<p> No such book </p>" << endl;
	  }
	  else if (result == NO_CHAPTER) {
		  validInput = false;
		  cout << "<p> No such chapter </p>" << endl;
	  }
	  else if (result == NO_VERSE) {
		  validInput = false;
		  cout << "<p> No such verse </p>" << endl;
	  }
	  else {
		  validInput = false;
		  cout << "<p> Something went wrong </p>" << endl;
	  }
  }

  /* SEND BACK THE RESULTS
   * Finally we send the result back to the client on the standard output stream
   * in HTML text format.
   * This string will be inserted as is inside a container on the web page, 
   * so we must include HTML formatting commands to make things look presentable!
   */
  if (validInput) {
	cout << "Search Type: <b>" << **st << "</b>" << endl;
	cout << "<p>Your result: "
		<< **book << " " << **chapter << ":" << **verse
		<< "<em>" << **nv;
		retrievedVerse.display();
		cout << "</em></p>" << endl;
  }
  else {
	  cout << "<p>Invalid Input: <em>report the more specific problem.</em></p>" << endl;
  }
  return 0;
}
