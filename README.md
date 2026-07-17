This project is an effort to update the [PGN-Spy](https://github.com/MGleason1/PGN-Spy) cheat detection software made by @MGleason1 on github. The last release was in 2017, and since then chess cheating has only grown more high-profile. Our current goal is to include the following improvements upon PGN-Spy:

1. Add the ability to import a certain # of games (or between some dates) from a certain website, based on a given username.
2. In addition to analysis of raw game accuracy (T1/T2/T3 measures), include analysis of move times if possible.
3. Automatically store benchmarks and have an option to "run benchmarks on local hardware" for comparison.
4. Create a report that points out discrepancies compared to either people at a much higher level (from benchmarks) or people at a similar level (rating-wise, on the same platform -- maia3 as a potential basis for this) rather than only stating percentages.

> The plan is to include every feature that PGN-Spy currently has, so anyone who has used it in the past and wants only *some* of these improvements will be able to take advantage of whatever they please without losing anything in return. Our minimum-viable-product should be a direct upgrade.

This project will remain under the MIT License that the original project used, and we will attempt to work on a way to 'build from source' as well, though the original version does not offer that option.

## Disclaimer: This is not a fork of PGN-Spy. All code is human-written and new.