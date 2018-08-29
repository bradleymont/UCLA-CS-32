//
//  investment.cpp
//  Investment
//
//  Created by Bradley Mont on 2/8/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

//#include <iostream>
//#include <string>
//using namespace std;

//Your declarations and implementations would go here

class Investment
{
public:
    Investment(string name, int price)
    {
        m_name = name;
        m_purchasePrice = price;
    }
    
    string name() const  //non-virtual since no subclass needs to redefine it
    {
        return m_name;
    }
    
    int purchasePrice() const   //non-virtual since no subclass needs to redefine it
    {
        return m_purchasePrice;
    }
    
    virtual bool fungible() const //non-pure virtual because most investments are non-fungible, but subclasses have the option to redefine the function if they are fungible
    {
        return false;
    }
    
    virtual string description() const = 0;  //pure virtual because the base class has no logical implementation of the constructor
    
    virtual ~Investment()   //must have a virtual destructor
    {

    }
    
private:
    string m_name;
    int m_purchasePrice;
};

class Painting: public Investment
{
public:
    Painting(string name, int purchasePrice)
    : Investment(name, purchasePrice)
    {
       
    }
    
    virtual string description() const
    {
        return "painting";
    }
    
    virtual ~Painting()
    {
        cout << "Destroying " << name() << ", a painting" << endl;
    }
};

class Stock: public Investment
{
public:
    Stock(string name, int purchasePrice, string ticker)
    : Investment(name, purchasePrice)
    {
        m_ticker = ticker;
    }
    
    virtual bool fungible() const
    {
        return true;
    }
    
    virtual string description() const
    {
        return "stock trading as " + m_ticker;
    }
    
    virtual ~Stock()
    {
        cout << "Destroying " << name() << ", a stock holding" << endl;
    }
    
private:
    string m_ticker;
};

class House: public Investment
{
public:
    House(string name, int purchasePrice)
    : Investment(name, purchasePrice)
    {
        
    }
    
    virtual string description() const
    {
        return "house";
    }
    
    virtual ~House()
    {
        cout << "Destroying the house " << name() << endl;
    }
};

//void display(const Investment* inv)
//{
//    cout << inv->name();
//    if (inv->fungible())
//        cout << " (fungible)";
//    cout << ": " << inv->description() << " bought for $" << inv->purchasePrice() << endl;
//}

//int main()
//{
//    Investment* portfolio[4];
//    portfolio[0] = new Painting("Salvator Mundi", 450300000);
//    // Stock holdings have a name, value, and ticker symbol
//    portfolio[1] = new Stock("Alphabet", 100000, "GOOGL");
//    portfolio[2] = new Stock("Symantec", 50000, "SYMC");
//    portfolio[3] = new House("4 Privet Drive", 660000);
//
//    for (int k = 0; k < 4; k++)
//        display(portfolio[k]);
//
//    // Clean up the investments before exiting
//    cout << "Cleaning up" << endl;
//    for (int k = 0; k < 4; k++)
//        delete portfolio[k];
//}

