/**
 * Algorithm: Edmonds' Blossom (Unweighted)
 * Complexity: O(V^3)
 * Description: Finds the maximum matching in a general unweighted graph.
 * Usage:
 * EdmondsBlossom graph(n);
 * graph.add_edge(u, v); // 1-based indexing
 * int max_match_size = graph.solve();
 * // graph.match[u] contains the matched pair of u, or 0 if unmatched.
 */
#include <bits/stdc++.h>
using namespace std;

struct EdmondsBlossom {
    int n;
    vector<vector<int>> adj;
    vector<int> match, p, base, q;
    vector<bool> inq, inb;
    vector<int> vis;
    int timer;

    EdmondsBlossom(int n) : n(n), adj(n + 1), match(n + 1, 0), p(n + 1, 0),
                            base(n + 1, 0), q(n + 1, 0), inq(n + 1, false),
                            inb(n + 1, false), vis(n + 1, 0), timer(0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int get_lca(int u, int v) {
        timer++;
        while (true) {
            if (u != 0) {
                u = base[u];
                if (vis[u] == timer) return u;
                vis[u] = timer;
                u = p[match[u]];
            }
            swap(u, v);
        }
    }

    void mark_blossom(int lca, int u) {
        while (base[u] != lca) {
            int v = match[u];
            inb[base[u]] = inb[base[v]] = true;
            u = p[v];
            if (base[u] != lca) p[u] = v;
        }
    }

    void blossom_contraction(int u, int v, int &tail) {
        int lca = get_lca(u, v);
        fill(inb.begin(), inb.end(), false);
        mark_blossom(lca, u);
        mark_blossom(lca, v);
        if (base[u] != lca) p[u] = v;
        if (base[v] != lca) p[v] = u;
        for (int i = 1; i <= n; i++) {
            if (inb[base[i]]) {
                base[i] = lca;
                if (!inq[i]) {
                    inq[i] = true;
                    q[tail++] = i;
                }
            }
        }
    }

    bool bfs(int s) {
        fill(p.begin(), p.end(), 0);
        fill(inq.begin(), inq.end(), false);
        iota(base.begin(), base.end(), 0);

        int head = 0, tail = 0;
        q[tail++] = s;
        inq[s] = true;

        while (head < tail) {
            int u = q[head++];
            for (int v : adj[u]) {
                if (base[u] == base[v] || match[u] == v) continue;
                if (v == s || (match[v] != 0 && p[match[v]] != 0)) {
                    blossom_contraction(u, v, tail);
                } else if (p[v] == 0) {
                    p[v] = u;
                    if (match[v] == 0) {
                        int curr = v;
                        while (curr != 0) {
                            int next_node = p[curr];
                            int next_match = match[next_node];
                            match[curr] = next_node;
                            match[next_node] = curr;
                            curr = next_match;
                        }
                        return true;
                    } else {
                        inq[match[v]] = true;
                        q[tail++] = match[v];
                    }
                }
            }
        }
        return false;
    }

    int solve() {
        int ans = 0;
        for (int i = 1; i <= n; i++) {
            if (match[i] == 0 && bfs(i)) {
                ans++;
            }
        }
        return ans; // Returns the number of edges in the matching
    }
};