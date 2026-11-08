#include <bits/stdc++.h>
using namespace std;

// Eulerian path / cycle (Hierholzer, iterative, O(V + E)).
// Works for directed and undirected graphs, multi-edges and self-loops.
//
// Usage:
//   Euler g(n, directed);          // vertices are 0..n-1
//   g.add_edge(u, v);
//   vector<int> p = g.path();      // Eulerian path  (vertex sequence)
//   vector<int> c = g.cycle();     // Eulerian cycle (first == last vertex)
//
// Returns a sequence of m+1 vertices, or an EMPTY vector if none exists.
// (A graph with no edges returns {0}.)
struct Euler {
    int n, m = 0;
    bool directed;
    vector<vector<pair<int, int>>> g;  // (neighbor, edge id)
    vector<int> in, out;

    Euler(int n, bool directed) : n(n), directed(directed), g(n), in(n), out(n) {}

    void add_edge(int u, int v) {
        g[u].push_back({v, m});
        if (!directed) g[v].push_back({u, m});
        out[u]++, in[v]++;
        m++;
    }

    vector<int> solve(bool need_cycle) {
        if (m == 0) return {0};
        int start = -1, cs = 0, ce = 0;  // cs = # start-like vertices, ce = # end-like
        for (int v = 0; v < n; v++) {
            if (directed) {
                int d = out[v] - in[v];
                if (d == 1) cs++, start = v;
                else if (d == -1) ce++;
                else if (d != 0) return {};
            } else if (g[v].size() % 2) {
                cs++, start = v;
            }
        }
        if (directed ? (cs != ce || cs > 1) : (cs != 0 && cs != 2)) return {};
        if (need_cycle && cs) return {};
        if (start == -1)  // all balanced: start anywhere that has an edge
            for (int v = 0; v < n; v++)
                if (!g[v].empty()) { start = v; break; }

        vector<int> it(n, 0), res, st = {start};
        vector<char> used(m, 0);
        while (!st.empty()) {
            int v = st.back();
            while (it[v] < (int)g[v].size() && used[g[v][it[v]].second]) it[v]++;
            if (it[v] == (int)g[v].size()) {
                res.push_back(v);
                st.pop_back();
            } else {
                auto [to, id] = g[v][it[v]++];
                used[id] = 1;
                st.push_back(to);
            }
        }
        if ((int)res.size() != m + 1) return {};  // edges not all connected
        reverse(res.begin(), res.end());
        return res;
    }

    vector<int> path() { return solve(false); }
    vector<int> cycle() { return solve(true); }
};

int main() {
    // Example: undirected triangle with a tail -> path exists, cycle doesn't
    Euler g(4, false);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 0);
    g.add_edge(2, 3);

    auto p = g.path();
    if (p.empty()) puts("no Eulerian path");
    else for (int v : p) printf("%d ", v);
    puts("");

    puts(g.cycle().empty() ? "no Eulerian cycle" : "has Eulerian cycle");
}