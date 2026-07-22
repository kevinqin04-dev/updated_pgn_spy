#include <stdio.h>
#include <iostream>
#include <ctime>
#include "imports/imports.h"
#include "analysis/analysis.h"
#include "reports/reports.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc < 3) {
        cerr << "Not enough arguments, please provide a mode and username." << endl;
        return -1;
    }

    string mode = argv[1];
    string username = argv[2];

    if(mode == "import") {
        if(argc < 5) {
            cerr << "Not enough arguments for 'file' mode. Please provide startDate and endDate." << endl;
            return -1;
        }
        string website = argv[3];
        string startDate = argv[4];
        string endDate = argv[5];
        imports::importGames(username, website, startDate, endDate);
    } else if(mode == "file") {
        if(argc < 4) {
            cerr << "Not enough arguments for 'file' mode. Please provide a file path." << endl;
            return -1;
        }
        string filePath = argv[3];
        // analysis::analyze_games(username, filePath);
    } else {
        cerr << "Invalid mode. Please use 'import' or 'file'." << endl;
    }
    
    return 0;
}