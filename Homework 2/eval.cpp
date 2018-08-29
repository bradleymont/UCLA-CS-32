//
//  eval.cpp
//  EvaluateInfix
//
//  Created by Bradley Mont on 2/3/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

//declarations of any additional functions you might have written to help you evaluate an expression
bool isSyntacticallyValid(string infix);
void removeSpaces(string& s);
bool checkAndExtractParentheses(string &s);
bool checkPrecedence(char ch, char other);

int evaluate(string infix, const Map& values, string& postfix, int& result);
/*
 Evaluates an integer arithmetic expression
   If infix is a syntactically valid infix integer expression whose
   only operands are single lower case letters (whether or not they
   appear in the values map), then postfix is set to the postfix
   form of the expression;
 
   otherwise postfix may or may not be changed, result is unchanged, and the function returns 1.
 
   If infix is syntactically valid but contains at least one lower
   case letter operand that does not appear in the values map, then
   result is unchanged and the function returns 2.
 
   If infix is syntactically valid and all its lower case operand letters
   appear in the values map, then if evaluating the expression
   (using for each letter in the expression the value in the map
   that corresponds to it) attempts to divide by zero, then result
   is unchanged and the function returns 3;
 
   otherwise, result is set to the value of the expression and the function returns 0.
 */

int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
           pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
    // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
           pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
           pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
           pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    cout << "Passed all tests" << endl;
}

int evaluate(string infix, const Map& values, string& postfix, int& result) //Evaluates an integer arithmetic expression
{
    //if not syntactically valid, result is unchanged, and the function returns 1.
    if (!isSyntacticallyValid(infix))
    {
        return 1;
    }
    
//    If infix is a syntactically valid infix integer expression whose
//    only operands are single lower case letters (whether or not they
//    appear in the values map), then postfix is set to the postfix
//    form of the expression;
    
    //convert infix to postfix
    postfix = "";
    
    stack<char> operators;
    
    for (int k = 0; k < infix.size(); k++)
    {
        char ch = infix[k];
    
        switch (ch)
        {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
                postfix += ch;
                break;
                
            case '(':
                operators.push(ch);
                break;
            
            case ')':
                // pop stack until matching '('
                while (operators.top() != '(')
                {
                    postfix += operators.top();
                    operators.pop();
                }
    
                operators.pop();    // remove the '('
                break;
                
//case operator:
            case '+':
            case '-':
            case '*':
            case '/':
            
//    While the stack is not empty and the stack top is not '(' and precedence of ch <= precedence of stack top
            while (!operators.empty()  &&   operators.top() != '(' &&   checkPrecedence(ch, operators.top()))
            {
                postfix += operators.top();
                operators.pop();
            }
            operators.push(ch);
            break;
        }
    }
    
    while (!operators.empty())
    {
        postfix += operators.top();
        operators.pop();
    }
    
    //If infix is syntactically valid but contains at least one lower
    //case letter operand that does not appear in the values map, then
    //result is unchanged and the function returns 2.
    for (int k = 0; k < infix.size(); k++)
    {
        if (islower(infix[k]))
        {
            if (!values.contains(infix[k]))
            {
                return 2;
            }
        }
    }
    
    //evaluation of the postfix expression:
    stack<int> operands;

    for (int k = 0; k < postfix.size(); k++)
    {
        char ch = postfix[k];
        
        if (islower(ch))
        {
            int value = 0;
            values.get(ch, value);
            operands.push(value);
        }
        else    // ch is a binary operator
        {
            int operand2 = operands.top();
            operands.pop();
            int operand1 = operands.top();
             operands.pop();
            int myResult;
            
            switch(ch)
            {
                case '+':
                    myResult = operand1 + operand2;
                    break;
                case '-':
                    myResult = operand1 - operand2;
                    break;
                case '*':
                    myResult = operand1 * operand2;
                    break;
                case '/':
//                  If infix is syntactically valid and all its lower case operand letters
//                  appear in the values map, then if evaluating the expression
//                  (using for each letter in the expression the value in the map
//                  that corresponds to it) attempts to divide by zero, then result
//                  is unchanged and the function returns 3;
                    if (operand2 == 0)
                    {
                        return 3;
                    }
                    myResult = operand1 / operand2;
                    break;
            }
            operands.push(myResult);
        }
    }

    result = operands.top();;
    return 0;
}

///////////////////////////////////////////
////////////HELPER FUNCTIONS///////////////
///////////////////////////////////////////

