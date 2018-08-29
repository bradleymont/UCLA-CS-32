#include "provided.h"
#include <string>
#include <vector>
using namespace std;

//for hash implementations
#include <functional>

//for file I/O
#include <iostream>
#include <fstream>

//for the Hash Table
#include "MyHash.h"

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
    
private:
    
    //private Hash Table
    //the hash table maps a string - EVERY LETTER PATTERN
    //to a vector of strings - ALL WORDS THAT HAVE THAT LETTER PATTERN
    MyHash<string, vector<string> > m_wordAssociations;
    
    void toLowercase(string& word) const;
    
    string getLetterPattern(string s) const;
};

bool WordListImpl::loadWordList(string filename)
{
    ifstream infile(filename);    // infile is a name of our choosing
    
    if ( ! infile )                // Did opening the file fail?
    {
        return false;
    }
    
    string currentWord;
    // getline returns infile; the while tests its success/failure state
    while (getline(infile, currentWord))
    {
        string letterPattern = getLetterPattern(currentWord);
        
        if ( m_wordAssociations.find(letterPattern) == nullptr )     //if the letter pattern is not currently in the hash table
        {
            vector<string> vectorOfWordsWithThatLetterPattern;
            vectorOfWordsWithThatLetterPattern.push_back(currentWord);
            m_wordAssociations.associate(letterPattern, vectorOfWordsWithThatLetterPattern);
        }
        else    //that letter pattern already exists
        {
            m_wordAssociations.find(letterPattern)->push_back(currentWord);
        }
    }
    
    return true;
}

bool WordListImpl::contains(string word) const
{
    string lowerCaseWord = word;
    toLowercase(lowerCaseWord);
    
    string letterPattern = getLetterPattern(lowerCaseWord);
    
    if (m_wordAssociations.find(letterPattern) == nullptr)  //if it doesn't contain the letter pattern, then it for sure doesn't contain the word
    {
        return false;
    }
    else
    {
        vector<string> wordsWithTheSameLetterPattern = *(m_wordAssociations.find(letterPattern));
        
        for (int i = 0; i < wordsWithTheSameLetterPattern.size(); i++)
        {
            string lowerCaseCurrentWord = wordsWithTheSameLetterPattern[i];
            toLowercase(lowerCaseCurrentWord);
            if (lowerCaseCurrentWord == lowerCaseWord)
            {
                return true;
            }
        }
    }
    
    return false;   //we went through all words with the same pattern, and the word still wasn't there
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    vector<string> candidates;
    
    string letterPatternOfCipherWord = getLetterPattern(cipherWord);
    
    //For w to be returned in the vector, it must have
    //the same letter pattern as cipherWord
    
    vector<string> allWordsWithSameLetterPattern;
    
    if (m_wordAssociations.find(letterPatternOfCipherWord) != nullptr)
    {
        allWordsWithSameLetterPattern = *(m_wordAssociations.find(letterPatternOfCipherWord));
    }
    
    string lowerCaseCurrTranslation = currTranslation;
    toLowercase(lowerCaseCurrTranslation);
    

    //iterate through the vector of possible candidates
    for (int i = 0; i < allWordsWithSameLetterPattern.size(); i++)
    {
        string currentWord = allWordsWithSameLetterPattern[i];
        toLowercase(currentWord);
        
        for (int j = 0; j < currentWord.size(); j++)
        {
            if (isalpha(lowerCaseCurrTranslation[j]))   //if it's a letter
            {
                if (lowerCaseCurrTranslation[j] != currentWord[j])
                {
                    break;  //CHECK THIS
                }
            }
            else if (lowerCaseCurrTranslation[j] == '?')
            {
                if (  !  isalpha(currentWord[j]))
                {
                    break;
                }
            }
            else if (lowerCaseCurrTranslation[j] == '\'')    //apostrophe
            {
                if (currentWord[j] != '\'')
                {
                    break;
                }
            }
            
            if (j == currentWord.size() - 1)    //the word satisfied all these cases
            {
                candidates.push_back(currentWord);
            }
        }
    }
    
    return candidates;
}

void WordListImpl::toLowercase(string& word) const
{
    for (int i = 0; i < word.size(); i++)
    {
        word[i] = tolower(word[i]);
    }
}

string WordListImpl::getLetterPattern(string s) const
{
    toLowercase(s);
    string result = s;
    char currentCode = 'A';
    
    // original word = lowercase
    // code = uppercase
    
    for (int i = 0; i < s.size(); i++)
    {
        //if the character has already been encoded, continue
        if (isupper(result[i]))
        {
            continue;
        }
        
        char currentChar = s[i];
        
        for (int j = i; j < s.size(); j++)
        {
            if (s[j] == currentChar)
            {
                result[j] = currentCode;
            }
        }
        
        currentCode++;
    }
    
    return result;
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
    return m_impl->findCandidates(cipherWord, currTranslation);
}

///////////////////////////////////////
/////HASH FUNCTION IMPLEMENTATIONS/////
///////////////////////////////////////

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}
