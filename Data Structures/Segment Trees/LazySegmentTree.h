struct Node {
    int val = 0, lazy = 0;

    void apply(int l, int r, int v) {
        val += v * (r - l + 1);
        lazy += v;
    }

    friend Node merge(const Node& a, const Node& b) {
        Node res;
        res.val = a.val + b.val;
        return res;
    }
};

struct segment_tree {
    int n;
    vector<Node> tree;

    segment_tree(int n) : n(n), tree(4 * n) {}

    void push(int u, int l, int r) {
        if (tree[u].lazy == 0) return;
        int mid = l + (r - l) / 2;
        tree[2 * u].apply(l, mid, tree[u].lazy);
        tree[2 * u + 1].apply(mid + 1, r, tree[u].lazy);
        tree[u].lazy = 0;
    }

    void update(int u, int l, int r, int ql, int qr, int val) {
        if (l > qr || r < ql) return;
        if (l >= ql && r <= qr) { tree[u].apply(l, r, val); return; }
        push(u, l, r);
        int mid = l + (r - l) / 2;
        update(2 * u, l, mid, ql, qr, val);
        update(2 * u + 1, mid + 1, r, ql, qr, val);
        tree[u] = merge(tree[2 * u], tree[2 * u + 1]);
    }

    Node query(int u, int l, int r, int ql, int qr) {
        if (l > qr || r < ql) return Node();
        if (l >= ql && r <= qr) return tree[u];
        push(u, l, r);
        int mid = l + (r - l) / 2;
        return merge(
            query(2 * u, l, mid, ql, qr),
            query(2 * u + 1, mid + 1, r, ql, qr)
            );
    }

    void update(int l, int r, int val) { update(1, 0, n - 1, l, r, val); }
    int query(int l, int r) { return query(1, 0, n - 1, l, r).val; }
};