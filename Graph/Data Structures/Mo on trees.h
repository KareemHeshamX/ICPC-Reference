#include <bits/stdc++.h>
using namespace std;

// Mo's algorithm on tree paths (Euler tour + Hilbert order). Requires C++17.
//   T             : value / answer type
//   EdgeT         : `int` for unweighted graphs, `pair<int, W>` for weighted ones
//   VALUE_ON_EDGE : true  -> value of an edge is stored on its child node (LCA excluded)
//                   false -> values live on nodes (LCA included)
template <typename T = int, typename EdgeT = int, bool VALUE_ON_EDGE = false>
class MoTree {
    static constexpr bool kUnweighted = is_same_v<EdgeT, int>;

    struct Query {
        int left, right;   // range on the Euler tour
        int lca;           // extra node to add temporarily, or -1 if none
        int id;            // index of the query in input order
        int64_t order;     // Hilbert order key
        bool operator<(const Query& o) const { return order < o.order; }
    };

    int n, LOG = 0, hilbertBits = 0, timer = 1;
    int curLeft = 0, curRight = -1;
    T answer{};
    vector<T> value, results;
    vector<int> tin, tout, euler;
    vector<char> inPath;
    vector<vector<int>> up;   // up[u][b] = 2^b-th ancestor (root points to itself)
    vector<Query> queries;

public:
    MoTree(int n, const vector<vector<EdgeT>>& graph, vector<T> values = {}, int root = 1)
        : n(n), value(move(values)), tin(n + 1), tout(n + 1), euler(2 * n + 2), inPath(n + 1) {
        if ((int)value.size() < n + 1) value.resize(n + 1);
        while ((1 << LOG) <= n) LOG++;
        while ((1 << hilbertBits) <= 2 * n) hilbertBits++;
        up.assign(n + 1, vector<int>(LOG));
        dfs(root, root, graph);
    }

    void addQuery(int u, int v) {
        if (tin[u] > tin[v]) swap(u, v);
        const int a = lca(u, v);
        Query q;
        q.id = queries.size();
        q.right = tin[v];
        if (a == u) q.left = tin[u] + VALUE_ON_EDGE, q.lca = -1;
        else        q.left = tout[u],               q.lca = a;
        q.order = hilbertOrder(q.left, q.right, hilbertBits, 0);
        queries.push_back(q);
    }

    // Answers all queries (call once); results are in the order queries were added.
    const vector<T>& solve() {
        results.assign(queries.size(), T{});
        if (queries.empty()) return results;
        sort(queries.begin(), queries.end());
        curLeft = queries[0].left, curRight = curLeft - 1;

        for (const Query& q : queries) {
            moveTo(q.left, q.right);
            const bool needLca = q.lca != -1 && !VALUE_ON_EDGE;
            if (needLca) addNode(q.lca);
            results[q.id] = answer;
            if (needLca) removeNode(q.lca);
        }
        return results;
    }

private:
    // ---- problem-specific part: update `answer` using value[u] ----
    void addNode(int u)    { /* TODO */ }
    void removeNode(int u) { /* TODO */ }

    // ---- Mo machinery ----
    void moveTo(int l, int r) {
        while (curLeft > l)  toggle(--curLeft);
        while (curRight < r) toggle(++curRight);
        while (curLeft < l)  toggle(curLeft++);
        while (curRight > r) toggle(curRight--);
    }

    void toggle(int pos) {
        const int u = euler[pos];
        (inPath[u] ^= 1) ? addNode(u) : removeNode(u);
    }

    // ---- tree preprocessing ----
    void dfs(int u, int parent, const vector<vector<EdgeT>>& graph) {
        tin[u] = timer, euler[timer++] = u;
        up[u][0] = parent;
        for (int b = 1; b < LOG; b++) up[u][b] = up[up[u][b - 1]][b - 1];

        for (const auto& e : graph[u]) {
            int v;
            if constexpr (kUnweighted) v = e; else v = e.first;
            if (v == parent) continue;
            if constexpr (!kUnweighted) value[v] = e.second;
            dfs(v, u, graph);
        }
        tout[u] = timer, euler[timer++] = u;
    }

    bool isAncestor(int u, int v) const { return tin[u] <= tin[v] && tout[v] <= tout[u]; }

    int lca(int u, int v) const {
        if (isAncestor(u, v)) return u;
        if (isAncestor(v, u)) return v;
        for (int b = LOG - 1; b >= 0; b--)
            if (!isAncestor(up[u][b], v)) u = up[u][b];
        return up[u][0];
    }

    static int64_t hilbertOrder(int x, int y, int bits, int rotation) {
        if (bits == 0) return 0;
        const int half = 1 << (bits - 1);
        int quadrant = x < half ? (y < half ? 0 : 3) : (y < half ? 1 : 2);
        quadrant = (quadrant + rotation) & 3;
        constexpr int rotationDelta[4] = {3, 0, 0, 1};
        const int64_t quadrantSize = int64_t(1) << (2 * bits - 2);
        const int64_t inner = hilbertOrder(x & ~half, y & ~half, bits - 1,
                                           (rotation + rotationDelta[quadrant]) & 3);
        return quadrant * quadrantSize +
               ((quadrant == 1 || quadrant == 2) ? inner : quadrantSize - inner - 1);
    }
};

/* Usage:
   vector<vector<int>> g(n + 1);                 // or vector<vector<pair<int,int>>> for weighted
   MoTree<int, int, false> mo(n, g, values);     // values[1..n]
   mo.addQuery(u, v);                            // for each query
   auto res = mo.solve();
*/