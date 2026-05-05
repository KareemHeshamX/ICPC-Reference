template<typename T>
struct BIT_Max {
	int n;
	vector<T> fen;
	T INF;

	// Automatically sets INF to the minimum value of type T (e.g., -2^31 for int)
	BIT_Max(int _n, T _INF = numeric_limits<T>::lowest())
		: n(_n), fen(_n + 1, _INF), INF(_INF) {}

	void update(int p, T x) {
		for (; p <= n; p += p & -p)
			fen[p] = max(fen[p], x);
	}

	T query(int p) {
		T res = INF;
		for (; p >= 1; p -= p & -p)
			res = max(res, fen[p]);
		return res;
	}
};