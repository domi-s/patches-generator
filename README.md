# Patches (LinkedIn Game) generator in C++

By Dominic Satnoianu

See [LinkedIn Patches](https://www.linkedin.com/games/patches/) for details about how the game works; or read the instructions when running the program.

This repository, like others, is entirely written by myself (no AI or autocomplete used).

The generation algorithm takes the initial region of the entire board, and then uses a pseudo-randomised approach in selecting a region from the frontier, splitting it vertically or horizontally (whichever creates more square-like sub-regions, ie. split the larger side) and applies this split. It repeats this process (incrementing the number of regions at each step) until we reach the desired number of sub-regions.

It then displays the grid with its solution.

Note that utilities are slightly bodged, and could be improved. But the core functionality is logical and runs efficiently (for our intents and purposes). Some improvements:

- Add map for wall character display (instead of the function that checks every combination)
- Add input method options
- Use a set/map or a linked-list for logarithmic or sublogarithmic removals (don't remove regions from the frontier, but rather their references)
