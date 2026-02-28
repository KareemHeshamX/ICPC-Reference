template <class T, class F = function<T(const T&, const T&)>>
struct segment_tree {
    int n;
    vector<T> tree;
    T neutral;
    F merge;

    segment_tree() {}
    segment_tree(int _n, F _merge, T _neutral) : n(1 << (__bit_width(_n))), tree(2 * n, _neutral), neutral(_neutral), merge(_merge) {}

    void update(int k, T x) {
        (tree[k += n] = x);
        for (k /= 2; k >= 1; k /= 2) {
            tree[k] = merge(tree[2 * k], tree[2 * k + 1]);
        }
    }

    T query(int l, int r) {
        T ansl = neutral, ansr = neutral;
        for (l += n, r += n; l <= r; l /= 2, r /= 2) {
            if (l & 1) ansl = merge(ansl, tree[l++]);
            if (!(r & 1)) ansr = merge(tree[r--], ansr);
        }
        return merge(ansl, ansr);
    }
};