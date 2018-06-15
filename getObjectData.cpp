//
//  main.cpp
//  getAnnotations
//
//  Created by Raymond Esteybar on 6/14/18.
//  Copyright © 2018 Raymond Esteybar. All rights reserved.
//

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

int main() {
    ifstream readFile;
    vector<Creature> creaturesFound;

    string path = "D008_03HD_00-00-45.xml";

    cout << "Reading in xml.. \n\n";

    readFile.open(path);

    string line;
    bool foundObject = false;
    Creature temp;

    while(!readFile.eof()) {
        readFile >> line;

        // When object is found, store value in our temporary 'Creature'
        if(foundObject) {
            storeValue(temp, line);
        }

        // If object found in frame
        if(line == "<object>") {
            foundObject = true;
        } else if (line == "</object>") {
            foundObject = false;
        }
    }

    creaturesFound.push_back(temp);

    cout << "Name: " << temp.name << endl
    << "xmin: " << temp.dim.xmin << endl
    << "ymin: " << temp.dim.ymin << endl
    << "xmax: " << temp.dim.xmax << endl
    << "ymax: " << temp.dim.ymax << endl;

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
