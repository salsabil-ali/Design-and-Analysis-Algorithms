# Task 8 - Colored Tromino Tiling

Given a 2^n x 2^n board (n > 1) with one missing square, tile every remaining
square with non-overlapping right trominoes (L-shaped tiles of three squares).
Only three colors are used, and no two trominoes sharing an edge have the same
color. The program outputs the positions and colors of the placed trominoes.

**Required technique:** Divide and Conquer
**Language:** C++17
**Function:** `vector<Tromino> tileTromino(int n, int missingRow, int missingCol)`

## Files

| File | Description |
|------|-------------|
| `tileTromino.cpp` | Full solution: tiling, coloring, verifier, self test |
| `README.md` | This file |

## Data structure

```cpp
struct Tromino {
    array<pair<int,int>, 3> cells; // three {row, col} cells forming an L
    int color;                     // 0 = Blue, 1 = Yellow, 2 = Green
};
```

Rows and columns are numbered from 0, with (0,0) at the top-left corner.

## Algorithm

### 1. Tiling (Divide and Conquer)
1. Split the board into four equal quadrants. One of them contains the missing square.
2. Place one L-tromino at the center of the board, covering one square in each
   of the three quadrants that do **not** contain the missing square.
3. Each quadrant now has exactly one "missing" square (the original one, or the
   one just covered by the center tromino). Recurse on all four quadrants.
4. Base case: a 1x1 board is a single missing square, so nothing is placed.

A 2^n x 2^n board has 4^n - 1 squares, which is divisible by 3, so the number
of trominoes is (4^n - 1) / 3.

### 2. Coloring (3 colors)
1. Build a graph where each tromino is a node and two nodes are connected if
   their tiles share an edge.
2. 3-color the graph using DSATUR ordering (most constrained tile first) with
   backtracking.

The tiling step is a straightforward divide and conquer. The coloring step is a
search and not a closed-form construction. It worked on every board tested
(n = 1 to 8), but there is no proof here that it can never backtrack for long.

## Input

One line with three integers:

```
n missingRow missingCol
```

- `n >= 1`, and the board is 2^n x 2^n
- `0 <= missingRow, missingCol < 2^n`

## Output

- The number of trominoes
- The list of trominoes with their cells and colors (printed when n <= 4)
- The colored board (printed when n <= 5): `B` = Blue, `Y` = Yellow, `G` = Green, `X` = missing square
- `Valid: YES` or `Valid: NO`, from a built-in checker that verifies full
  coverage, no overlap, L-shaped tiles, and that adjacent tiles differ in color

## Examples

### Example 1: input `2 0 0`

```
Number of trominoes: 5
Tromino 1: (1,2) (2,1) (2,2) - Blue
Tromino 2: (0,1) (1,0) (1,1) - Yellow
Tromino 3: (0,2) (0,3) (1,3) - Green
Tromino 4: (2,0) (3,0) (3,1) - Green
Tromino 5: (2,3) (3,2) (3,3) - Yellow

Board (B=Blue, Y=Yellow, G=Green, X=missing):
X Y G G
Y Y B G
G B B Y
G G Y Y
Valid: YES
```

### Example 2: input `3 5 2`

```
B B G G B B Y Y
B Y Y G B G G Y
G Y B B Y Y G B
G G B G G Y B B
B B Y Y G B G G
B Y X Y B B Y G
G Y Y B G Y Y B
G G B B G G B B
Valid: YES
```

(The program also lists all 21 trominoes before the board.)

## Testing

Run the built-in self test:

```
tromino --test
```

It tries many missing-square positions for n = 1 to 6 and expects
`276/276 tests passed`.

Other inputs worth trying: `1 1 1` (smallest board), `3 7 7` (corner),
`4 8 8` (near the center), `8 100 200` (256x256 board; only the count and the
validity line are printed).

## How to run

Requires a C++ compiler with C++17 support (g++ from MSYS2/MinGW on Windows,
`g++` on Linux, Xcode command line tools on macOS).

### Windows (PowerShell, e.g. in the VS Code terminal)

1. Open the folder that contains `tileTromino.cpp` (for example with
   **File -> Open Folder** in VS Code, or `cd` into it).
2. Compile:
   ```powershell
   g++ -std=c++17 -O2 tileTromino.cpp -o tromino
   ```
3. Run:
   ```powershell
   .\tromino.exe
   ```
   When prompted, type for example `3 5 2` and press Enter.
4. Run the self test:
   ```powershell
   .\tromino.exe --test
   ```

### Linux / macOS

```bash
g++ -std=c++17 -O2 tileTromino.cpp -o tromino
./tromino
./tromino --test
```
