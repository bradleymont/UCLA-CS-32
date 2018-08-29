//
//  tree.cpp
//  Tree
//
//  Created by Bradley Mont on 2/10/18.
//  Copyright © 2018 Bradley Mont. All rights reserved.
//

//#include <iostream>
//using namespace std;

// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then for this value of a2     the function must return
//    10 20 40            1
//    10 40 30            2
//    20 10 40            0
//    50 40 30            3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
    if (n2 <= 0)
    {
        return 1;
    }
    if (n1 <= 0)
    {
        return 0;
    }
    
    int result = 0;
    
    if (a1[0] == a2[0])
    {
        if (n2 > 1)
        {
            result += countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1) + countIncludes(a1 + 1, n1 - 1, a2, n2);
        }
        else
        {
            result += countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
        }
    }
    else   //not =
    {
        result += countIncludes(a1 + 1, n1 - 1, a2, n2);
    }
    
    return result;
}

// Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > splitter come before all the other elements,
// and all the elements whose value is < splitter come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= splitter, or n if there is no such element, and firstLess is
// set to the index of the first element that is < splitter, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > splitter
//   * for firstNotGreater <= i < firstLess, a[i] == splitter
//   * for firstLess <= i < n, a[i] < splitter
// All the elements > splitter end up in no particular order.
// All the elements < splitter end up in no particular order.
void split(double a[], int n, double splitter,
           int& firstNotGreater, int& firstLess)
{
    if (n < 0)
        n = 0;
    
    // It will always be the case that just before evaluating the loop
    // condition:
    //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
    //  Every element earlier than position firstNotGreater is > splitter
    //  Every element from position firstNotGreater to firstUnknown-1 is
    //    == splitter
    //  Every element from firstUnknown to firstLess-1 is not known yet
    //  Every element at position firstLess or later is < splitter
    
    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
        if (a[firstUnknown] < splitter)
        {
            firstLess--;
            exchange(a[firstUnknown], a[firstLess]);
        }
        else
        {
            if (a[firstUnknown] > splitter)
            {
                exchange(a[firstNotGreater], a[firstUnknown]);
                firstNotGreater++;
            }
            firstUnknown++;
        }
    }
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
    if (n <= 1)
    {
        return;
    }
    if (n == 2)
    {
        if (a[0] < a[1])
            exchange(a[0], a[1]);
        return;
    }
                     
    int firstNotGreater = 0, firstLess = 0;
                     
    split(a, n, a[0], firstNotGreater, firstLess);
    
    order(a, firstNotGreater);
    
    order(a + firstLess, n - firstLess);
}

//int main()
//{
//    double arr1[7] = {10, 50, 40, 20, 50, 40, 30};
//    double arr2[3] = {50, 40, 30};
//    double arr3[3] = {20, 10, 40};
//    double arr4[3] = {10, 40, 30};
//    double arr5[8] = {90, 60,70, 30, 60, 60, 70, 30};
//    double arr6[1] = {10};
//    double arr7[1] = {10};
//    cout << countIncludes(arr1, 7, arr2, 3) << endl;
//    cout << countIncludes(arr1, 7, arr3, 3) << endl;
//    cout << countIncludes(arr1, 7, arr4, 3) << endl;
//    cout << countIncludes(arr1, 7, arr5, 8) << endl;
//    cout << countIncludes(arr6, 1, arr7, 1) << endl;
//
//    order(arr1, 7);
//    order(arr2, 3);
//    order(arr3, 3);
//    order(arr4, 3);
//    order(arr5, 8);
//    order(arr6, 1);
//    order(arr7, 1);
//
//    for (int k = 0; k < 7; k++)
//        cout << arr1[k] << " ";
//    cout << endl;
//    for (int k = 0; k < 3; k++)
//        cout << arr2[k] << " ";
//cout << endl;
//    for (int k = 0; k < 3; k++)
//        cout << arr3[k] << " ";
//cout << endl;
//    for (int k = 0; k < 3; k++)
//        cout << arr4[k] << " ";
//cout << endl;
//    for (int k = 0; k < 8; k++)
//        cout << arr5[k] << " ";
//cout << endl;
//    for (int k = 0; k < 1; k++)
//        cout << arr6[k] << " ";
//cout << endl;
//    for (int k = 0; k < 1; k++)
//        cout << arr7[k] << " ";
//cout << endl;
//}

