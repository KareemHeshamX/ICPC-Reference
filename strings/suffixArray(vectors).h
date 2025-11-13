const int LOG = 20;
struct SuffixArray {
    vector<int> suf, order, newOrder, lcp, logs;
    vector<vector<int>> table;
    vector<int> s;
    int n;

    SuffixArray(const vector<int>& _s) : n(sz(s) + 1), s(_s) {
        s.push_back(-1);
        suf = order = newOrder = vector<int>(n);
        {
            vector<pair<int, int>> temp;
            for (int i = 0; i < sz(s); i++) temp.emplace_back(s[i], i);
            ranges::sort(temp);
            for (int i = 0; i < sz(s); i++) suf[i] = temp[i].second;
            order[suf[0]] = 0;
            for (int i = 1; i < sz(s); i++)
                order[suf[i]] = order[suf[i - 1]] + (int)(temp[i].first != temp[i - 1].first);
        }

        auto getOrder = [&](const int& a) -> int {
            if (a >= n) return 0;
            return order[a];
        };

        auto radix_sort = [&](const int& len) -> void {
            vector<int> f(n), new_suf(n);
            for (int i = 0; i < n; i++) f[getOrder(suf[i] + len)]++;
            for (int i = 1; i < n; i++) f[i] += f[i - 1];
            for (int i = n - 1; i >= 0; i--) new_suf[--f[getOrder(suf[i] + len)]] = suf[i];
            suf = new_suf;
        };

        for (int len = 1; newOrder.back() != n - 1; len <<= 1) {
            auto comp = [&](const int &a, const int &b) -> bool {
                if (order[a] == order[b]) return getOrder(a + len) < getOrder(b + len);
                return order[a] < order[b];
            };
            for (int i = 0; i < n; i++) {
                suf[i] -= len;
                if (suf[i] < 0) suf[i] += n;
            }
            radix_sort(0);
            for (int i = 1; i < sz(s); i++) newOrder[i] = newOrder[i - 1] + (int)comp(suf[i - 1], suf[i]);
            for (int i = 0; i < sz(s); i++) order[suf[i]] = newOrder[i];
        }
        buildLCP();
    }

    void buildLCP() {
        lcp = vector<int>(n);
        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int pos = order[i];
            int j = suf[pos - 1];
            while (s[i + k] == s[j + k]) k++;
            lcp[pos] = k;
            k = max(0, k - 1);
        }

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

    int compare_subtrings(int l1, int r1, int l2, int r2) {
        int k = min({LCP(l1, l2), r1 - l1 + 1, r2 - l2 + 1});
        l1 += k; l2 += k;
        if (l1 > r1 && l2 > r2) return 0;
        if (l1 > r1) return -1;
        if (l2 > r2) return 1;
        return (s[l1] > s[l2] ? 1 : -1);
    }
};
