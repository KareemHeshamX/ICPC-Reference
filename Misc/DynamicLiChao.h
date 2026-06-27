const ll INF = 2e18;
const ll XMIN = -2e9, XMAX = 2e9;

// ld instead of ll in everything except l, r, x if m or c only are doubles
struct Line {
    ll m = 0, c = -INF;
    ll get(ll x) const { return m * x + c; }
};
// REST OF THE CHANGES IN THE COMMENTS BELOW IS NEEDED IF THE QUERIES ALSO ARE DOUBLES

struct DynamicLiChao {
    struct Node { Line line; int lc = 0, rc = 0; };
    vector<Node> tree;

    DynamicLiChao() {
        tree.reserve(15000000);
        tree.push_back({});
        tree.push_back({});
    }

    int get_l(int v) {
        if (!tree[v].lc) { tree.push_back({}); tree[v].lc = tree.size() - 1; }
        return tree[v].lc;
    }
    int get_r(int v) {
        if (!tree[v].rc) { tree.push_back({}); tree[v].rc = tree.size() - 1; }
        return tree[v].rc;
    }

    void push(Line nw, int v = 1, ll l = XMIN, ll r = XMAX) {
        // ld m = l + (r - l) / 2.0;
        ll m = l + (r - l) / 2 - ((l + r) < 0 && (l + r) % 2);
        bool left = nw.get(l) > tree[v].line.get(l);
        bool mid  = nw.get(m) > tree[v].line.get(m);

        if (mid) swap(tree[v].line, nw);
        // r - l < eps
        if (l == r) return;

        if (left != mid) push(nw, get_l(v), l, m);
        else             push(nw, get_r(v), m + 1, r); // remove +1
    }

    void add(Line nw, ll L = XMIN, ll R = XMAX, int v = 1, ll l = XMIN, ll r = XMAX) {
        // doubles comp
        if (l > R || r < L) return;
        if (L <= l && r <= R) return push(nw, v, l, r);

        // ld m = l + (r - l) / 2.0;
        ll m = l + (r - l) / 2 - ((l + r) < 0 && (l + r) % 2);
        add(nw, L, R, get_l(v), l, m);
        add(nw, L, R, get_r(v), m + 1, r); // remove +1
    }

    ll query(ll x, int v = 1, ll l = XMIN, ll r = XMAX) {
        if (!v) return -INF;
        ll curr = tree[v].line.get(x);
        // r - l < eps
        if (l == r) return curr;

        // ld m = l + (r - l) / 2.0;
        ll m = l + (r - l) / 2 - ((l + r) < 0 && (l + r) % 2);
        if (x <= m) return max(curr, query(x, tree[v].lc, l, m));
        else        return max(curr, query(x, tree[v].rc, m + 1, r)); // remove +1
    }
};
