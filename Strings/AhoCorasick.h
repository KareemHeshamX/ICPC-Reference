struct AhoCorasick {
    static const int alpha = 26;
    char offset = 'a';
    bool built = false;

    struct Node {
        array<int, alpha> child{}, nxt{};
        int fail = 0, idx = -1, up = 0, freq = 0;
        Node() {
            child.fill(-1);
            nxt.fill(0);
        }
    };

    vector<Node> trie{Node()};
    vector<int> pat_len;

    int add_pattern(const string& s) {
        int cur = 0;
        for (auto& ch : s) {
            int c = ch - offset;
            if (trie[cur].child[c] == -1) {
                trie[cur].child[c] = (int)trie.size();
                trie.emplace_back();
            }
            cur = trie[cur].child[c];
        }
        if (trie[cur].idx == -1) {
            trie[cur].idx = sz(pat_len);
            pat_len.push_back(sz(s));
        }
        trie[cur].freq++;
        return cur;
    }

    void buildAhoTree() {
        built = true;
        queue<int> q;
        for (int c = 0; c < alpha; ++c) {
            int u = trie[0].child[c];
            if (u != -1) {
                trie[0].nxt[c] = u;
                q.push(u);
            }
        }

        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int c = 0; c < alpha; ++c) {
                int v = trie[u].child[c];
                if (v != -1) {
                    trie[v].fail = trie[trie[u].fail].nxt[c];
                    trie[u].nxt[c] = v;

                    if (trie[trie[v].fail].idx != -1) trie[v].up = trie[v].fail;
                    else trie[v].up = trie[trie[v].fail].up;

                    q.push(v);
                }
                else trie[u].nxt[c] = trie[trie[u].fail].nxt[c];
            }
        }
    }

    vector<vector<int>> match(const string& s) {
        if (!built) buildAhoTree();
        vector<vector<int>> res(sz(pat_len));
        int cur = 0;

        for (int i = 0; i < sz(s); ++i) {
            int c = s[i] - offset;
            cur = trie[cur].nxt[c];
            int temp = (trie[cur].idx != -1) ? cur : trie[cur].up;

            while (temp > 0) {
                res[trie[temp].idx].push_back(i - pat_len[trie[temp].idx] + 1);
                temp = trie[temp].up;
            }
        }

        return res;
    }
};