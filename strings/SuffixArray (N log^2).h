string s;
int n, len;
vector<int>order, newOrder, suf, lcp;

bool comp(int a, int b) {
    return order[a] < order[b] ||
        order[a] == order[b] && order[a + len] < order[b + len];
}

void build() {
    s.push_back('@');
    n = sz(s);
    order = newOrder = lcp = suf = vector<int>(n);
    for (int i = 0; i < n; i++) order[i] = s[i], suf[i] = i;
    for (len = 1; ; len *= 2) {
        sort(suf.begin(), suf.end(), comp);
        for (int i = 1; i < n; i++) newOrder[i] = newOrder[i - 1] + comp(suf[i - 1], suf[i]);
        for (int i = 0; i < n; i++) order[suf[i]] = newOrder[i];
        if (newOrder[n - 1] == n - 1) break;
    }
}

void buildLCP() {
    lcp = vector<int>(n);
    int k = 0;
    for (int i = 0; i < n; i++) {
        int pos = order[i] - 1;
        if (!pos) {
            k = 0;
            lcp[0] = 0;
            continue;
        }
        int j = suf[pos - 1];
        while (s[i + k] == s[j + k]) k++;
        lcp[pos] = k;
        if (k) k--;
    }
}

int getLCP(int a, int b, sparse_table<int> &sp) {
    a = order[a] - 1, b = order[b] - 1;
    if (a > b) swap(a, b);
    return sp.query(a + 1, b);
}

bool comp2(array<int, 2>a, array<int, 2>b, sparse_table<int> &sp) {
    int com = getLCP(a[0], b[0], sp);
    int sz1 = a[1] - a[0] + 1, sz2 = b[1] - b[0] + 1;
    com = min(com, min(sz1, sz2));
    if (a[0] + com > a[1]) return (sz2 > sz1);
    if (b[0] + com > b[1]) return false;
    return s[a[0] + com] < s[b[0] + com];
}