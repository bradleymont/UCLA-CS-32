#include "provided.h"
#include <string>
using namespace std;

//for the Hash Table
#include "MyHash.h"

//for map
#include <map>

class TranslatorImpl
{
public:
    TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
    
private:
    
    //EVERYTHING IS UPPERCASE
    MyHash<char, char> m_mappingTable;
    
    int m_numTimesPushed;
    int m_numTimesPopped;
    
    vector<map<char, char>> m_history;
    
    void toUpperCase(string& word) const;
};

TranslatorImpl::TranslatorImpl()
{
    map<char, char> m;
    
    for (char c = 'A'; c <= 'Z'; c++)
    {
        m_mappingTable.associate(c, '?');
        
        m[c] = '?';
    }
    
     m_history.push_back(m);  //26 question marks
    
    m_numTimesPushed = 0;
    m_numTimesPopped = 0;
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    bool result = true;
    
    vector<string> tempPushing;
    
    if (ciphertext.size() != plaintext.size())
    {
        return false;
    }
    
    //if either contains a non-letter, return false
    for (int i = 0; i < ciphertext.size(); i++)
    {
        if (       !        (     isalpha(ciphertext[i]) &&    isalpha(plaintext[i])    )    )
        {
            return false;
        }
    }
    
    for (int i = 0; i < ciphertext.size(); i++)
    {
        //check for inconsistency
        char currentCipherTextCharacter = ciphertext[i];
        currentCipherTextCharacter = toupper(currentCipherTextCharacter);
        char currentPlainTextCharacter = plaintext[i];
        currentPlainTextCharacter = toupper(currentPlainTextCharacter);
        
        if ( *m_mappingTable.find(currentCipherTextCharacter) != '?')  //if letter already has a mapping
        {
            //check if it matches current mapping
            
            if (*m_mappingTable.find(currentCipherTextCharacter) != currentPlainTextCharacter)
            {
                return false;
            }

        }
        
        //check to see if a letter already maps to plaintext[i]
        for (char j = 'A'; j <= 'Z'; j++)
        {
            if (j == currentCipherTextCharacter)
            {
                continue;
            }
            char c = *m_mappingTable.find(j);    //the value that A-Z maps to
            if (c == currentPlainTextCharacter)
            {
                return false;
            }
        }
        
        currentCipherTextCharacter = toupper(currentCipherTextCharacter);
        currentPlainTextCharacter = toupper(currentPlainTextCharacter);
        string assoc;
        assoc += currentCipherTextCharacter;
        assoc += currentPlainTextCharacter;
        tempPushing.push_back(assoc);
    }
    
    
    if (result == false) return false;
    
    toUpperCase(ciphertext);
    toUpperCase(plaintext);
    
    //"DHL", "ERD"
    //add to history
    map<char, char> updatedMapping;

    for (int i = 0; i < tempPushing.size(); i++)
    {
        string s = tempPushing[i];
        m_mappingTable.associate(s[0], s[1]);
    }
        
    for (char c = 'A'; c <= 'Z'; c++)
    {
        char currentMapping = *(m_mappingTable.find(c));
        updatedMapping[c] = currentMapping;
    }
    
    
    m_history.push_back(updatedMapping);
    m_numTimesPushed++;
    
    return result;
}

bool TranslatorImpl::popMapping()
{
    if (m_numTimesPopped == m_numTimesPushed)
    {
        return false;
    }
    
    m_history.pop_back();
    
    vector<map<char, char>>::iterator it;
    it = m_history.end();
    it--;

    map<char, char> lastMap = *it;

    for (char c = 'A'; c <= 'Z'; c++)
    {
        m_mappingTable.associate(c, lastMap[c]);
    }
    
    m_numTimesPopped++;
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string translation = ciphertext;
    
    for (int i = 0; i < ciphertext.size(); i++)
    {
        char currentChar = ciphertext[i];
        
        char currentMapping;
        if (isalpha(currentChar))
        {
            currentMapping = *(m_mappingTable.find(toupper(currentChar)));
        }
        else
        {
            currentMapping = currentChar;
        }

        if (isalpha(currentMapping))
        {
            if (isupper(currentChar))
            {
                translation[i] = toupper(currentMapping);
            }
            else
            {
                translation[i] = tolower(currentMapping);
            }
        }
        else if (currentMapping == '?')
        {
            translation[i] = '?';
        }
        else
        {
            translation[i] = currentChar;
        }
    }
    
    return translation;
}

void TranslatorImpl::toUpperCase(string& word) const
{
    for (int i = 0; i < word.size(); i++)
    {
        word[i] = toupper(word[i]);
    }
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}


