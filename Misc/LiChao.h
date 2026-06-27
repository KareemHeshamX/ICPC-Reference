const ll INF = 4e18;
const ll XMIN = 0, XMAX = 100005; // Global bounds

struct Line {
    ll m = 0, c = -INF;
    ll get(ll x) const { return m * x + c; }
};

struct LiChao {
    vector<Line> tree;

    LiChao() {
        tree.assign(4 * (XMAX - XMIN + 5), {});
    }

    void push(Line nw, int v = 1, ll l = XMIN, ll r = XMAX) {
        ll m = l + (r - l) / 2;
        bool left = nw.get(l) > tree[v].get(l);
        bool mid  = nw.get(m) > tree[v].get(m);

        if (mid) swap(tree[v], nw);
        if (l == r) return;

        if (left != mid) push(nw, 2 * v, l, m);
        else             push(nw, 2 * v + 1, m + 1, r);
    }

    void add(Line nw, ll L = XMIN, ll R = XMAX, int v = 1, ll l = XMIN, ll r = XMAX) {
        if (l > R || r < L) return;
        if (L <= l && r <= R) return push(nw, v, l, r);

        ll m = l + (r - l) / 2;
        add(nw, L, R, 2 * v, l, m);
        add(nw, L, R, 2 * v + 1, m + 1, r);
    }

    ll query(ll x, int v = 1, ll l = XMIN, ll r = XMAX) {
        ll curr = tree[v].get(x);
        if (l == r) return curr;

        ll m = l + (r - l) / 2;
        if (x <= m) return max(curr, query(x, 2 * v, l, m));
        else        return max(curr, query(x, 2 * v + 1, m + 1, r));
    }
};