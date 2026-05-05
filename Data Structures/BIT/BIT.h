template<typename T>
struct BIT {
    int n;
    vector<T> fen;

    BIT(int _n) : n(_n), fen(_n + 1, T(0)) {}

    void add(int p, T x) {
        for (; p <= n; p += p & -p)
            fen[p] += x;
    }

    T pre(int p) {
        T s = T(0);
        for (; p >= 1; p -= p & -p)
            s += fen[p];
        return s;
    }

    T query(int l, int r) {
        return pre(r) - pre(l - 1);
    }
};