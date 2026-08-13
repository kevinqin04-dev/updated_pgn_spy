#include <stdio.h>
#include <iostream>
#include <string>
#include "imports/imports.h"
#include "analysis/analysis.h"
#include "reports/reports.h"

using namespace std;

int main(int argc, char *argv[]) {
    /*
    //testing
    imports::importGames("SindarovGM", "lichess", "2026/07", "2026/08"); 
    // the dates are on the 1st of the month. So for this one it would be July 1st-August 1st 
    vector<vector<int>> res= analysis::analyzeGame("../pgnfiles/game.pgn", 5); 
    for(vector<int> s: res){
        for(int i = 0; i < s.size(); i++){
            cout << s[i]  << " "; 
        }
        cout << endl; 
    }
    return 0; 
    //testing
    */
    
    
    
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
        if(username == "N/A") {
            cerr << "Username is required for 'import' mode." << endl;
            return -1;
        }
        imports::importGames(username, website, startDate, endDate);
    } else if(mode == "file") {
        if(argc < 4) {
            cerr << "Not enough arguments for 'file' mode. Please provide a file path." << endl;
            return -1;
        }
        string filePath = argv[3];
        // analysis::analyze_games(username, filePath); //to be completed
    } else {
        cerr << "Invalid mode. Please use 'import' or 'file'." << endl;
    }
    
    return 0;
}