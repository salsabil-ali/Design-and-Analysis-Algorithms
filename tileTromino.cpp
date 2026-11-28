// Task 8 - Colored Tromino Tiling (Divide and Conquer)
// Build: g++ -std=c++17 -O2 tileTromino.cpp -o tromino
// Run  : ./tromino            (then type: n missingRow missingCol)
//        ./tromino --test     (runs built-in self tests)
#include <bits/stdc++.h>
using namespace std;

struct Tromino {
    array<pair<int,int>, 3> cells; // {row, col} x3
    int color = 0;                 // 0 = Blue, 1 = Yellow, 2 = Green
};

static void solve(int top, int left, int s, int mr, int mc, vector<Tromino>& out) {
    if (s == 1) return;
    int h = s / 2, midR = top + h, midC = left + h;
    pair<int,int> centers[4] = {{midR-1, midC-1}, {midR-1, midC}, {midR, midC-1}, {midR, midC}};
    int qi = (mr >= midR ? 2 : 0) + (mc >= midC ? 1 : 0); // quadrant with the missing cell
    Tromino t; int k = 0;
    int subR[4], subC[4];
    for (int q = 0; q < 4; q++) {
        if (q == qi) { subR[q] = mr; subC[q] = mc; }
        else { t.cells[k++] = centers[q]; subR[q] = centers[q].first; subC[q] = centers[q].second; }
    }
    out.push_back(t); // centre tromino
    for (int q = 0; q < 4; q++)
        solve(top + (q >> 1) * h, left + (q & 1) * h, h, subR[q], subC[q], out);
}

vector<Tromino> tileTromino(int n, int missingRow, int missingCol) {
    int size = 1 << n;
    vector<Tromino> tr;
    solve(0, 0, size, missingRow, missingCol, tr);

    // adjacency graph between trominoes
    int N = tr.size();
    vector<int> owner(size * size, -1);
    for (int i = 0; i < N; i++) for (auto& c : tr[i].cells) owner[c.first * size + c.second] = i;
    vector<set<int>> adj(N);
    for (int r = 0; r < size; r++) for (int c = 0; c < size; c++) {
        int a = owner[r * size + c]; if (a < 0) continue;
        if (c + 1 < size) { int b = owner[r * size + c + 1]; if (b >= 0 && b != a) { adj[a].insert(b); adj[b].insert(a); } }
        if (r + 1 < size) { int b = owner[(r + 1) * size + c]; if (b >= 0 && b != a) { adj[a].insert(b); adj[b].insert(a); } }
    }

    // 3-colouring: DSATUR order + iterative backtracking
    vector<int> col(N, -1);
    auto pickNext = [&]() {
        int best = -1, bs = -1, bd = -1;
        for (int v = 0; v < N; v++) if (col[v] < 0) {
            set<int> s; for (int u : adj[v]) if (col[u] >= 0) s.insert(col[u]);
            int sat = s.size(), d = adj[v].size();
            if (sat > bs || (sat == bs && d > bd)) { best = v; bs = sat; bd = d; }
        }
        return best;
    };
    vector<pair<int,int>> st; // (vertex, next colour to try)
    int v = pickNext();
    if (v >= 0) st.push_back({v, 0});
    while (!st.empty()) {
        auto& [cv, nxt] = st.back();
        bool placed = false;
        while (nxt < 3) {
            int k = nxt++; bool ok = true;
            for (int u : adj[cv]) if (col[u] == k) { ok = false; break; }
            if (ok) { col[cv] = k; placed = true; break; }
        }
        if (!placed) { col[cv] = -1; st.pop_back(); continue; }
        v = pickNext();
        if (v < 0) break;
        st.push_back({v, 0});
    }
    for (int i = 0; i < N; i++) tr[i].color = col[i];
    return tr;
}

// Checks: full coverage, no overlap, L-shape, adjacent tiles differ in colour
bool verify(int n, int mr, int mc, const vector<Tromino>& tr) {
    int s = 1 << n;
    if ((int)tr.size() * 3 != s * s - 1) return false;
    vector<int> g(s * s, -1);
    for (int i = 0; i < (int)tr.size(); i++) {
        int minR = 1e9, maxR = -1, minC = 1e9, maxC = -1;
        for (auto& [r, c] : tr[i].cells) {
            if (r < 0 || c < 0 || r >= s || c >= s || g[r * s + c] != -1) return false;
            g[r * s + c] = i;
            minR = min(minR, r); maxR = max(maxR, r); minC = min(minC, c); maxC = max(maxC, c);
        }
        if (maxR - minR != 1 || maxC - minC != 1) return false; // must fit in 2x2 box
        if (tr[i].color < 0 || tr[i].color > 2) return false;
    }
    if (g[mr * s + mc] != -1) return false;
    for (int r = 0; r < s; r++) for (int c = 0; c < s; c++) {
        int a = g[r * s + c]; if (a < 0) continue;
        if (c + 1 < s) { int b = g[r * s + c + 1]; if (b >= 0 && b != a && tr[a].color == tr[b].color) return false; }
        if (r + 1 < s) { int b = g[(r + 1) * s + c]; if (b >= 0 && b != a && tr[a].color == tr[b].color) return false; }
    }
    return true;
}

void printResult(int n, int mr, int mc, const vector<Tromino>& tr) {
    const char* names[] = {"Blue", "Yellow", "Green"};
    int s = 1 << n;
    cout << "Number of trominoes: " << tr.size() << "\n";
    if (n <= 4) {
        for (size_t i = 0; i < tr.size(); i++) {
            cout << "Tromino " << i + 1 << ": ";
            for (auto& [r, c] : tr[i].cells) cout << "(" << r << "," << c << ") ";
            cout << "- " << names[tr[i].color] << "\n";
        }
    }
    vector<string> grid(s, string(s, '.'));
    for (auto& t : tr) for (auto& [r, c] : t.cells) grid[r][c] = "BYG"[t.color];
    grid[mr][mc] = 'X';
    cout << "\nBoard (B=Blue, Y=Yellow, G=Green, X=missing):\n";
    if (n <= 5) for (auto& row : grid) { for (char ch : row) cout << ch << ' '; cout << "\n"; }
    else cout << "(board too large to print)\n";
    cout << "Valid: " << (verify(n, mr, mc, tr) ? "YES" : "NO") << "\n";
}

int main(int argc, char** argv) {
    if (argc > 1 && string(argv[1]) == "--test") {
        int fails = 0, total = 0;
        for (int n = 1; n <= 6; n++) {
            int s = 1 << n;
            for (int r = 0; r < s; r += max(1, s / 8)) for (int c = 0; c < s; c += max(1, s / 8)) {
                total++;
                if (!verify(n, r, c, tileTromino(n, r, c))) { fails++; cout << "FAIL n=" << n << " (" << r << "," << c << ")\n"; }
            }
        }
        cout << (total - fails) << "/" << total << " tests passed\n";
        return fails != 0;
    }
    int n, mr, mc;
    cout << "Enter n missingRow missingCol: ";
    if (!(cin >> n >> mr >> mc)) return 1;
    int s = 1 << n;
    if (n < 1 || mr < 0 || mc < 0 || mr >= s || mc >= s) { cout << "Invalid input\n"; return 1; }
    printResult(n, mr, mc, tileTromino(n, mr, mc));
}
