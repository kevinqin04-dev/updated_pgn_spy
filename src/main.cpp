#include <stdio.h>
#include <iostream>
#include <ctime>
#include "imports/imports.h"
#include "reports/reports.h"
#include "analysis/analysis.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc < 3) {
        cerr << "Not enough arguments, please provide a mode and username." << endl;
        return -1;
    }

    string mode = argv[1];
    string username = argv[2];

    if(mode == "import") {
        // imports::importData(username);
    } else if(mode == "file") {
        if(argc < 5) {
            cerr << "Not enough arguments for 'file' mode. Please provide startDate and endDate." << endl;
            return -1;
        }
        string startDate = argv[3];
        string endDate = argv[4];
        // analysis::analyze_games(username, startDate, endDate);
    } else {
        cerr << "Invalid mode. Please use 'import' or 'file'." << endl;
    }
    
    return 0;
}