#include <iostream>
#include "ExpandableHashMap.h"

using namespace std;


int main()
{
    ExpandableHashMap<string, double> nameToGPA(0.2);
    // Add new items to the hashmap. Inserting the third item will cause
   // the hashmap to increase the number of buckets (since the maximum
   // load factor is 0.2), forcing a rehash of all items.
    nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
    nameToGPA.associate("David", 3.99); // David beat Carey
    nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA

    double* davidsGPA = nameToGPA.find("David");
    if (davidsGPA != nullptr)
        *davidsGPA = 1.5; // after a re-grade of David�s exam
    nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0
    // replaces old 3.5 GPA
    double* lindasGPA = nameToGPA.find("Linda");
    if (lindasGPA == nullptr)
        cout << "Linda is not in the roster!" << endl;
    else
        cout << "Linda�s GPA is: " << *lindasGPA << endl;
}