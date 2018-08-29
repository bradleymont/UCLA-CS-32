#include "provided.h"
#include <string>
#include <vector>
using namespace std;


class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    
    WordList m_wordList;
    Translator m_translator;
    Tokenizer m_tokenizer;
    
    bool isValidCipherText(const string& ciphertext) const;
    
    int numQuestionMarks(const string& ciphertext) const;
    
    //for picking the next word to translate
    vector<bool> hasBeenChosen;
    bool boolVectorInitialized;
    
    //the final translated message
    vector<string> m_translatedMessage;
};

DecrypterImpl::DecrypterImpl()
:m_tokenizer("0123456789 ,;:.!()[]{}-\"#$%^&")
{
    boolVectorInitialized = false;
}

bool DecrypterImpl::load(string filename)
{
    if (m_wordList.loadWordList(filename))
    {
        return true;
    }
    
    return false;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    if (!isValidCipherText(ciphertext))
    {
        return vector<string>();
    }
    
    //1. start with empty mapping (this is done in the Translator constructor)
    
    //tokenize the ciphertext into separate words
    vector<string> words = m_tokenizer.tokenize(ciphertext);
    
    //2. and pick a ciphertext word w from the message that (a) has not yet been chosen and (b) has the most ciphertext letters for which we don’t have any translation
    //pick the one that has the most ?s (but also hasn't been chosen

    if (!boolVectorInitialized)
    {
        for (int i = 0; i < words.size(); i++)
        {
            hasBeenChosen.push_back(false);
        }
        boolVectorInitialized = true;
    }
    
    int maxQuestionMarks = -1;
    int indexOfNextWord = 0;
    for (int i = 0; i < words.size(); i++)
    {
        string currentWord = words[i];
        string translatedWord = m_translator.getTranslation(currentWord);
        int questionMarks = numQuestionMarks(translatedWord);
        if (questionMarks > maxQuestionMarks && !hasBeenChosen[i])
        {
            indexOfNextWord = i;
            maxQuestionMarks = questionMarks;
        }
    }
    
    hasBeenChosen[indexOfNextWord] = true;
    string w = words[indexOfNextWord];  //w = the word with the most untranslated letters
    
    //3. Translate the chosen encrypted word
    string translation = m_translator.getTranslation(w);
    
    //4 and 5.  create a collection C of all words in the word list that could possibly match the ciphertext word compatibly with the partially decrypted version of the word
    
    vector<string> C = m_wordList.findCandidates(w, translation);
    
    
    //4 and 5 inside recursive call
    if (C.empty())
    {
        hasBeenChosen[indexOfNextWord] = false;
        m_translator.popMapping();
    }
    
    for (int i = 0; i < C.size(); i++)
    {
        //6a
        string p = C[i];
        
        if (!m_translator.pushMapping(w, p))
        {
            //do nothing
        }
        else
        {
            //6b
            string partiallyDecryptedMessage = m_translator.getTranslation(ciphertext);
            
            vector<string> updatedTranslatedWords = m_tokenizer.tokenize(partiallyDecryptedMessage);
            
            int numCorrect = 0;
            //6c - check for correct decryption
            for (int k = 0; k < updatedTranslatedWords.size(); k++)
            {
                string currentWord = updatedTranslatedWords[k];
                if (numQuestionMarks(currentWord) == 0  && m_wordList.contains(currentWord))
                {
                    numCorrect++;
                }
            }
            
            if (numCorrect == updatedTranslatedWords.size())
            {
                m_translatedMessage.push_back(partiallyDecryptedMessage);
                m_translator.popMapping();
                continue;
            }
            
            //6c check all fully translated words
            for (int j = 0; j < updatedTranslatedWords.size(); j++)
            {
                string currentWord = updatedTranslatedWords[j];
                if (numQuestionMarks(currentWord) == 0)    //the word is fully translated
                {
                    if (!m_wordList.contains(currentWord)) //if it doesn't contain the word, then the mapping is wrong
                    {
                        m_translator.popMapping();
                        continue;
                    }
                }
            }
            crack(ciphertext);
        }
    }
    
    return m_translatedMessage;
}

bool DecrypterImpl::isValidCipherText(const string& ciphertext) const
{
    for (int i = 0; i < ciphertext.size(); i++)
    {
        char c = ciphertext[i];
        if ( ! (   isalpha(c)     ||  c == '\'' || isdigit(c) || c == ' ' || c == ',' || c == ';' || c == ':' || c == '.' || c == '!' || c ==  '(' ||    c ==  ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '-' || c == '\"' || c == '#' || c == '$' || c == '%' || c == '^' || c == '&'))
        {
            return false;
        }
    }
    return true;
}

int DecrypterImpl::numQuestionMarks(const string& ciphertext) const
{
    int result = 0;
    for (int i = 0; i < ciphertext.size(); i++)
    {
        if (ciphertext[i] == '?')
        {
            result++;
        }
    }
    return result;
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
    return m_impl->crack(ciphertext);
}

