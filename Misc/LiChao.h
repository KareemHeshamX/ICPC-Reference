#define ll long long

const int LC_N = (int)1e6 + 1;
const ll LC_INF = (ll)1e17;
vector<array<ll,2>> lc_tree(4 * LC_N, {0, LC_INF});

ll f_line(const array<ll,2>& line, int x) {
    return line[0] * x + line[1];
}

void lc_insert(array<ll,2> line, int lo = 1, int hi = LC_N, int i = 1) {
    int m = (lo + hi) / 2;
    bool left = f_line(line, lo) < f_line(lc_tree[i], lo);
    bool mid  = f_line(line, m)  < f_line(lc_tree[i], m);

    if (mid) swap(lc_tree[i], line);
    if (hi - lo == 1) return;

    if (left != mid)
        lc_insert(line, lo, m, 2 * i);
    else
        lc_insert(line, m, hi, 2 * i + 1);
}

ll lc_query(int x, int lo = 1, int hi = LC_N, int i = 1) {
    int m = (lo + hi) / 2;
    ll curr = f_line(lc_tree[i], x);

    if (hi - lo == 1) return curr;

    if (x < m)
        return min(curr, lc_query(x, lo, m, 2 * i));
    else
        return min(curr, lc_query(x, m, hi, 2 * i + 1));
}