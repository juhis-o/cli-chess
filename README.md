# cli-chess
cli-chess is basic chess which is played in command-line-interface. cli-chess supports features: Castling, En passant and Promotion.
Binaries are available at releases.

## Requirements
- C++14+ Compiler
- ncurses and g++ for linux or PDCurses, Microsoft C++ toolset for Windows

### Compiling for linux
1. Install ncurses-dev package from your linux-package manager.
2. ```sh
   git clone https://github.com/juhis-o/cli-chess.git
   cd cli-chess
   ```
3. ```sh
   make
   ```
4. Run
   ```sh
   ./cli-chess
   ```

### Compiling for windows
1. Install PDCurses https://pdcurses.org/ 
2. Open developer tools
   ```sh
   git clone https://github.com/juhis-o/cli-chess.git
   cd cli-chess
   ```
3. ```sh
   nmake -f Makefile.vc
   ```
4. Run
   ```sh
   ./cli-chess
   ```
