// T: type of values
// merge: function to combine two segments
// apply: function to apply lazy value to a segment node
// neutral: identity element for merge
// no_lazy: identity element for lazy values

template <typename T>
struct LazySegTree {
    int n;
    vector<T> tree, lazy;
    function<T(const T&, const T&)> merge;
    function<void(T&, const T&)> apply;
    T neutral, no_lazy;

    // Constructor
    LazySegTree(int _n,
                function<T(const T&, const T&)> _merge,
                T _neutral,
                function<void(T&, const T&)> _apply,
                T _no_lazy)
        : n(_n), merge(_merge), apply(_apply), neutral(_neutral), no_lazy(_no_lazy) {
        tree.assign(4 * n, neutral);
        lazy.assign(4 * n, no_lazy);
    }

    // Build from initial vector
    void build(const vector<T>& v) {
        build(1, 0, n - 1, v);
    }

    // Range update [l..r] with value val
    void update(int l, int r, const T& val) {
        update(1, 0, n - 1, l, r, val);
    }

    // Point update: set position pos to value val
    void update(int pos, const T& val) {
        update(1, 0, n - 1, pos, val);
    }

    // Range query [l..r]
    T query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

private:
    void build(int idx, int L, int R, const vector<T>& v) {
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
        apply(tree[left], lazy[idx]);
        apply(lazy[left], lazy[idx]);
        apply(tree[right], lazy[idx]);
        apply(lazy[right], lazy[idx]);
        // Clear current lazy
        lazy[idx] = no_lazy;
    }

    void update(int idx, int L, int R, int l, int r, const T& val) {
        if (r < L || R < l) return;
        if (l <= L && R <= r) {
            apply(tree[idx], val);
            apply(lazy[idx], val);
            return;
        }
        pushdown(idx, L, R);
        int mid = (L + R) >> 1;
        update(idx << 1, L, mid, l, r, val);
        update(idx << 1 | 1, mid + 1, R, l, r, val);
        tree[idx] = merge(tree[idx << 1], tree[idx << 1 | 1]);
    }

    void update(int idx, int L, int R, int pos, const T& val) {
        if (L == R) {
            tree[idx] = val;
            lazy[idx] = no_lazy;
            return;
        }
        pushdown(idx, L, R);
        int mid = (L + R) >> 1;
        if (pos <= mid)
            update(idx << 1, L, mid, pos, val);
        else
            update(idx << 1 | 1, mid + 1, R, pos, val);
        tree[idx] = merge(tree[idx << 1], tree[idx << 1 | 1]);
    }

    T query(int idx, int L, int R, int l, int r) {
        if (r < L || R < l) return neutral;
        if (l <= L && R <= r) return tree[idx];
        pushdown(idx, L, R);
        int mid = (L + R) >> 1;
        T leftRes = query(idx << 1, L, mid, l, r);
        T rightRes = query(idx << 1 | 1, mid + 1, R, l, r);
        return merge(leftRes, rightRes);
    }
};
