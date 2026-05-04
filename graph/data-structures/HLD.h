struct Node {
    int val = 0;
    Node(int v = 0) : val(v) {}
} neutral;

Node merge(Node a, Node b) {
    Node ret;
    ret.val = (a.val + b.val);
}

struct segtree {
    segtree *left = nullptr, *right = nullptr;
    Node node;
    int start, end;

    segtree(int l = 0, int r = 0) : start(l), end(r) {}

    void extend() {
        if (left == nullptr) {
            int mid = (start + end) >> 1; // Added parentheses for safety
            left = new segtree(start, mid);
            right = new segtree(mid + 1, end);
        }
    }

    Node pushup(Node a, Node b) {
        return merge(a, b);
    }

    void update(int idx, int val) {
        if (start > idx || end < idx) return;
        if (start == end) {
            node.val = val;
            return;
        }
        extend();
        left->update(idx, val);
        right->update(idx, val);
        node = pushup(left->node, right->node);
    }

    Node query(int l, int r) {
        if (r < start || end < l) return neutral;
        extend();
        if (l <= start && end <= r) return node;
        return pushup(left->query(l, r), right->query(l, r));
    }

    ~segtree() {
        if (left != nullptr) {
            delete left;
            delete right;
        }
    }
};

struct HLD {
    int n;
    vector<int> par, depth, heavy, head, cnt, pos_array, pos_node, val, edge_node;
    vector<vector<int>> adj; // Standardized to simple unweighted adjacency list internally
    bool value_on_edge = false;
    segtree* seg;

    // CONSTRUCTOR 1: For EDGE weights
    // adj_edges: u -> {v, weight, edge_index}
    HLD(int n, vector<vector<array<int, 3>>>& adj_edges) : n(n) {
        value_on_edge = true;
        adj.assign(n + 1, vector<int>());
        val.assign(n + 1, neutral.val);
        edge_node.assign(n + 1, 0);

        auto direct_edge = [&](auto& self, int u, int p) -> void {
            for (auto& [v, w, idx] : adj_edges[u]) {
                if (v == p) continue;
                edge_node[idx] = v; // Map edge index to the deeper node
                adj[u].push_back(v);
                adj[v].push_back(u);
                val[v] = w; // Store edge weight in deeper node
                self(self, v, u);
            }
        };
        direct_edge(direct_edge, 1, -1);
        init();
    }

    // CONSTRUCTOR 2: For NODE weights
    // unweighted_adj: u -> {v} (standard adjacency list)
    HLD(int n, vector<vector<int>>& unweighted_adj, vector<int>& node_vals) : n(n) {
        value_on_edge = false;
        adj = unweighted_adj;
        val = node_vals; // node_vals should be 1-indexed (size n + 1)
        init();
    }

    void init() {
        depth = head = cnt = pos_array = pos_node = vector<int>(n + 1);
        heavy = par = vector<int>(n + 1, -1);
        decompose(1);
    }

    void dfs_hld(int u) {
        cnt[u] = 1;
        int max_size = 0;
        for (auto v : adj[u]) {
            if (v == par[u]) continue;
            par[v] = u;
            depth[v] = depth[u] + 1;
            dfs_hld(v);
            cnt[u] += cnt[v];
            if (max_size < cnt[v]) {
                max_size = cnt[v];
                heavy[u] = v;
            }
        }
    }

    void decompose(int root = 1) {
        dfs_hld(root);
        int nxt = 1; // FIX: Changed from 0 to 1 because segtree is built from 1 to n
        for (int chain_root = 1; chain_root <= n; chain_root++) {
            if (par[chain_root] == -1 || heavy[par[chain_root]] != chain_root) {
                for (int ch = chain_root; ~ch; ch = heavy[ch]) {
                    head[ch] = chain_root;
                    pos_array[ch] = nxt;
                    pos_node[nxt++] = ch;
                }
            }
        }
        seg = new segtree(1, n);
        for (int u = 1; u <= n; u++) {
            seg->update(pos_array[u], val[u]);
        }
    }

    Node query(int u, int v) {
        Node ans = neutral;
        for (; head[u] != head[v]; v = par[head[v]]) {
            if (depth[head[u]] > depth[head[v]]) swap(u, v);
            Node cur = seg->query(pos_array[head[v]], pos_array[v]);
            ans = seg->pushup(ans, cur);
        }
        if (depth[u] > depth[v]) swap(u, v);

        // value_on_edge natively acts as an integer:
        // 1 (true) skips the LCA node, 0 (false) includes the LCA node!
        if (pos_array[u] + value_on_edge <= pos_array[v]) {
            Node last_chain = seg->query(pos_array[u] + value_on_edge, pos_array[v]);
            ans = seg->pushup(ans, last_chain);
        }
        return ans;
    }

    void update_node(int u, int c) {
        seg->update(pos_array[u], c);
    }

    void update_edge(int edge_idx, int c) {
        if (!value_on_edge) return; // Safeguard
        update_node(edge_node[edge_idx], c);
    }

    ~HLD() {
        delete seg;
    }
};