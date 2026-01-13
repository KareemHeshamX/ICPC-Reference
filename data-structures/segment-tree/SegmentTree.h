struct SegTree {
    struct Node {
        int val;
        Node(){}
        // constructor here
        Node (int _val) : val(_val) {
        }
        Node operator+(Node &other) const {
            Node ret;
            // merge function here
            ret.val = max(val,  other.val);
            return ret;
        }
    };
    int n;
    vector<Node> tree;
    Node neutral;

    // Constructor
    SegTree(int _n) : n(_n) {
        // initialize neutal
        neutral.val = 0;
        tree.assign(4 * n, neutral);
    }

    void build(vector<int>&v) {
        build(1, 0, n - 1, v);
    }

    // Point update: set position pos to value val
    void update(int pos, const int val) {
        update(1, 0, n - 1, pos, val);
    }

    // Range query [l..r]
    Node query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

private:

    void build(int idx, int L, int R, vector<int>&v) {
        if (L == R) {
            tree[idx].val = v[L];
            return;
        }
        int mid = (L + R) >> 1;
        build(idx << 1, L, mid, v);
        build(idx << 1 | 1, mid + 1, R, v);
        tree[idx] = tree[idx << 1] + tree[idx << 1 | 1];
    }

    void update(int idx, int L, int R, int pos, const int val) {
        if (L == R) {
            tree[idx] = val;
            return;
        }
        int mid = (L + R) >> 1;
        if (pos <= mid) update(idx << 1, L, mid, pos, val);
        else update(idx << 1 | 1, mid + 1, R, pos, val);
        tree[idx] = tree[idx << 1] + tree[idx << 1 | 1];
    }

    Node query(int idx, int L, int R, int l, int r) {
        if (r < L || R < l) return neutral;
        if (l <= L && R <= r) return tree[idx];
        int mid = (L + R) >> 1;
        Node leftRes = query(idx << 1, L, mid, l, r);
        Node rightRes = query(idx << 1 | 1, mid + 1, R, l, r);
        return leftRes + rightRes;
    }
};