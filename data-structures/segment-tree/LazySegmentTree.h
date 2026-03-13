template<typename T1, typename T2>
struct LazySegTree {
    int n;
    vector<T1> tree;
    vector<T2> lazy;
    function<T1(const T1 &, const T1 &)> merge;
    function<void(T1 &, const T2 &, bool, int, int)> apply;
    T1 neutral;
    T2 no_lazy;

    // Constructor
    LazySegTree(int _n,
                function<T1(const T1 &, const T1 &)> _merge,
                T1 _neutral,
                function<void(T1 &, const T2 &, bool, int, int)> _apply,
                T2 _no_lazy)
        : n(_n), merge(_merge), apply(_apply), neutral(_neutral), no_lazy(_no_lazy) {
        tree.assign(4 * n, neutral);
        lazy.assign(4 * n, no_lazy);
    }

    // Build from initial vector
    void build(const vector<T1> &v) {
        build(1, 0, n - 1, v);
    }

    // Range update [l..r] with value val
    void update(int l, int r, const T2 &val) {
        update(1, 0, n - 1, l, r, val);
    }

    // Range query [l..r]
    T1 query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

private:
    void build(int idx, int L, int R, const vector<T1> &v) {
        if (L == R) {
            tree[idx] = v[L];
        } else {
            int mid = (L + R) >> 1;
            build(idx << 1, L, mid, v);
            build(idx << 1 | 1, mid + 1, R, v);
            tree[idx] = merge(tree[idx << 1], tree[idx << 1 | 1]);
        }
    }

    void pushdown(int idx, int L, int R) {
        if (lazy[idx] == no_lazy) return;
        int left = idx << 1, right = idx << 1 | 1;
        // Apply to children
        apply(tree[left], lazy[idx], true, L, (L + R) >> 1);
        apply(lazy[left], lazy[idx], false, L, (L + R) >> 1);
        apply(tree[right], lazy[idx], true, (((L + R) >> 1) + 1), R);
        apply(lazy[right], lazy[idx], false, (((L + R) >> 1) + 1), R);
        // Clear current lazy
        lazy[idx] = no_lazy;
    }

    void update(int idx, int L, int R, int l, int r, const T2 &val) {
        if (r < L || R < l) return;
        if (l <= L && R <= r) {
            apply(tree[idx], val, true, L, R);
            apply(lazy[idx], val, false, L, R);
            return;
        }
        pushdown(idx, L, R);
        int mid = (L + R) >> 1;
        update(idx << 1, L, mid, l, r, val);
        update(idx << 1 | 1, mid + 1, R, l, r, val);
        tree[idx] = merge(tree[idx << 1], tree[idx << 1 | 1]);
    }

    T1 query(int idx, int L, int R, int l, int r) {
        if (r < L || R < l) return neutral;
        if (l <= L && R <= r) return tree[idx];
        pushdown(idx, L, R);
        int mid = (L + R) >> 1;
        T1 leftRes = query(idx << 1, L, mid, l, r);
        T1 rightRes = query(idx << 1 | 1, mid + 1, R, l, r);
        return merge(leftRes, rightRes);
    }
};