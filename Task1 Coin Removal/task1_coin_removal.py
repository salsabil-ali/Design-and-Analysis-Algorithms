
from collections import deque
import sys


def removeCoins(n, coins):
    coins = list(coins)
    assert len(coins) == n and all(c in "HT" for c in coins)

    # need[i] = parity of the number of neighbours that must be removed BEFORE i
    # so that coin i is heads-up when it is removed:  'H' -> even, 'T' -> odd.
    need = [1 if c == "T" else 0 for c in coins]

    # GREEDY PASS (left -> right) 
    # first[i] = True  : coin i is removed BEFORE coin i+1 (i flips i+1)
    # first[i] = False : coin i+1 is removed BEFORE coin i (i+1 flips i)
    # The left edge of coin i is already fixed, so its right edge is FORCED.
    first = [False] * max(n - 1, 0)
    left_in = 0                              # flips coin i receives from the left
    for i in range(n - 1):
        right_in = need[i] ^ left_in         # flips i must receive from the right
        first[i] = (right_in == 0)           # 0 -> i goes first
        left_in = 1 if first[i] else 0       # does i+1 receive a flip from i?
    if left_in != need[n - 1]:               # last coin can't be fixed
        return False, []

    # ---- BUILD THE REMOVAL ORDER (topological order of the path) ----------
    indeg = [0] * n
    for i in range(n - 1):
        indeg[i + 1 if first[i] else i] += 1
    queue = deque(i for i in range(n) if indeg[i] == 0)
    order = []
    while queue:
        i = queue.popleft()
        order.append(i)
        if i + 1 < n and first[i]:
            indeg[i + 1] -= 1
            if indeg[i + 1] == 0:
                queue.append(i + 1)
        if i - 1 >= 0 and not first[i - 1]:
            indeg[i - 1] -= 1
            if indeg[i - 1] == 0:
                queue.append(i - 1)
    return True, order


def simulate(coins, order):
    heads = [c == "H" for c in coins]
    alive = [True] * len(coins)
    for i in order:
        if not alive[i] or not heads[i]:
            return False
        alive[i] = False
        for j in (i - 1, i + 1):
            if 0 <= j < len(coins) and alive[j]:
                heads[j] = not heads[j]
    return not any(alive)


def run(row):
    possible, seq = removeCoins(len(row), list(row))
    print(f"Input   : {row}  (n={len(row)}, heads={row.count('H')})")
    print(f"Possible: {possible}")
    if possible:
        print(f"Sequence: {seq}")
        print(f"Verified: {simulate(row, seq)}")
    print()


if __name__ == "__main__":
    # usage: python task1_coin_removal.py HTHHT HHH      or      -f tests/task1_inputs.txt
    args = sys.argv[1:]
    if args[:1] == ["-f"]:
        with open(args[1]) as f:
            rows = [l.strip() for l in f if l.strip() and not l.startswith("#")]
    else:
        rows = args or ["HTHHT"]
    for r in rows:
        run(r.upper())
