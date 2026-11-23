export interface Tromino {
  cells: [number, number][]; // three [row, col] cells forming an L
  color: 0 | 1 | 2;          // 0 = blue, 1 = yellow, 2 = green
}

export function tileTromino(n: number, missingRow: number, missingCol: number): Tromino[] {
  const size = 1 << n;
  const trominoes: Tromino[] = [];

  // Divide and conquer: split board into 4 quadrants; put one L-tromino at the
  // centre covering the three quadrants that do not contain the missing cell.
  // Each such quadrant then has exactly one "missing" cell (the one covered).
  function solve(top: number, left: number, s: number, mr: number, mc: number): void {
    if (s === 1) return;
    const h = s / 2;
    const midR = top + h, midC = left + h;
    const centers: [number, number][] = [[midR - 1, midC - 1], [midR - 1, midC], [midR, midC - 1], [midR, midC]];
    const qi = (mr >= midR ? 2 : 0) + (mc >= midC ? 1 : 0); // quadrant of missing cell
    const cells: [number, number][] = [];
    const sub: [number, number, number, number][] = [];
    for (let q = 0; q < 4; q++) {
      const qt = top + (q >> 1) * h, ql = left + (q & 1) * h;
      if (q === qi) sub.push([qt, ql, mr, mc]);
      else { cells.push(centers[q]); sub.push([qt, ql, centers[q][0], centers[q][1]]); }
    }
    trominoes.push({ cells, color: 0 });
    for (const [qt, ql, r, c] of sub) solve(qt, ql, h, r, c);
  }
  solve(0, 0, size, missingRow, missingCol);

  // Build adjacency graph (trominoes sharing an edge).
  const owner = new Int32Array(size * size).fill(-1);
  trominoes.forEach((t, i) => t.cells.forEach(([r, c]) => (owner[r * size + c] = i)));
  const adj: Set<number>[] = trominoes.map(() => new Set<number>());
  for (let r = 0; r < size; r++)
    for (let c = 0; c < size; c++) {
      const a = owner[r * size + c];
      if (a < 0) continue;
      for (const [dr, dc] of [[0, 1], [1, 0]]) {
        const rr = r + dr, cc = c + dc;
        if (rr >= size || cc >= size) continue;
        const b = owner[rr * size + cc];
        if (b >= 0 && b !== a) { adj[a].add(b); adj[b].add(a); }
      }
    }

  // 3-colour with DSATUR-ordered backtracking (iterative to avoid deep recursion).
  const N = trominoes.length;
  const col = new Int8Array(N).fill(-1);
  const sat = (v: number) => { const s = new Set<number>(); adj[v].forEach(u => col[u] >= 0 && s.add(col[u])); return s.size; };
  const pickNext = () => {
    let best = -1, bs = -1, bd = -1;
    for (let v = 0; v < N; v++) if (col[v] < 0) {
      const s = sat(v), d = adj[v].size;
      if (s > bs || (s === bs && d > bd)) { best = v; bs = s; bd = d; }
    }
    return best;
  };
  const stack: { v: number; next: number }[] = [];
  let v = pickNext();
  if (v >= 0) stack.push({ v, next: 0 });
  while (stack.length) {
    const top = stack[stack.length - 1];
    let placed = false;
    while (top.next < 3) {
      const k = top.next++;
      let ok = true;
      for (const u of adj[top.v]) if (col[u] === k) { ok = false; break; }
      if (ok) { col[top.v] = k; placed = true; break; }
    }
    if (!placed) { col[top.v] = -1; stack.pop(); continue; }
    v = pickNext();
    if (v < 0) break;
    stack.push({ v, next: 0 });
  }
  trominoes.forEach((t, i) => (t.color = col[i] as 0 | 1 | 2));
  return trominoes;
}
