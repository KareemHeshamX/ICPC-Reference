/**
 * Algorithm: General Graph Maximum Weight Perfect Matching
 * Complexity: O(V^3)
 * Description: Finds a perfect matching that maximizes the sum of edge weights.
 * Usage:
 * MaxWeightMatching mwm(n); // n MUST be even. Add dummy nodes if necessary.
 * mwm.w[u][v] = mwm.w[v][u] = weight; // 1-based indexing
 * long long max_weight = mwm.solve();
 * // mwm.mate[u] contains the matched pair of u.
 */
#include <bits/stdc++.h>
using namespace std;

struct MaxWeightMatching {
    static const int INF = 2e9; // Use long long INF if weights are massive
    int n, n_x;
    vector<vector<int>> w;
    vector<int> mate, link, base, vis;
    vector<int> q;
    vector<int> slack;
    vector<int> lx, ly;
    int head, tail;

    MaxWeightMatching(int n) : n(n), n_x(n + 1), w(n + 1, vector<int>(n + 1, 0)),
                               mate(n + 1, 0), link(n + 1, 0), base(n + 1, 0),
                               vis(n + 1, 0), q(n + 1, 0), slack(n + 1, 0),
                               lx(n + 1, 0), ly(n + 1, 0) {}

    int get_lca(int u, int v) {
        static int t = 0;
        t++;
        while (u || v) {
            if (u) {
                if (vis[base[u]] == t) return base[u];
                vis[base[u]] = t;
                u = link[mate[u]];
            }
            swap(u, v);
        }
        return 0;
    }

    void group(int u, int lca) {
        while (base[u] != lca) {
            int v = mate[u];
            link[v] = u;
            base[u] = base[v] = lca;
            if (vis[v] == 1) {
                vis[v] = 0;
                q[tail++] = v;
            }
            u = link[v];
        }
    }

    void add_edge(int u, int lca) {
        group(u, lca);
    }

    void match(int u) {
        for (int i = 1; i <= n; i++) {
            base[i] = i;
            vis[i] = -1;
            link[i] = 0;
            slack[i] = 0;
        }
        head = tail = 0;
        q[tail++] = u;
        vis[u] = 0;

        while (true) {
            while (head < tail) {
                int x = q[head++];
                for (int y = 1; y <= n; y++) {
                    if (w[x][y] == 0 || base[x] == base[y] || mate[x] == y) continue;
                    if (vis[y] == -1) {
                        link[y] = x;
                        slack[y] = x;
                        vis[y] = 1;
                        if (!mate[y]) {
                            int curr = y;
                            while (curr) {
                                int pre = link[curr];
                                int next_match = mate[pre];
                                mate[curr] = pre;
                                mate[pre] = curr;
                                curr = next_match;
                            }
                            return;
                        }
                        vis[mate[y]] = 0;
                        q[tail++] = mate[y];
                    } else if (vis[y] == 0) {
                        int lca = get_lca(x, y);
                        add_edge(x, lca);
                        add_edge(y, lca);
                    }
                }
            }
            // Update duals if BFS is exhausted
            int d = INF;
            for (int i = 1; i <= n; i++) {
                if (vis[i] == -1 && slack[i]) {
                    d = min(d, lx[slack[i]] + ly[i] - w[slack[i]][i]);
                }
            }
            if (d == INF) return; // No perfect matching exists

            for (int i = 1; i <= n; i++) {
                if (vis[i] == 0) lx[i] -= d;
                else if (vis[i] == 1) ly[i] += d;
                else if (vis[i] == -1 && slack[i]) {
                    slack[i] = (lx[slack[i]] + ly[i] - w[slack[i]][i] == 0) ? slack[i] : slack[i];
                    // To keep implementation robust against zero-weight drifts
                }
            }

            for (int i = 1; i <= n; i++) {
                if (vis[i] == -1 && slack[i] && lx[slack[i]] + ly[i] == w[slack[i]][i]) {
                    vis[i] = 1;
                    if (!mate[i]) {
                        int curr = i;
                        while (curr) {
                            int pre = slack[curr]; // Use slack to trace back
                            if(!pre) pre = link[curr];
                            int next_match = mate[pre];
                            mate[curr] = pre;
                            mate[pre] = curr;
                            curr = next_match;
                        }
                        return;
                    }
                    vis[mate[i]] = 0;
                    q[tail++] = mate[i];
                }
            }
        }
    }

    long long solve() {
        // Initialize potentials
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                lx[i] = max(lx[i], w[i][j]);
            }
        }
        for (int i = 1; i <= n; i++) {
            if (!mate[i]) match(i);
        }
        long long ans = 0;
        for (int i = 1; i <= n; i++) {
            if (mate[i] > i) {
                ans += w[i][mate[i]];
            }
        }
        return ans;
    }
};