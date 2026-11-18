// ld instead of ll in everything except l, r, x if m or c only are doubles
struct Line {
    ll m = 0, c = -INF;
    ll get(ll x) const { return m * x + c; }
};
// REST OF THE CHANGES IN THE COMMENTS BELOW IS NEEDED IF THE QUERIES ALSO ARE DOUBLES

struct PersistentLiChao {
    struct Node { Line line; int lc = 0, rc = 0; };
    vector<Node> tree;
    vector<int> roots;

    PersistentLiChao(int reserve_nodes = 15000000) {
        tree.reserve(reserve_nodes);
        tree.push_back({}); // node 0: empty sentinel {Line{0, -INF}, 0, 0}
        roots.push_back(0); // version 0: empty tree
    }

    int clone(int v) {
        tree.push_back(tree[v]);
        return (int)tree.size() - 1;
    }

    int push(Line nw, int v, ll l = XMIN, ll r = XMAX) {
        if (nw.m == 0 && nw.c == -INF) return v;
        v = clone(v);

        // ld m = l + (r - l) / 2.0;
        ll m = l + (r - l) / 2 - ((l + r) < 0 && (l + r) % 2);
        bool left = nw.get(l) > tree[v].line.get(l);
        bool mid  = nw.get(m) > tree[v].line.get(m);

        if (mid) swap(tree[v].line, nw);
        // r - l < eps
        if (l == r) return v;

        if (left != mid) tree[v].lc = push(nw, tree[v].lc, l, m);
        else             tree[v].rc = push(nw, tree[v].rc, m + 1, r); // remove +1
        return v;
    }

    int add_node(Line nw, ll L, ll R, int v, ll l = XMIN, ll r = XMAX) {
        // doubles comp
        if (l > R || r < L) return v;
        if (L <= l && r <= R) return push(nw, v, l, r);

        v = clone(v);
        // ld m = l + (r - l) / 2.0;
        ll m = l + (r - l) / 2 - ((l + r) < 0 && (l + r) % 2);
        tree[v].lc = add_node(nw, L, R, tree[v].lc, l, m);
        tree[v].rc = add_node(nw, L, R, tree[v].rc, m + 1, r); // remove +1
        return v;
    }

    // Branches from `ver`, adds line `nw` on [L, R], and returns the new version ID
    int add(int ver, Line nw, ll L = XMIN, ll R = XMAX) {
        int new_root = add_node(nw, L, R, roots[ver], XMIN, XMAX);
        roots.push_back(new_root);
        return (int)roots.size() - 1;
    }

    ll query_node(ll x, int v, ll l = XMIN, ll r = XMAX) const {
        if (!v) return -INF;
        ll curr = tree[v].line.get(x);
        // r - l < eps
        if (l == r) return curr;

        // ld m = l + (r - l) / 2.0;
        ll m = l + (r - l) / 2 - ((l + r) < 0 && (l + r) % 2);
        if (x <= m) return max(curr, query_node(x, tree[v].lc, l, m));
        else        return max(curr, query_node(x, tree[v].rc, m + 1, r)); // remove +1
    }

    ll query(int ver, ll x) const {
        return query_node(x, roots[ver], XMIN, XMAX);
    }
};