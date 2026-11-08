// Eulerian path / cycle -- Hierholzer, iterative, O(V+E).
// Directed or undirected; multi-edges and self-loops are fine.
//   Euler g(n, directed);   g.add(u, v);     // vertices 0..n-1, edge ids 0,1,2.. in add order
//   auto p = g.solve();        // any Eulerian path          -> vertices p[0..m]
//   auto p = g.solve(s);       // path that must start at s
//   auto p = g.solve(-1, 1);   // Eulerian cycle (p[0] == p[m])
//   {} means impossible.  g.E[i] = id of the edge between p[i] and p[i+1].
struct Euler {
    int n, m = 0;
    bool dir;
    vector<vector<pair<int, int>>> g;  // (to, edge id)
    vector<int> in, E;
    Euler(int n, bool dir) : n(n), dir(dir), g(n), in(n) {}
    void add(int u, int v) {
        g[u].push_back({v, m});
        if (!dir) g[v].push_back({u, m});
        in[v]++, m++;
    }
    // d(v) = 1: v must be the start, -1: v must be the end, 0: balanced
    int d(int v) { return dir ? (int)g[v].size() - in[v] : (int)g[v].size() & 1; }
    vector<int> solve(int s = -1, bool cycle = false) {
        if (!m) return {s < 0 ? 0 : s};
        int bad = 0, f = -1;
        for (int v = 0; v < n; v++) {
            int x = d(v);
            if (abs(x) > 1) return {};
            bad += x != 0;
            if (x == 1) f = v;
        }
        if (bad > (cycle ? 0 : 2)) return {};
        if (s < 0) s = f;                                   // start forced by degrees
        if (s < 0) for (s = 0; g[s].empty(); s++);          // else: any vertex with an edge
        if (bad && d(s) != 1) return {};
        vector<int> it(n), res, st = {s}, se = {-1};
        vector<char> used(m);
        E.clear();
        while (!st.empty()) {
            int v = st.back(), &i = it[v];
            while (i < (int)g[v].size() && used[g[v][i].second]) i++;
            if (i == (int)g[v].size()) {
                res.push_back(v), E.push_back(se.back());
                st.pop_back(), se.pop_back();
            } else {
                auto [to, id] = g[v][i++];
                used[id] = 1, st.push_back(to), se.push_back(id);
            }
        }
        if ((int)res.size() != m + 1) return {};            // edges not all connected
        E.pop_back();
        reverse(res.begin(), res.end()), reverse(E.begin(), E.end());
        return res;
    }
};