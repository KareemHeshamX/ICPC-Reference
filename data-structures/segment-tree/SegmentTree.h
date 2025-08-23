template <typename T>
struct SegTree {
    int n;
    vector<T> tree;
    function<T(const T&, const T&)> merge;
    T neutral;

    // Constructor
    SegTree(int _n,
                function<T(const T&, const T&)> _merge,
                T _neutral
                )
        : n(_n), merge(_merge), neutral(_neutral) {
        tree.assign(4 * n, neutral);
    }

    // Build from initial vector
    void build(const vector<T>& v) {
        build(1, 0, n - 1, v);
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

    void update(int idx, int L, int R, int pos, const T& val) {
        if (L == R) {
            tree[idx] = val;
            return;
        }
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
        int mid = (L + R) >> 1;
        T leftRes = query(idx << 1, L, mid, l, r);
        T rightRes = query(idx << 1 | 1, mid + 1, R, l, r);
        return merge(leftRes, rightRes);
    }
};