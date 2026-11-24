// Task 7 - Poisoned Wine (Required technique: Divide and Conquer)
//
// Input  (stdin): one or more tester counts, e.g.  10 8 7 6
// Output (stdout): for each count: possible?, verification, schedule summary, sample decodes
//
// Assumptions: 1000 barrels, exactly one is poisoned, poison kills exactly 30 days after
// tasting, feast on day 35. So there are 2 tasting rounds:
//   day 0 -> death on day 30,   day 5 -> death on day 35.
// A tester has 3 outcomes: 0 = survives, 1 = dies day 30, 2 = dies day 35.
// So `testers` testers can tell apart 3^testers barrels.
//
// Divide and conquer:
//   Divide : cut the barrel range into 3 parts.
//   Conquer: tester k-1 skips part 0, drinks part 1 on day 0 and part 2 on day 5.
//            The other k-1 testers repeat the same idea inside each part
//            (they can be reused because only ONE part contains the poison).
//   Combine: nothing to merge.        T(n) = 3T(n/3) + O(n)  =  O(n log n)

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

const int BARRELS = 1000;

// day0[t] / day5[t] = barrels that tester t drinks on day 0 / day 5
struct Schedule {
    vector<vector<int> > day0, day5;
};

// designWineTests returns (possible, schedule, decoder)
struct Result {
    bool possible;
    Schedule schedule;
    function<int(const vector<int> &)> decoder;  // tester outcomes -> poisoned barrel
};

long long power3(int e) {
    long long r = 1;
    for (int i = 0; i < e; i++) {
        r *= 3;
        if (r > 1000000000LL) return r;  // big enough, avoid overflow
    }
    return r;
}

// DIVIDE: cut [lo, hi) into [lo, m1), [m1, m2), [m2, hi), each of size <= ceil(n / 3)
void split3(int lo, int hi, int &m1, int &m2) {
    int third = (hi - lo + 2) / 3;
    m1 = min(lo + third, hi); 
    m2 = min(lo + 2 * third, hi);
}

// CONQUER: schedule barrels [lo, hi) using testers 0 .. k-1
void build(int lo, int hi, int k, Schedule &s) {
    if (hi - lo <= 1 || k == 0) return;  // base case: nothing left to find out
    int m1, m2;
    split3(lo, hi, m1, m2);
    for (int b = m1; b < m2; b++) s.day0[k - 1].push_back(b);  // part 1 -> day 0
    for (int b = m2; b < hi; b++) s.day5[k - 1].push_back(b);  // part 2 -> day 5
    build(lo, m1, k - 1, s);
    build(m1, m2, k - 1, s);
    build(m2, hi, k - 1, s);
}

// Decoder: tester k-1's outcome (0, 1 or 2) says which part holds the poison,
// then repeat inside that part with k-1 testers.
int decode(int lo, int hi, int k, const vector<int> &out) {
    if (hi - lo <= 1 || k == 0) return lo;
    int m1, m2;
    split3(lo, hi, m1, m2);
    if (out[k - 1] == 0) return decode(lo, m1, k - 1, out);
    if (out[k - 1] == 1) return decode(m1, m2, k - 1, out);
    return decode(m2, hi, k - 1, out);
}

Result designWineTests(int testers) {
    Result r;
    r.possible = (power3(testers) >= BARRELS);  // counting argument: 3^testers >= 1000
    if (!r.possible) return r;
    r.schedule.day0.resize(testers);
    r.schedule.day5.resize(testers);
    build(0, BARRELS, testers, r.schedule);
    r.decoder = [testers](const vector<int> &out) { return decode(0, BARRELS, testers, out); };
    return r;
}

// Pretend barrel `poisoned` is the bad one: what happens to each tester?
vector<int> simulate(const Schedule &s, int poisoned) {
    vector<int> out(s.day0.size(), 0);
    for (size_t t = 0; t < out.size(); t++) {
        if (find(s.day0[t].begin(), s.day0[t].end(), poisoned) != s.day0[t].end()) out[t] = 1;
        else if (find(s.day5[t].begin(), s.day5[t].end(), poisoned) != s.day5[t].end()) out[t] = 2;
    }
    return out;
}

// Try every barrel as the poisoned one; the decoder must always find it.
bool verifyAll(const Result &r) {
    for (int b = 0; b < BARRELS; b++)
        if (r.decoder(simulate(r.schedule, b)) != b) return false;
    return true;
}

void report(int testers) {
    cout << "=== testers = " << testers << " ===\n";
    cout << "capacity 3^" << testers << " = " << power3(testers) << " (need " << BARRELS << ")\n";
    Result r = designWineTests(testers);
    if (!r.possible) {
        cout << "possible: false\n\n";
        return;
    }
    cout << "possible: true\n";
    cout << "verified all " << BARRELS << " barrels: " << (verifyAll(r) ? "true" : "false") << "\n";
    for (int t = 0; t < testers; t++)
        cout << "  tester " << t << ": day 0 drinks " << r.schedule.day0[t].size()
             << " barrels, day 5 drinks " << r.schedule.day5[t].size() << " barrels\n";
    int samples[3] = {0, 500, 999};
    for (int i = 0; i < 3; i++) {
        vector<int> out = simulate(r.schedule, samples[i]);
        cout << "  poisoned=" << samples[i] << " -> outcomes [";
        for (int t = 0; t < testers; t++) cout << out[t] << (t + 1 < testers ? ", " : "");
        cout << "] -> decoded " << r.decoder(out) << "\n";
    }
    cout << "\n";
}

int main() {
    int testers, count = 0;
    while (cin >> testers) {
        report(testers);
        count++;
    }
    if (count == 0) cerr << "usage: give one or more tester counts, e.g.  10 8 7 6\n";
    return 0;
}
