//
//  readAnnotations.cpp
//  readAnnotations
//
//  Created by Mayra Ochoa & Raymond Esteybar on 6/14/18.
//  Copyright © 2018 Mayra Ochoa & Raymond Esteybar. All rights reserved.
//

#include <dirent.h>     // This library represents a directory stream or folder.
#include <sys/types.h>

#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
using namespace std;

// Object Values
struct BoundingBox {
    int xmin;
    int ymin;
    int xmax;
    int ymax;
};

struct Creature {
    string name;
    BoundingBox dim;
};

// Grabbing XML Value
//  - Helper Function
int getTagSize(const string& xmlTag);                   // Counts the 'xml' tag size to help w/ grabbing the value

//  - Main Functions
string getValueInTag(const string& xmlTag);             // Grabs the value in: <xml> Value </xml>
void storeValue(Creature& temp, const string& line);    // Stores value in temporary 'Creature' Object

// Grab all File Names
//  - This function have access to a directory and store the file names "v" passed as argument.
void read_directory(vector<string>& v);

// Sorts Files
//  - Comparison Operator for sort()
bool leastToGreat(const string& a, const string& b);

int main() {
    ifstream readFile;
    vector<Creature> creaturesFound;
    
    // 1) Store File Names
    vector<string> filenames;
    read_directory(filenames);
    
    // 2) Read File Names
    //  - Store
    for(const auto& file : filenames) {
        readFile.open(file);
        
        string line;
        bool foundObject = false;
        Creature temp;
        
        // Locate Object
        while(!readFile.eof()) {
            readFile >> line;
            
            //  - When object is found, store value in our temporary 'Creature'
            if(foundObject) {
                storeValue(temp, line);
            }
            
            //  - If object is found in frame
            if(line == "<object>") {
                foundObject = true;
            } else if (line == "</object>") {
                foundObject = false;
                
                //  - Store 'Creature' object in list (CAN CHANGE DATA STRUCTURE)
                creaturesFound.push_back(temp);
            }
        }
        
        readFile.close();
    }
    
    // 3) Check Objects' Values
    //    char choice = 'q';
    //
    //    do {
    //
    //
    //        cout << "Would you like to go again?\n";
    //        cin >> choice;
    //
    //    } while(tolower(choice) != 'q');
    
    return EXIT_SUCCESS;
}

// Grabbing XML Value
//  - Helper Function
//      - Counts the 'xml' tag size to help w/ grabbing the value
int getTagSize(const string& xmlTag) {
    int tagSize = 0;
    
    for(const auto& c : xmlTag) {
        ++tagSize;
        if(c == '>')
        break;
    }
    
    return tagSize;
}

//  - Main Functions
//      - Grabs the value in: <xml> Value </xml>
string getValueInTag(const string& xmlTag) {
    int tagSize = getTagSize(xmlTag);
    
    return xmlTag.substr(tagSize, xmlTag.length() - (2 * tagSize) - 1);
}

//      - Stores value in temporary 'Creature' Object
void storeValue(Creature& temp, const string& line) {
    if(line.substr(1, 4) == "name") {
        temp.name = getValueInTag(line);
    } else if(line.substr(1, 4) == "xmin") {
        temp.dim.xmin = stoi(getValueInTag(line));
    } else if(line.substr(1, 4) == "ymin") {
        temp.dim.ymin = stoi(getValueInTag(line));
    } else if(line.substr(1, 4) == "xmax") {
        temp.dim.xmax = stoi(getValueInTag(line));
    } else if(line.substr(1, 4) == "ymax") {
        temp.dim.ymax = stoi(getValueInTag(line));
    }
}

// Grab all File Names
//  - This function have access to a directory and store the file names "v" passed as argument.
void read_directory(vector<string>& v)
{
    DIR* dirp = opendir("Annotations");
    struct dirent * dp;
    // readdir() removes files from a directory or adds to a directory.
    while ((dp = readdir(dirp)) != NULL) {
        if(isalpha(dp->d_name[0]))
        v.push_back(dp->d_name); //d_name obtains the name of the files while the dirent pointer itirate into the files.
    }
    
    // Sort file names based on: Time Captured
    sort(v.begin(), v.end(), leastToGreat);
    
    closedir(dirp);
}

// Sort Files
//  - Comparison Operator for sort()
bool leastToGreat(const string& a, const string& b) {
    int aHour = stoi(a.substr(a.find("D_") + 2, 2)),
    aMin = stoi(a.substr(a.find("D_") + 5, 2)),
    aSec = stoi(a.substr(a.find("D_") + 8, 2));
    
    int bHour = stoi(b.substr(b.find("D_") + 2, 2)),
    bMin = stoi(b.substr(b.find("D_") + 5, 2)),
    bSec = stoi(b.substr(b.find("D_") + 8, 2));
    
    if(aHour == bHour)              // When hours are the same, scale by minutes
    if(aMin == bMin)
    return aSec < bSec;     // When min are the same, scale by seconds
    
    return aMin < bMin;             // Otherwise, scale by min
}
