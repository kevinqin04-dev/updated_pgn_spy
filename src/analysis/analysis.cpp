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
    virtual ~MyVisitor() {}
    
    void startPgn() override {
        board = chess::Board();
         
        moves.clear(); 

    }

    void header(std::string_view key, std::string_view value) {
        // Process header tags
    }

    void startMoves() {
        // Prepare to process moves
    }

    void move(std::string_view move, std::string_view comment) {
        chess::Move m = chess::uci::parseSan(board, move);
        string uciMove = chess::uci::moveToUci(m);
        moves.push_back(uciMove);

        board.makeMove(m);
    }

    void endPgn() {
        // Cleanup code
        games.push_back(moves);
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