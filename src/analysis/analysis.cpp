#include <vector>
#include <string>
#include <iostream>
#include <boost/process.hpp>
#include "analysis.h"
#include <fstream>

#include "chess.hpp"
namespace bp = boost::process; 
using namespace std;
using namespace chess; 
bp::opstream in; 
bp::ipstream out; 
class MyVisitor : public pgn::Visitor {
public:
    chess::Board board;
    vector<string> moves = {};
    vector<vector<string>> games; 
    vector<vector<int>> allWhiteMoveTimes = {}; 
    vector<vector<int>> allBlackMoveTimes = {}; 
    vector<int> whiteMoveTimes = {}; 
    vector<int> blackMoveTimes = {}; 

    virtual ~MyVisitor() {}
    
    int totalTime = 0; 
    int increment = 0; 
    int count = 0; 
    int prevWhite;
    int prevBlack; 

    void startPgn() override {
        board = chess::Board();
        count = 0; 
        
        moves.clear(); 

    }

    void header(std::string_view key, std::string_view value) {
        if(key == "TimeControl"){
            int pos = value.find('+');

            totalTime = stoi(static_cast<string>(value.substr(0, pos)));
            increment = stoi(static_cast<string>(value.substr(pos + 1))); 
        }
    }

    void startMoves() {
        // Prepare to process moves
    }

    void move(std::string_view move, std::string_view comment) {
        count++; 
        int pos = comment.find("\%clk ")+5; 
        //HOURS MUST BE BETWEEN 0 AND 9! NO ONE IS PLAYING 10+ HOUR CHESS GAMES!
        int hours = stoi(static_cast<string>(comment.substr(pos, 1))); 
        int minutes = stoi(static_cast<string>(comment.substr(pos+2,2)));
        int seconds = stoi(static_cast<string>(comment.substr(pos+5,2))); 
        seconds = seconds + minutes*60 + hours * 3600; 
        
        if(count % 2 == 1){
            if(whiteMoveTimes.size() == 0){
                whiteMoveTimes.push_back(totalTime-seconds+increment);
                prevWhite = seconds; 

            } else{
                whiteMoveTimes.push_back(prevWhite - seconds + increment); 
                prevWhite = seconds;
            }
            cout << "White took " << to_string(whiteMoveTimes[whiteMoveTimes.size()-1]) << " seconds on this move " << endl;  
        }
        if(count % 2 == 0){
            if(blackMoveTimes.size() == 0){
                blackMoveTimes.push_back(totalTime-seconds+increment);
                prevBlack = seconds; 

            } else{
                blackMoveTimes.push_back(prevBlack - seconds + increment); 
                prevBlack = seconds;
            }
            cout << "Black took " << to_string(blackMoveTimes[blackMoveTimes.size()-1]) << " seconds on this move " << endl;  
        }
        chess::Move m = chess::uci::parseSan(board, move);
        string uciMove = chess::uci::moveToUci(m);
        moves.push_back(uciMove);

        board.makeMove(m);
    }

    void endPgn() {
        // Cleanup code
        games.push_back(moves);
        allBlackMoveTimes.push_back(blackMoveTimes); 
        allWhiteMoveTimes.push_back(whiteMoveTimes);
        blackMoveTimes.clear(); 
        whiteMoveTimes.clear(); 
        cout << "Game parsed." << endl; 
    }
};
namespace analysis {
    

    vector<vector<int>> analyzeGame(const string& filepath, int depth, bool white){
        bp::child engine("/usr/games/stockfish", bp::std_in < in, bp::std_out > out); 
        vector<int> res1(4); 
        vector<vector<int>> finalres; 
        // res[0] is # of T1 moves, res[1] is T2 moves, ... and res[3] is the # of moves that isn't in the top 3
        ifstream pgnstream(filepath); 
        
        if(!pgnstream){
            throw std::runtime_error("No PGN file"); 
        }
        MyVisitor visitor;
        pgn::StreamParser parser(pgnstream); 
        auto error = parser.readGames(visitor); 
        in << "setoption name MultiPV value 3" << endl;
        cout << visitor.totalTime << "TOTAL TIME" << visitor.increment << "INCREMENT" << endl; 
        for(auto& game1: visitor.games){
            string game = "position startpos moves"; 
            vector<string> res = {}; 
        
         
        
            for(int i = 0; i < game1.size(); ++i){
                if(white && i % 2 != 0){
                    game += " "+game1[i];
                    continue; 
                } else if(!white && i % 2 == 0){
                    game += " "+game1[i]; 
                    continue; 
                }
                //we dont analyze the last move because the game will already be over
            
            
            
            
                in << game << endl; 
                in << "go depth "<< to_string(depth) << endl; 
                std::string line;
        
                int count = 0; 
                bool NotTop3 = true; 
                while (getline(out, line)) {
            
                    if (line.substr(0, 11+to_string(depth).size()) == "info depth "+to_string(depth)) {
                        
                        
                        int start = line.find(" pv ")+4;
                        int end = line.find(' ', start);
                        
                        
                        res.push_back(line.substr(start, -start+end)); 
                        count++; 
                        string nextmove = game1[i];
                        string engineMove = res[res.size()-1];
                        if(engineMove == nextmove){
                            res1[count-1]++;
                            NotTop3 = false; 
                        }    
                    
                    
                    }
                    if (line.substr(0, 8) == "bestmove") {
                        if(NotTop3){
                            res1[3]++; 
                        }
                        break;
                    }   
                
            
                }
                game += " "+game1[i];
                // this guy is at the end so we can analyze whether the first move is in the top 1-3 moves of the engine

            }
            
            finalres.push_back(res1);
            for(int i = 0; i < 4; i++){
                res1[i] = 0; 
            }

        }
        in << "quit" << endl; 
        engine.wait(); 
        
        
        
        return finalres; //right now it just converts a pgn file to uci moves
    }
    
}