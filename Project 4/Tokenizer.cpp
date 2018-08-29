#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
    std::vector<char> m_separators;
    bool isSeparator(char c) const;
};

TokenizerImpl::TokenizerImpl(string separators)
{
    for (int i = 0; i < separators.size(); i++)
    {
        m_separators.push_back(separators[i]);
    }
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    std::vector<std::string> result;
    std::string tempS = s;
    
    bool partOfWord = false;
    int indexOfBeginningOfWord = 0;
    
    for (int i = 0; i < tempS.size(); i++)
    {
        char currentChar = tempS[i];
        bool separator = isSeparator(currentChar);
        
        //if a word hasn't started and the current character is a valid character
        if (!partOfWord && !separator)
        {
            partOfWord = true;
            indexOfBeginningOfWord = i;
        }
        
        //if the current character is a separator that is ending a word
        else if (partOfWord && separator)
        {
            partOfWord = false;
            std::string currentWord = tempS.substr(indexOfBeginningOfWord, i - indexOfBeginningOfWord);
            result.push_back(currentWord);
            tempS = tempS.substr(i + 1);
            i = -1; //it will be pre-incremented to 0 in the next iteration of the loop
        }
        
        //otherwise there's 2 other scenarios, but we do nothing in both cases:
        //1. if a word is in progress and the current character is a valid character, then just keep iterating
        //2. if the current character is a separator that is NOT ending a word, then just keep iterating
        
        //must treat the final word (with no separators after it) separately
        if (i == tempS.size() - 1 && partOfWord)
        {
            std::string currentWord = tempS.substr(indexOfBeginningOfWord);
            result.push_back(currentWord);
        }
    }
    
    return result;
}

bool TokenizerImpl::isSeparator(char c) const
{
    for (int i = 0; i < m_separators.size(); i++)
    {
        if (c == m_separators[i])
        {
            return true;
        }
    }
    return false;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}

