//Parallel DSU (DSU on sparse table), range union with equal shift
//unite_range(a, b, len): unite a+i with b+i for all 0 <= i < len
//O((N log N + Q) * alpha(N)) time, O(N log N) memory
struct Parallel_DSU {//0-based
    int n, LOG;
    vector<vector<int>> p;//p[k][i]: block [i, i+2^k); negative = root, value = -size

    Parallel_DSU(int n) :n(n), LOG(32 - __builtin_clz(n)), p(LOG) {
        for (int k = 0; k < LOG; k++)
            p[k].assign(n - (1 << k) + 1, -1);
    }
    int find(int k, int x) {//depth is O(log n) thanks to union by size
        return p[k][x] < 0 ? x : p[k][x] = find(k, p[k][x]);
    }
    bool merge(int k, int a, int b) {//flat union on level k only
        a = find(k, a), b = find(k, b);
        if (a == b)return false;
        if (p[k][a] > p[k][b])swap(a, b);
        p[k][a] += p[k][b];
        p[k][b] = a;
        return true;
    }

    //---------- online version ----------
    void unite(int k, int a, int b) {//merge at level k and push down
        if (!merge(k, a, b))return;
        if (k) {
            int h = 1 << (k - 1);
            unite(k - 1, a, b);
            unite(k - 1, a + h, b + h);
        }
    }
    void unite_range(int a, int b, int len) {
        if (a == b || len <= 0)return;
        int k = 31 - __builtin_clz(len);
        unite(k, a, b);
        unite(k, a + len - (1 << k), b + len - (1 << k));
    }

    //---------- offline version ----------
    void add_lazy(int a, int b, int len) {//touches level k only
        if (a == b || len <= 0)return;
        int k = 31 - __builtin_clz(len);
        merge(k, a, b);
        merge(k, a + len - (1 << k), b + len - (1 << k));
    }
    void push_all() {//call once after all add_lazy
        for (int k = LOG - 1; k >= 1; k--) {
            int h = 1 << (k - 1);
            for (int i = 0; i + (1 << k) <= n; i++) {
                int r = find(k, i);
                if (r != i) {
                    merge(k - 1, i, r);
                    merge(k - 1, i + h, r + h);
                }
            }
        }
    }

    //---------- queries (level 0) ----------
    int root(int x) { return find(0, x); }
    bool same(int a, int b) { return find(0, a) == find(0, b); }
    int components() {
        int c = 0;
        for (int i = 0; i < n; i++)c += (p[0][i] < 0);
        return c;
    }
};

//Online
Parallel_DSU d(n);
while (q--) { int a, b, len; cin >> a >> b >> len; d.unite_range(a, b, len); }
int comps = d.components();         // e.g. answer = 26^comps

//Offline
Parallel_DSU d(n);
while (q--) { int a, b, len; cin >> a >> b >> len; d.add_lazy(a, b, len); }
d.push_all();
// now level 0 is final: d.same(i, j), d.root(i), d.components()