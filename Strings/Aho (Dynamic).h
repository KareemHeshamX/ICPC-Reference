#include <bits/stdc++.h>
using namespace std;

using ll = int_fast64_t;
using ld = long double;
#define sz(s) (int)(s.size())

int dx[] = {0, 0, 1, -1, 1, 1, -1, -1};
int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};

template<typename T>
struct BIT {
    int n;
    vector<T> fen1, fen2;

    BIT(int _n) : n(_n), fen1(_n + 1, T(0)), fen2(_n + 1, T(0)) {}

    void add(vector<T>& fen, int p, T x) {
        for (; p <= n; p += p & -p)
            fen[p] += x;
    }

    void range_add(int l, int r, T x) {
        add(fen1, l, x);
        add(fen1, r + 1, -x);
        add(fen2, l, x * (T)(l - 1));
        add(fen2, r + 1, -x * (T)r);
    }

    T pre(int p) {
        T s1 = T(0), s2 = T(0);
        for (int i = p; i >= 1; i -= i & -i) {
            s1 += fen1[i];
            s2 += fen2[i];
        }
        return s1 * (T)p - s2;
    }

    T query(int l, int r) {
        return pre(r) - pre(l - 1);
    }
};

BIT<ll>fen(1000005);

struct AhoCorasick {
    static const int alpha = 26;
    char offset = 'a';
    bool built = false;

    struct Node {
        array<int, alpha> nxt{};
        int fail = 0, idx = -1, up = 0;
        Node() { nxt.fill(-1); }
    };

    vector<Node> trie{Node()};
    vector<int> pat_len, in, out;
    vector<vector<int>> adj;

    int add_pattern(const string &s) {
        int cur = 0;
        for (auto &ch: s) {
            int c = ch - offset;
            if (trie[cur].nxt[c] == -1) {
                trie[cur].nxt[c] = (int) trie.size();
                trie.emplace_back();
            }
            cur = trie[cur].nxt[c];
        }
        if (trie[cur].idx == -1) {
            trie[cur].idx = sz(pat_len);
            pat_len.push_back(sz(s));
        }
        return cur;
    }

    int compF(int u, int c) {
        while (trie[u].nxt[c] == -1) u = trie[u].fail;
        return trie[u].nxt[c];
    }

    int getNxt(int u) {
        if (!u) return u;
        int &v = trie[u].up;
        return (~trie[v].idx ? v : v = getNxt(v));
    }

    void buildAhoTree() {
        queue<int> q;
        for (int c = 0; c < alpha; ++c) {
            int u = trie[0].nxt[c];
            if (u == -1) trie[0].nxt[c] = 0;
            else {
                trie[u].fail = 0;
                q.push(u);
            }
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int c = 0; c < alpha; ++c) {
                int v = trie[u].nxt[c];
                if (v == -1) continue;
                trie[v].up = trie[v].fail = compF(trie[u].fail, c);
                q.push(v);
            }
        }
    }

    void buildFailTree(){
        adj = vector<vector<int>>(sz(trie));
        in = out = vector<int>(sz(trie));
        for(int v = 1; v < sz(trie); ++v){
            adj[getNxt(v)].push_back(v);
        }
        int curTime = -1;
        auto dfs = [&](auto& self, int u) -> void{
            in[u] = ++curTime;
            for(auto& v : adj[u]) self(self, v);
            out[u] = curTime;
        };
        dfs(dfs, 0);
    }

    ll match(const string &s) {
        if (!built) {
            buildAhoTree();
            buildFailTree();
            built = true;
        }
        int cur = 0;
        ll ans = 0;
        for (int i = 0; i < sz(s); ++i) {
            int c = s[i] - offset;
            cur = compF(cur, c);
            for (int u = cur; u; u = getNxt(u)) {
                if (trie[u].idx != -1) {
                    ans += fen.query(in[u], in[u]);
                    break;
                }
            }
        }
        return ans;
    }
};

void testCase() {
    int q, n; cin >> q >> n;
    AhoCorasick aho;
    vector<int>id(n);
    vector<string>pat(n);
    for (int i = 0; i < n; i++) {
        cin >> pat[i];
        id[i] = aho.add_pattern(pat[i]);
    }
    aho.buildAhoTree();
    aho.built = true;
    aho.buildFailTree();
    for (int i = 0; i < n; i++) {
        fen.range_add(aho.in[id[i]], aho.out[id[i]], 1);
    }
    vector<bool>vis(n, true);
    for (int i = 0; i < q; i++) {
        char c; cin >> c;
        if (c == '+') {
            int idx; cin >> idx;
            idx--;
            if (!vis[idx]) {
                fen.range_add(aho.in[id[idx]], aho.out[id[idx]], 1);
                vis[idx] = true;
            }
        }
        else if (c == '-') {
            int idx; cin >> idx;
            idx--;
            if (vis[idx]) {
                fen.range_add(aho.in[id[idx]], aho.out[id[idx]], -1);
                vis[idx] = false;
            }
        }
        else {
            string s; cin >> s;
            cout << aho.match(s) << '\n';
        }
    }
}

signed main(int argc, char** argv) {
    cin.tie(0)->sync_with_stdio(0);
#ifdef LOCAL
    freopen(argv[1], "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    int t = 1;
    // cin >> t;
    while (t--) testCase();
    return 0;
}