#include "hangman.h"

Hangman::Hangman()
{
    this->iIncorrectLetterCount = 0;
    this->iPointcounter = 0;
    this->sHint = "";
    this->sUserName ="";
    this->bIncremented = false;
}

// Asks the user to give a username

// gets the user name
string Hangman::getName() {
    return this->sUserName;
}

// sets the current term
void Hangman::setTerm(StringList& termsList, unsigned int & iPos) {

    this->iPos = iPos;
    wordControler.setTerm(termsList.getSelectedTerm(this->iPos));
    iPointcounter = 0;
    iIncorrectLetterCount = 0;
}

// overloaded method for db access
void Hangman::setTerm(QString& table, unsigned int& key)
{
    this->iPos = key;
    this->table = table;
    QPair<QString, QString> pair;
    pair = DbProxy::getProxy()->getTermHintPair(table, key);
    wordControler.setTerm(pair.first.toStdString());
    iPointcounter = 0;
    iIncorrectLetterCount = 0;
}

void Hangman::startGame()
{
    bIncremented = false;
	try {
        getCurrentTerm();
        cout << "Guess a letter for the word" << endl;
        // verifiys that the user provided a char
        for(char cValue : sGuess){
            if(cValue != ' ') {

                cValue = tolower(cValue);
                //gets the guest from the user.
                wordControler.getGuess(cValue);
                //checks to see how many times the user has guested the word

                if(wordControler.getbWordupdate()) {
                    bIncremented = true;
                    iPointcounter++;
                }
             }
        }
		// displays the currently know letters
        getCurrentTerm();
        //checks if the value was equal to or greater then 10

	}
	catch (int e) {
		cout << "An exception has occured.  " << e << '\n';
	}

    if (!bIncremented && iIncorrectLetterCount <= 10 ) {
        cout << "Letter not found" << endl;
        iIncorrectLetterCount++;
        //sHint = wordControler.giveHint(this->iPos);
    }
    if (wordControler.runHangman()) {
        iPointcounter+= 10;
        highscore.updateHighScore(sUserName, iPointcounter);
    }

    bIncremented = false;
	//updates the high score file
	highscore.updateFile();
	//clears out word controler
	wordControler.clearOutWord();
}

// Sets the value for guess
void Hangman::setGuess(string& sGuess) {
    this->sGuess = sGuess;
}

// sets the values for name
void Hangman::setName(string userName) {
    this->sUserName = userName;
}

// gets the value for points
string Hangman::getCurrentPoints() {
    return to_string(iPointcounter);
}

// gets the value for points
string Hangman::getIncorrectLetters() {
    return to_string(iIncorrectLetterCount);
}

// gets the value for the current term
string Hangman::getCurrentTerm() {
    return wordControler.displayKnowLetters();
}

// gets the value for hint.
string Hangman::getHint() {
    return sHint = wordControler.giveHint(this -> iPos);
}

// gets a hint from the db record
string Hangman::getHintDb()
{
    return sHint = wordControler.giveHintDb(this->table, this->iPos);
}

// returns whether the score was updated after the previous guess.
bool Hangman::scoreIncremented() {
    return bIncremented;
}

// displays the high score to the user.  


