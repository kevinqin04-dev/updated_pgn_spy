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
    vector<string> moves;
    virtual ~MyVisitor() {}
    
    void startPgn() override {
        
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
    }
};
namespace analysis {
    

    vector<string> analyzeGame(const string& filepath){
        bp::child engine("/usr/games/stockfish", bp::std_in < in, bp::std_out > out); 
        vector<int> res1(4); 
        
        // res[0] is # of T1 moves, res[1] is T2 moves, ... and res[3] is the # of moves that isn't in the top 3
        ifstream pgnstream(filepath); 
        
        if(!pgnstream){
            throw std::runtime_error("No PGN file"); 
        }
        MyVisitor visitor;
        pgn::StreamParser parser(pgnstream); 
        auto error = parser.readGames(visitor); 
        string game = "position startpos moves"; 
        vector<string> res; 
        
        in << "setoption name MultiPV value 3" << endl; 
        for(int i = 0; i < visitor.moves.size()-1; ++i){
            //we dont analyze the last move because the game will already be over
            game += " "+visitor.moves[i];
            in << game << endl; 
            in << "go depth 10" << endl; 
            std::string line;
        
            int count = 0; 
            while (getline(out, line)) {
            
                if (line.substr(0, 13) == "info depth 10") {
                    res.push_back(line.substr(line.find(" pv ")+4, 4)); 
                    count++; 
                    string nextmove = visitor.moves[i+1];
                    string engineMove = res[res.size()-1];
                    if(engineMove == nextmove){
                        res1[count-1]++; 
                    }
                    
                    if(count == 3){
                        break; 
                    }
                }   
                
            
            }
        }
        in << "quit" << endl; 
        engine.wait(); 
        for(int& x: res1){
            res.push_back(to_string(x)); 
        }
        return res; //right now it just converts a pgn file to uci moves
    }
    
}