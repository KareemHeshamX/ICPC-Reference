struct SqrtCount {
    int n, B; vector<int> cnt, blk;
    SqrtCount(int n) : n(n), B(sqrt(n) + 1), cnt(n + 1), blk(n / B + 2) {}
    void add(int p, int x) { cnt[p] += x; blk[p / B] += x; }
    int query(int l, int r) {                    // sum of cnt[l..r], both clamped
        l = max(l, 1), r = min(r, n);
        if (l > r) return 0;
        int res = 0;
        while (l <= r && l % B) res += cnt[l++];
        while (l + B - 1 <= r) res += blk[l / B], l += B;
        while (l <= r) res += cnt[l++];
        return res;
    }
};