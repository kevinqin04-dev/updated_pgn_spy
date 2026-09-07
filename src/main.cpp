#include <stdio.h>
#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include <fstream>
#include <algorithm>
#include "imports/imports.h"
#include "analysis/analysis.h"
#include "analysis/benchmark.h"
#include "reports/reports.h"
#include <xgboost/c_api.h>
#include <numeric>
using namespace std;
vector<double> getdata(vector<int> times){
    if(times.empty()){
        return {0.0, 0.0, 0.0, 0.0, 0.0}; 
    }
    double sum = accumulate(times.begin(), times.end(), 0.0);
    double mean = sum/times.size(); 
    double stdev = 0.0;
    for(int i : times){
        stdev += (i-mean) * (i-mean); 
    }
    stdev = sqrt(stdev/times.size()); 
    sort(times.begin(), times.end()); 
    double median = times[times.size()/2]; 
    double min1 = times.front();
    double max1 = times.back(); 
    return {mean, stdev, median, min1, max1}; 
}
double percentile(vector<double> v1, double val) {
    
    auto it = lower_bound(v1.begin(), v1.end(), val);
    int count = distance(v1.begin(), it);
    
    return ((double)count / v1.size()) * 100.0;
}
int main(int argc, char *argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    //testing
    //imports::importGames("meth", "chess.com", "2026/07", "2026/08"); 
    // the dates are on the 1st of the month. So for this one it would be July 1st-August 1st 
    auto res0 = analysis::analyzeGame("../pgnfiles/doppelgangsterr.pgn", 1, "MVChessAI"); 
    vector<vector<int>> userevals = res0.second;
    vector<vector<int>> movetimes = res0.first; 
    std::ofstream outFile("../cheat_movetimes.txt"); 
    for (const auto& game : movetimes) {
        for (int t : game) {
            outFile << t << " ";
        }
        outFile << "\n";
    }
    outFile.close();
    vector<double> Ufirstmoves; 
    vector<double> Usecondmoves; 
    vector<double> Uthirdmoves; 
    vector<double> Unonemoves;
    for(vector<int> p : userevals){
        double total = p[0]+p[1]+p[2]+p[3]; 
        if(total == 0){
            continue; 
        }
        Ufirstmoves.push_back((round(p[0] * 1000.0) / 1000.0f)/total);
        Usecondmoves.push_back((round(p[1] * 1000.0f) / 1000.0f)/total);
        Uthirdmoves.push_back((round(p[2] * 1000.0f) / 1000.0f)/total);
        Unonemoves.push_back((round(p[3] * 1000.0f) / 1000.0f)/total);
         
    }
    vector<double> finalUserResult(4); 
    for(double x : Ufirstmoves){
        finalUserResult[0] += x; 

    }
    finalUserResult[0] /= Ufirstmoves.size();
    for(double x : Usecondmoves){
        finalUserResult[1] += x; 

    }
    finalUserResult[1] /= Usecondmoves.size(); 
    for(double x : Uthirdmoves){
        finalUserResult[2] += x; 

    }
    finalUserResult[2] /= Uthirdmoves.size(); 
    for(double x : Unonemoves){
        finalUserResult[3] += x; 

    }
    finalUserResult[3] /= Unonemoves.size();  


    vector<vector<vector<int>>> res1 = benchmark::analyzeBenchmark("../pgnfiles/benchmarks.pgn", 1, true);
    vector<double> firstmoves; 
    vector<double> secondmoves; 
    vector<double> thirdmoves; 
    vector<double> nonemoves;  
    std::ofstream f1("../clean_movetimes.txt"); 
    for (const auto& game : res1[1]) {
        for (int t : game) {
            f1 << t << " ";
        }
        f1 << "\n";
    }
    for (const auto& game : res1[2]) {
        for (int t : game) {
            f1 << t << " ";
        }
        f1 << "\n";
    }
    f1.close(); 
    for(vector<int> p : res1[0]){
        double total = p[0]+p[1]+p[2]+p[3]; 
        if(total == 0){
            continue; 
        }
        firstmoves.push_back((round(p[0] * 1000.0) / 1000.0f)/total);
        secondmoves.push_back((round(p[1] * 1000.0f) / 1000.0f)/total);
        thirdmoves.push_back((round(p[2] * 1000.0f) / 1000.0f)/total);
        nonemoves.push_back((round(p[3] * 1000.0f) / 1000.0f)/total);
         
    }
    vector<vector<vector<int>>> res2 = benchmark::analyzeBenchmark("../pgnfiles/benchmarks.pgn", 1, false);
    for(vector<int> p : res2[0]){
        double total = p[0]+p[1]+p[2]+p[3]; 
        if(total == 0){
            continue; 
        }
        firstmoves.push_back((round(p[0] * 1000.0) / 1000.0f)/total);
        secondmoves.push_back((round(p[1] * 1000.0f) / 1000.0f)/total);
        thirdmoves.push_back((round(p[2] * 1000.0f) / 1000.0f)/total);
        nonemoves.push_back((round(p[3] * 1000.0f) / 1000.0f)/total);
         
    }
    sort(firstmoves.begin(), firstmoves.end()); 
    sort(secondmoves.begin(), secondmoves.end()); 
    sort(thirdmoves.begin(), thirdmoves.end());
    sort(nonemoves.begin(), nonemoves.end()); 
    double p1 = percentile(firstmoves, finalUserResult[0]);
    double p2 = percentile(secondmoves, finalUserResult[1]);
    double p3 = percentile(thirdmoves, finalUserResult[2]);
    double pNone = percentile(nonemoves, finalUserResult[3]);
    for(double i: finalUserResult){
        cout << i << endl; 
    }
    cout << "\n" << p1 << " " << p2 << " " << p3 << " " << pNone << endl; 
    BoosterHandle booster; 
    XGBoosterCreate(NULL, 0, &booster); 
    XGBoosterLoadModel(booster, "../model.json"); 

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
    return 0; 
    //testing
    
    
    
    
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