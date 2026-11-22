"""
EED461 - Task 2: Domino Tiling (Technique: Brute Force)
tileDominoes(n, missing1, missing2) -> (possible, dominoes)

Brute force = exhaustive backtracking over every way of placing dominoes.
The first empty cell (row-major order) must be covered by a domino that goes
either RIGHT or DOWN (left/up neighbours are already filled), so we try both
and undo (backtrack) if we get stuck.
"""
import sys
sys.setrecursionlimit(10000)


def tileDominoes(n, missing1, missing2, precheck=True):
    # ---- input validation ----
    if not (2 <= n <= 8):
        raise ValueError("n must satisfy 2 <= n <= 8")
    for (r, c) in (missing1, missing2):
        if not (0 <= r < n and 0 <= c < n):
            raise ValueError("missing square outside the board")
    if missing1 == missing2:
        raise ValueError("the two missing squares must be different")

    # ---- optional O(1) feasibility filter (colour counting) ----
    # Every domino covers 1 black + 1 white square, so a tiling needs equal
    # colour counts. If they differ, no tiling exists -> skip the search.
    if precheck:
        black = white = 0
        for r in range(n):
            for c in range(n):
                if (r, c) in (missing1, missing2):
                    continue
                if (r + c) % 2 == 0:
                    black += 1
                else:
                    white += 1
        if black != white:
            return False, []

    # ---- brute-force backtracking ----
    filled = [[False] * n for _ in range(n)]
    filled[missing1[0]][missing1[1]] = True
    filled[missing2[0]][missing2[1]] = True
    placed = []                       # stack of ((r1,c1),(r2,c2))
    total_pairs = (n * n - 2) // 2

    if (n * n - 2) % 2 == 1:          # odd number of squares -> impossible
        return False, []

    def solve(pos):
        # find first empty cell at or after pos (row-major)
        while pos < n * n and filled[pos // n][pos % n]:
            pos += 1
        if pos == n * n:
            return True               # every square covered
        r, c = divmod(pos, n)
        # option 1: horizontal (cover right neighbour)
        if c + 1 < n and not filled[r][c + 1]:
            filled[r][c] = filled[r][c + 1] = True
            placed.append(((r, c), (r, c + 1)))
            if solve(pos + 1):
                return True
            placed.pop()              # backtrack
            filled[r][c] = filled[r][c + 1] = False
        # option 2: vertical (cover neighbour below)
        if r + 1 < n and not filled[r + 1][c]:
            filled[r][c] = filled[r + 1][c] = True
            placed.append(((r, c), (r + 1, c)))
            if solve(pos + 1):
                return True
            placed.pop()              # backtrack
            filled[r][c] = filled[r + 1][c] = False
        return False

    if solve(0):
        return True, [d for pair in placed for d in pair]  # flat (row,col) list
    return False, []


def pairs(dominoes):
    return [(dominoes[i], dominoes[i + 1]) for i in range(0, len(dominoes), 2)]


def verify(n, m1, m2, dominoes):
    """Independent checker: full coverage, no overlap, adjacency."""
    seen = {m1, m2}
    for a, b in pairs(dominoes):
        if abs(a[0] - b[0]) + abs(a[1] - b[1]) != 1:
            return False
        for cell in (a, b):
            if cell in seen or not (0 <= cell[0] < n and 0 <= cell[1] < n):
                return False
            seen.add(cell)
    return len(seen) == n * n


def show(n, m1, m2, dominoes):
    grid = [["."] * n for _ in range(n)]
    grid[m1[0]][m1[1]] = grid[m2[0]][m2[1]] = "X"
    letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef"
    for i, (a, b) in enumerate(pairs(dominoes)):
        grid[a[0]][a[1]] = grid[b[0]][b[1]] = letters[i]
    return "\n".join(" ".join(row) for row in grid)


if __name__ == "__main__":
    import json
    if len(sys.argv) == 1:
        print("usage: python domino_tiling.py tests.json")
        sys.exit(0)
    for t in json.load(open(sys.argv[1])):
        n, m1, m2 = t["n"], tuple(t["missing1"]), tuple(t["missing2"])
        ok, d = tileDominoes(n, m1, m2)
        print(f"--- {t['name']} ---")
        print(f"Input : n={n}, missing1={m1}, missing2={m2}")
        print(f"Output: possible={ok}")
        if ok:
            print("dominoes:", pairs(d))
            print(show(n, m1, m2, d))
            print("verified:", verify(n, m1, m2, d))
        print()
