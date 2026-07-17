Current Methodology as of 7/16/26

Chess.com Game Imports:
1. Check the live game archive (potentially laggy -- run as async?) does allow to sort by format, maybe try this, if we get a code 0, go to option 2
> https:// api.chess.com/pub/player/{username}/games/live/{base time (seconds)}/{increment (seconds)}
2. Find potential monthly archives to grab at 
> https://api.chess.com/pub/player/{username}/games/archives
3. For each archive grab the ndjson 
> https://api.chess.com/pub/player/{username}/games/{YYYY}/{MM}
or .pgn files 
> https://api.chess.com/pub/player/{username}/games/{YYYY}/{MM}/pgn
4. End result should be either .pgn files or an ndjson that includes the pgns we want, undecided which one currently, depends on the person parsing games, lichess and chess.com both support .pgn and ndjson formats

Lichess Game Imports
1. https://lichess.org/api#tag/games/GET/api/games/user/{username} seems like the only usable way for getting games on lichess, but it is recommended to "stream" games, which seems like an interesting idea (more research needed for "streaming njdson responses" or similar topics)
> for testing, having oauth set up + signing in and pulling our own games, will allow us to go 3x as fast as running as an anonymous program -- very much an interesting idea, but not a core component of this module

Chess.com Join Date
https://api.chess.com/pub/player/{username} for grabbing the join date of a player, no need to calculate monthly archives here, simply grabbing the `"joined":` field from the ndjson response should be sufficient. This is an epoch/unix time number, aka the number of seconds since some specific date in the 1960s that I do not care to remember. 
> - https://www.epochconverter.com/ human-readable converter
> - https://www.epochconverter.com/programming/c guide to creating an epoch-time to human-readable datetime in c, likely adapatable to c++ but depends on a specific library -- more research would be useful before implementation

Lichess Join Date
https://lichess.org/api#tag/users/GET/api/user/{username}
> Would need to parse the json to get the createdAt field and then convert from epoch time.