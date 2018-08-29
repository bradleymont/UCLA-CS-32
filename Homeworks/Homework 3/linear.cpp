//
//  linear.cpp
//  Linear
//
//  Created by Bradley Mont on 2/10/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

//#include <iostream>
//using namespace std;

//bool somePredicate(double x)
//{
//    return x > 0;
//}

// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n)
{
    if (n <= 0) return true;
    
    return somePredicate(a[0]) && allTrue(a + 1, n - 1);
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
    if (n <= 0) return 0;
    
    int count = 0;
    
    if (!somePredicate(a[0]))
    {
        count++;
    }
    
    return count + countFalse(a + 1, n - 1);
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
    if (n <= 0) return -1;

    int lowestIndex = -1;
    
    if (!somePredicate(a[n - 1]))
    {
        lowestIndex = n - 1;
    }
    
    //if there also exists more false indexes, then return that index, since it will be smaller than the current index
    if (firstFalse(a, n - 1) >= 0)
    {
        return firstFalse(a, n - 1);
    }
    //else if a false index was just found, and there exists no other false indexes, then return the current index
    else if (lowestIndex >= 0)
    {
        return lowestIndex;
    }
    //else if a false index wasn't found, then check the rest of the array
    else
    {
//        return firstFalse(a, n - 1);
        return -1;
    }
}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
    if (n <= 0) return -1;
    if (n == 1) return 0;
    
    int otherIndex = indexOfMin(a , n - 1);
    
    int result = n - 1;
    
    if (a[otherIndex] <= a[n - 1])
        result = otherIndex;
    
    return result;
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0) return true;
    if (n1 <= 0) return false;
    
    if (a1[0] == a2[0])
    {
        return includes(a1 + 1, n1 -1, a2 + 1, n2 - 1);
    }
    else   //not =
    {
        return includes(a1 + 1, n1 -1, a2, n2);
    }
}

// the predicate tests if a # is positive
//false = negative number

//int main()
//{
//    double a1[4] = {10, 20, 30, 40};
//
//    double a2[8] = {1, 2, 3, -4, 5, -6, 7, -8};
//
//    if (allTrue(a1, 4))
//        cout << "true" << endl;
//    else
//        cout << "false"<< endl;
//
//    if (allTrue(a2, 8))
//        cout << "true" << endl;
//    else
//        cout << "false"<< endl;
//
//    cout << countFalse(a1, 4) << endl;
//
//    cout << countFalse(a2, 8) << endl;
//
//    cout << firstFalse(a1, 4) << endl;
//
//    cout << firstFalse(a2, 8) << endl;
//
//    cout << indexOfMin(a1, 4) << endl;
//
//    cout << indexOfMin(a2, 8) << endl;
//
//
//    double a3[0] = {};
//    double a4[4] = {1, 2, 3, 4};
//
//    if (includes(a3, 0, a4, 4))
//        cout << "yes" << endl;
//    else
//        cout << "no" << endl;
//
//    if (includes(a4, 4, a3, 0))
//        cout << "yes" << endl;
//    else
//        cout << "no" << endl;
//
//    double a5[5] = {4, 6, 8, 10, 12};
//    double a6[2] = {4, 12};
//    if (includes(a5, 5, a6, 2))
//        cout << "yes" << endl;
//    else
//        cout << "no" << endl;
//
//    double a7[5] = {4, 6, 8, 10, 12};
//    double a8[2] = {4, 132};
//    if (includes(a7, 5, a8, 2))
//        cout << "yes" << endl;
//    else
//        cout << "no" << endl;
//}
