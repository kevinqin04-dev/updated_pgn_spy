Current Schematic as of 7/16/26

Input data to module 1 (imports) for a standard evaluation would be:
- Player username
- applicable website (chess.com, lichess.org)
- Number of games to include / starting / ending dates to check games (could be changed, TBD)
Input data to module 2 (analysis) would be:
- Player to analyze (if NULL, assume we analyze both sides for benchmark purposes)
- either .pgn files or an ndjson stream / api response, depending on confirmed implementation
> Possible to import pgn files directly for benchmarks, or choosing to "run benchmarks," bypassing module 1 for this purpose -- potentially faster for those already used to running PGN-Spy
> - Move-times would be a critical part of this -- confirming if they are included in inputs is necessary, if not then we would omit some of the report, *not* throw an error
Input data to module 3 (reporting) is currently unknown, more research is needed for presentation and as to what factors we can really check. Suggested that we spend some time doing research on statistics in general & how cheaters are normally caught using them (i.e that 3blue1brown video, it has sources I am sure)
> Module 3 is mostly unfinished / vague right now, it will depend on research. Possibly combine that with a wrapper / application, I am currently working on finding sources for c++ application design, will update eventually