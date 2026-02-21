template <class T, class F = function<T(const T&, const T&)>>
struct segment_tree {
    int n;
    vector<T> tree;
    T unit; F f;

    segment_tree() {}
    segment_tree(int _n, T _unit, F _f) : n(1 << (__bit_width(_n))), tree(2 * n, _unit), unit(_unit), f(_f) {}

    void update(int k, T x) {
        (tree[k += n] *= x) %= mod;
        for (k /= 2; k >= 1; k /= 2) {
            tree[k] = f(tree[2 * k], tree[2 * k + 1]);
        }
    }

    T query(int l, int r) {
        T ansl = unit, ansr = unit;
        for (l += n, r += n; l <= r; l /= 2, r /= 2) {
            if (l & 1) ansl = f(ansl, tree[l++]);
            if (!(r & 1)) ansr = f(tree[r--], ansr);
        }
        return f(ansl, ansr);
    }
};