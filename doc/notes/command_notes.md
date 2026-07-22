For implementation we are considering a JS wrapper of a c++ program, where the JS program would use command line arguments to inform the c++ executable and use the output data. 
Currently there are two planned modes for the start of a program:
1. Import mode: we will need to use the `/imports/` module to grab games from either the chess.com or lichess.org api
    a. username
    b. website
    c. starting date for importing games
    d. ending date for importing games
2. File mode: we will skip the `/imports/` module and go straight to the analysis module using a given .pgn file
    a. username
    b. .pgn file path
For both modes, there should be the option to either analyze one specific player (or username) or analyze both sides for each game -- for the purpose of creating benchmarks.
> The first command line argument will dictate which import mode we are using, and either give the path for the .pgn file, or the website, username, and other data for importing games.
> Second argument will be the username regardless of mode, with 'n/a' used for analyzing both sides of given games, since that cannot be a username on chess.com or lichess.org. 
    We can only have no username listed for mode 2. Potential ability to add a modifier to import mode to import games from a given link, but this will likely need to be a feature to look into after the complention of our MVP.