bool isSyntacticallyValid(string infix)
{
    removeSpaces(infix);
    
    //check for empty string
    if (infix == "")
    {
        return false;
    }

    if (!checkAndExtractParentheses(infix))     //this checks everything involving parentheses and then removes them
    {
        return false;
    }
    
    int numLetters = 0;
    int numOperators = 0;
    
    bool mustBeLetter = true;
    
    for (int k = 0; k < infix.size(); k++)
    {
        char current = infix[k];
            
        if (k == 0)  //handle first character differently - must be a letter (since spaces and parentheses have been removed)
        {
            if (!islower(current))  //if the first letter isn't a letter, return false
            {
                return false;
            }
            mustBeLetter = false;   //the next character must not be a letter since this character is a letter
            numLetters++;
            continue;
        }
        
        if (k == infix.size() - 1)  //handle last character differently - must be letter
        {
            if (!islower(current) || mustBeLetter == false)     //if the last character isn't a letter (or is preceded by a letter
            {
                return false;
            }
            numLetters++;
            continue;
        }
        
        //for everything else
        if (islower(current))   //if it's a lowercase letter
        {
            if (mustBeLetter == false)  //if it's not supposed to be a letter
            {
                return false;
            }
            else
            {
                mustBeLetter = false;
                numLetters++;
                continue;
            }
        }
        else if (current == '+' || current == '-' || current == '*' || current == '/')   //if it's an operator
        {
            if (mustBeLetter == true)   //if it's not supposed to be an operator
            {
                return false;
            }
            else
            {
                mustBeLetter = true;
                numOperators++;
                continue;
            }
        }
        else    //it should only be a lowercase letter or an operator - no other characters allowed
        {
            return false;
        }
    }
    return (numLetters - numOperators == 1);    //should always be one more letter than operator
}

void removeSpaces(string& s)    //removes the spaces from a string
{
    string result;
    for (int k = 0; k < s.size(); k++)
    {
        if (s[k] != ' ')
        {
            result += s[k];
        }
    }
    s = result;
}

bool checkAndExtractParentheses(string &s)  //checks if the string validly uses parentheses and then removes them
{
    //(the spaces have already been removed at this point)
    bool mustBeOpenParenthesis = true;
    int numConsecutiveOpenParentheses = 0;
    int numOpen = 0;
    int numClose = 0;
    
    for (int k = 0; k < s.size(); k++)
    {
        if (s[k] == '(')    //open parenthesis
        {
            numConsecutiveOpenParentheses++;
            mustBeOpenParenthesis = false;
            numOpen++;
            
            //a '(' cannot be preceded by a letter
            if (k != 0)     //if it's not the first element
            {
                if (islower(s[k - 1]))
                {
                    return false;
                }
            }
            
            //cannot have empty parentheses - ()
            if (k != s.size() - 1)  //if it's not the last character
            {
                if (s[k + 1] == ')')
                {
                    return false;
                }
            }
        }
        
        else if (s[k] == ')')   //close parenthesis
        {
            //if it must be an open parenthesis or there's no open parenthesis before it
            if (mustBeOpenParenthesis || numConsecutiveOpenParentheses == 0)
            {
                return false;
            }
            //if there's one open parenthesis before it
            else if (numConsecutiveOpenParentheses == 1)    //mustBeOpenParenthesis is false
            {
                numConsecutiveOpenParentheses = 0;
                mustBeOpenParenthesis = true;
            }
            //if there's more than one open parenthesis before it
            else if (numConsecutiveOpenParentheses > 1)     //mustBeOpenParenthesis is false
            {
                numConsecutiveOpenParentheses--;
            }
            
            //a ')' cannot be preceded by an operator
            if (k != 0)     //if it's not the first character
            {
                if (s[k - 1] == '+' || s[k - 1] == '-' || s[k - 1] == '*' || s[k - 1] == '/')
                {
                    return false;
                }
            }
            
            // a ')' cannot have a letter follow it
            if (k != s.size() - 1)  //if it's not the last character
            {
                if (islower(s[k + 1]))
                {
                    return false;
                }
            }
                
            numClose++;
        }
    }
    
    if (numOpen != numClose)    //the number of ')' and '(' must match
    {
        return false;
    }
    else    //extract parentheses
    {
        string copy;
        for (int k = 0; k < s.size(); k++)
        {
            if (s[k] != '(' && s[k] != ')')
            {
                copy += s[k];
            }
        }
        s = copy;
        return true;
    }
}

//precedence of ch <= precedence of stack top
bool checkPrecedence(char ch, char other)
{
    if (ch == '+' || ch == '-')
    {
        return true;
    }
    else if (other == '*' || other == '/')
    {
        return true;
    }
    else
    {
        return false;
    }
}
