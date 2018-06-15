//
//  main.cpp
//  getFileNames
//
//  Created by Mayra Ochoa on 6/14/18.
//  Copyright © 2018 Mayra Ochoa. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sys/types.h>
#include <dirent.h>     // This library represents a directory stream or folder.
#include <algorithm>

using namespace std;

// This function have access to a directory and store the file names "v" passed as argument.
void read_directory(vector<string>& v);

// Sorts Files
//  - Comparison Operator for sort()
bool leastToGreat(const string& a, const string& b);

int main(){
    vector<string>filenames;
    read_directory(filenames);
    
    sort(filenames.begin(), filenames.end(), leastToGreat);
    
    //Display all the file names
    for(int i=0; i<filenames.size(); i++){
        cout << filenames[i] << endl;
    }
    
    return EXIT_SUCCESS;
}

//This function have access to a directory and store the file names "v" passed as argument.
void read_directory(vector<string>& v)
{
    DIR* dirp = opendir("Annotations");
    struct dirent * dp;
    // readdir() removes files from a directory or adds to a directory.
    while ((dp = readdir(dirp)) != NULL) {
        if(isalpha(dp->d_name[0]))
            v.push_back(dp->d_name); //d_name obtains the name of the files while the dirent pointer itirate into the files.
    }
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
