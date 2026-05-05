template<typename T>
struct BIT {
	int n, m;
	vector<vector<T>> fen;

	BIT(int _n, int _m) : n(_n), m(_m), fen(_n + 1, vector<T>(_m + 1, T(0))) {}

	void add (int x, int y, T val) {
		for (int i = x; i <= n; i += i & -i)
			for (int j = y; j <= m; j += j & -j)
				fen[i][j] += val;
	}

	T pre (int x, int y) {
		T s = T(0);
		for (int i = x; i >= 1; i -= i & -i)
			for (int j = y; j >= 1; j -= j & -j)
				s += fen[i][j];
		return s;
	}

	T query (int x1, int y1, int x2, int y2) {
		return pre(x2, y2)
			 - pre(x1 - 1, y2)
			 - pre(x2, y1 - 1)
			 + pre(x1 - 1, y1 - 1);
	}
};