template<typename T>
struct BIT {
	int n;
	vector<T> fen1, fen2;

	BIT(int _n) : n(_n), fen1(_n + 1, T(0)), fen2(_n + 1, T(0)) {}

	void add(vector<T>& fen, int p, T x) {
		for (; p <= n; p += p & -p)
			fen[p] += x;
	}

	void range_add(int l, int r, T x) {
		add(fen1, l, x);
		add(fen1, r + 1, -x);
		add(fen2, l, x * (T)(l - 1));
		add(fen2, r + 1, -x * (T)r);
	}

	T pre(int p) {
		T s1 = T(0), s2 = T(0);
		for (int i = p; i >= 1; i -= i & -i) {
			s1 += fen1[i];
			s2 += fen2[i];
		}
		return s1 * (T)p - s2;
	}

	T query(int l, int r) {
		return pre(r) - pre(l - 1);
	}
};