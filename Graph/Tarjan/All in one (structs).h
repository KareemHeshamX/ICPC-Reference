#include<bits/stdc++.h>
using namespace std;

struct SCC {
    int n;
    vector<vector<int>> adj;
    vector<int> dfn, low, compId, stk;
    vector<bool> in_stk;
    int timer, scc_cnt;

    SCC(int _n) :
    n(_n), adj(_n + 1), dfn(_n + 1, 0), low(_n + 1, 0),compId(_n + 1, 0),
    in_stk(_n + 1, false), timer(0), scc_cnt(0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    void dfs(int u) {
        dfn[u] = low[u] = ++timer;
        stk.push_back(u);
        in_stk[u] = true;

        for (int v : adj[u]) {
            if (!dfn[v]) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stk[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (low[u] == dfn[u]) {
            scc_cnt++;
            while (true) {
                int v = stk.back();
                stk.pop_back();
                in_stk[v] = false;
                compId[v] = scc_cnt; // Lower compId means it's a sink / found earlier
                if (u == v) break;
            }
        }
    }

    void build() {
        for (int i = 1; i <= n; i++) {
            if (!dfn[i]) dfs(i);
        }
    }
};

struct Bridges {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin, low;
    vector<pair<int, int>> bridges;

    Bridges(int _n) :
    n(_n), adj(_n + 1), tin(_n + 1, 0),
    low(_n + 1, 0), timer(0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p = -1) {
        tin[u] = low[u] = ++timer;
        for (int v : adj[u]) {
            if (v == p) continue;

            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) {
                    bridges.push_back({min(u, v), max(u, v)});
                }
            }
        }
    }

    void build() {
        for (int i = 1; i <= n; i++) {
            if (!tin[i]) dfs(i);
        }
    }
};

struct ArticulationPoints {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin, low;
    vector<bool> is_art;

    ArticulationPoints(int _n) :
    n(_n), adj(_n + 1), tin(_n + 1, 0),
    low(_n + 1, 0), is_art(_n + 1, false), timer(0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p = -1) {
        tin[u] = low[u] = ++timer;
        int children = 0;

        for (int v : adj[u]) {
            if (v == p) continue;

            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                dfs(v, u);
                low[u] = min(low[u], low[v]);

                // Condition for non-root vertices
                if (low[v] >= tin[u] && p != -1) {
                    is_art[u] = true;
                }
                children++;
            }
        }

        // Condition for the root of a DFS tree
        if (p == -1 && children > 1) {
            is_art[u] = true;
        }
    }

    void build() {
        for (int i = 1; i <= n; i++) {
            if (!tin[i]) dfs(i);
        }
    }
};

struct TwoSat {
    int n;
    SCC scc;

    TwoSat(int _n) : n(_n), scc(2 * _n) {}

    int Not(int x) {
        return (x > n ? x - n : x + n);
    }

    void add_impl(int u, int v) {
        scc.add_edge(u, v);
    }

    // Clause: (A OR B)
    void add_or(int a, int b) {
        add_impl(Not(a), b);
        add_impl(Not(b), a);
    }

    // Clause: (A XOR B) -> Must be different
    void add_xor(int a, int b) {
        add_or(a, b);
        add_or(Not(a), Not(b));
    }

    // Clause: (A XNOR B) -> Must be the same
    void add_xnor(int a, int b) {
        add_or(Not(a), b);
        add_or(a, Not(b));
    }

    void force_true(int a) {
        add_impl(Not(a), a);
    }

    void force_false(int a) {
        add_impl(a, Not(a));
    }

    bool solve(vector<int>& value) {
        scc.build();

        value.assign(n + 1, 0);

        for (int i = 1; i <= n; i++) {
            // Contradiction: A and !A belong to the same Strongly Connected Component
            if (scc.compId[i] == scc.compId[Not(i)]) {
                return false;
            }
            // Topological assignment based on SCC IDs
            value[i] = scc.compId[i] < scc.compId[Not(i)];
        }

        return true;
    }
};