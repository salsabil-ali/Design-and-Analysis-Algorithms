"""Exhaustive verification: greedy answer vs brute-force search (n <= 10)
and vs the parity rule 'possible <=> odd number of heads' (n <= 16)."""
from itertools import product
from functools import lru_cache
from task1_coin_removal import removeCoins, simulate


def brute(coins):
    n = len(coins)

    @lru_cache(None)
    def go(state, alive):                    # bitmasks
        if alive == 0:
            return True
        for i in range(n):
            if alive >> i & 1 and state >> i & 1:
                s, a = state, alive & ~(1 << i)
                for j in (i - 1, i + 1):
                    if 0 <= j < n and a >> j & 1:
                        s ^= 1 << j
                if go(s, a):
                    return True
        return False

    st = sum(1 << i for i, c in enumerate(coins) if c == "H")
    return go(st, (1 << n) - 1)


total = 0
for n in range(1, 17):
    for row in product("HT", repeat=n):
        ok, seq = removeCoins(n, row)
        assert ok == (row.count("H") % 2 == 1), row      # parity rule
        if ok:
            assert simulate(row, seq), row               # valid sequence
        else:
            assert seq == [], row
        if n <= 10:
            assert ok == brute(row), row                 # brute force
        total += 1
print(f"All {total} rows (n=1..16) passed.")
