struct SuffixArray {
    const static int alpha = 128, LOG = 20;
    vector<int> suf, order, newOrder, lcp, logs;
    vector<vector<int>> table;
    string s;
    int n;

    SuffixArray(const string& _s) : n(sz(_s) + 1), s(_s) {
        s += ' ';
        suf = order = newOrder = vector<int>(n);
        vector<int> bucket_idx(n), newOrder(n), new_suf(n);
        vector<int> prev(n), head(alpha, -1);

        auto getOrder = [&](const int& a) -> int {
            return a < n ? order[a] : 0;
        };

        for (int i = 0; i < n; i++) {
            prev[i] = head[s[i]];
            head[s[i]] = i;
        }
        for (int i = 0, buc = -1, idx = 0; i < alpha; i++) {
            if(head[i] == -1) continue;
            bucket_idx[++buc] = idx;
            for (int j = head[i]; ~j; j = prev[j]){
                suf[idx++] = j; order[j] = buc;
            }
        }

        for (int len = 1; order[suf[n - 1]] != n - 1; len <<= 1) {
            auto comp = [&](const int &a, const int &b) -> bool {
                if (order[a] != order[b]) return order[a] < order[b];
                return getOrder(a + len) < getOrder(b + len);
            };
            for (int i = 0; i < n; i++) {
                int j = suf[i] - len;
                if(j < 0) continue;
                new_suf[bucket_idx[order[j]]++] = j;
            }
            for(int i = 1; i < n; i++){
                suf[i] = new_suf[i];
                bool newGroup = comp(suf[i - 1], suf[i]);
                newOrder[suf[i]] = newOrder[suf[i - 1]] + newGroup;
                if(newGroup){
                    bucket_idx[newOrder[suf[i]]] = i;
                }
            }
            order = newOrder;
        }

        lcp = vector<int>(n);
        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int pos = order[i];
            int j = suf[pos - 1];
            while (s[i + k] == s[j + k]) k++;
            lcp[pos] = k;
            k = max(0, k - 1);
        }
        buildTable();
    }

    void buildTable() {
        table = vector<vector<int>>(n + 1, vector<int>(LOG));
        logs = vector<int>(n + 1);
        logs[1] = 0;
        for (int i = 2; i <= n; i++)
            logs[i] = logs[i >> 1] + 1;
        for (int i = 0; i < n; i++) {
            table[i][0] = lcp[i];
        }
        for (int j = 1; j <= logs[n]; j++) {
            for (int i = 0; i <= n - (1 << j); i++) {
                table[i][j] = min(table[i][j - 1], table[i + (1 << (j - 1))][j - 1]);
            }
        }
    }

    int LCP(int i, int j) {
        if (i == j) return n - i - 1;
        int l = order[i], r = order[j];
        if (l > r) swap(l, r);
        l++;
        int sz = logs[r - l + 1];
        return min(table[l][sz], table[r - (1 << sz) + 1][sz]);
    }

    int LCP_By_Order(int l, int r) {
        if (l == r) return n - l - 1;
        if (l > r) swap(l, r);
        l++;
        int sz = logs[r - l + 1];
        return min(table[l][sz], table[r - (1 << sz) + 1][sz]);
    }

    int compare_substrings(int l1, int r1, int l2, int r2) {
        int k = min({LCP(l1, l2), r1 - l1 + 1, r2 - l2 + 1});
        l1 += k; l2 += k;
        if (l1 > r1 && l2 > r2) return 0;
        if (l1 > r1) return -1;
        if (l2 > r2) return 1;
        return (s[l1] > s[l2] ? 1 : -1);
    }
};

ll number_of_different_substrings(string s) {
    int n = s.size();
    SuffixArray sa(s);
    ll cnt = 0;
    for (int i = 0; i <= n; i++)
        cnt += n - sa.suf[i] - sa.lcp[i];
    return cnt;
}

string longest_common_substring(const string &s1, const string &s2) {
    SuffixArray sa(s1 + "#" + s2);
    vector<int> suf = sa.suf, lcp = sa.lcp;
    auto type = [&](int idx) {
        return idx <= s1.size();
    };
    int mx = 0, idx = 0;
    int len = s1.size() + 1 + s2.size();
    for (int i = 1; i <= len; i++)
        if (type(suf[i - 1]) != type(suf[i]) && lcp[i] > mx) {
            mx = lcp[i];
            idx = min(suf[i - 1], suf[i]);
        }
    return s1.substr(idx, mx);
}

int longest_common_substring(const vector<string> &v) {
    int n = v.size();
    int len = n - 1;
    for (auto &it : v)
        len += it.size();
    string s(len, '.');
    vector<int> type(len + 1, n), frq(n + 1);
    for (int i = 0, j = 0; i < v.size(); i++) {
        if (i)
            s[j] = 'z' + i;
        for (char ch : v[i]) {
            s[j] = ch;
            type[j] = i;
            j++;
        }
    }
    SuffixArray sa(s);
    vector<int> suf = sa.suf, lcp = sa.lcp;
    monoqueue q;
    int st = 0, ed = 0, cnt = 0, mx = 0;
    while (st <= s.size()) {
        while (ed <= s.size() && cnt < v.size()) {
            q.push(lcp[ed], ed);
            if (++frq[type[suf[ed]]] == 1)
                cnt++;
            ed++;
        }
        q.pop(st);
        if (cnt == v.size())
            mx = max(mx, q.getMin()); //st+1,ed
        if (--frq[type[suf[st]]] == 0)
            cnt--;
        st++;
    }
    return mx;
}

string kth_substring(string s, int k) {	//1-based,repated
    int n = s.size();
    SuffixArray sa(s);
    vector<int> suf = sa.suf, lcp = sa.lcp;
    for (int i = 1; i <= n; i++) {
        int len = n - suf[i];
        int cnt = 0;
        for (int l = 1; l <= len; l++) {
            cnt++;
            int st = i + 1, ed = n, ans = i;
            while (st <= ed) {
                int md = st + ed >> 1;
                if (sa.LCP_By_Order(i, md) >= l)
                    st = md + 1, ans = md;
                else
                    ed = md - 1;
            }
            cnt += ans - i;
            if (cnt >= k)
                return s.substr(suf[i], l);
        }
        k -= len;
    }
}