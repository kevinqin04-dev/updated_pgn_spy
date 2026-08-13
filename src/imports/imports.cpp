#include <string>
#include <iostream>
#include <ctime>

#include <cstdlib> 
using namespace std;

long long toMillis(const string& date) {
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));

    tm time{};
    time.tm_year = year - 1900;
    time.tm_mon = month - 1;
    time.tm_mday = 1;

    time_t seconds = timegm(&time);

    return static_cast<long long>(seconds) * 1000;
} 
namespace imports {
    void importGames(const string& username, const string& website, const string& startDate, const string& endDate)
    {
        // Implementation for importing games for the given username from the specified website within the date range.
        cout << "Importing games for user: " << username << " from website: " << website 
             << " between dates: " << startDate << " and " << endDate << endl;
        string command; 
        //gonna assume that startDate is in the format YYYY/MM

        if(website == "chess.com"){
            string date = startDate;
            while(date != endDate){
                
                command = "curl \"https://api.chess.com/pub/player/" +username+"/games/"+date+"/pgn\" >> ../pgnfiles/"+username+".pgn";
                system(command.c_str()); 

                int month = stoi(date.substr(5,2)); 
                int year = stoi(date.substr(0,4)); 
                if(month == 12){
                    month =1; 
                    year++; 

                } else {
                    month++; 
                }
                string mth = to_string(month); 
                if(month < 10){
                    mth = "0"+mth; 
                }
                date = to_string(year) +"/"+ mth; 
            }
            
        } 
        if(website == "lichess"){
            command = "curl -H \"User-Agent: USER AGENT\" -H \"Accept: application/x-chess-pgn\"  \"https://lichess.org/api/games/user/"+ username+"?since="+to_string(toMillis(startDate))+"&until="+to_string(toMillis(endDate))+"&sort=dateAsc\" >> ../pgnfiles/" +username+".pgn"; 
            system(command.c_str()); 
        } 
        
        
            
             
    
    }
    
}
