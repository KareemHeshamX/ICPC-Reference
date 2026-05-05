template<typename T>
struct sparse_table {
	vector<vector<T>> sparseTable;
	T neutral;
	using F = function<T(T,T)>;
	F merge;
	static int LOG2(int x) {
		return 31 - __builtin_clz(x);
	}
	sparse_table(vector<T> &v, F _merge, T _neutral) : merge(_merge), neutral(_neutral) {
		int n = v.size();
		int logN = LOG2(n);
		sparseTable = vector < vector < T >> (logN + 1);
		sparseTable[0] = v;
		for (int k = 1, len = 1; k <= logN; k++, len <<= 1) {
			sparseTable[k].resize(n);
			for (int i = 0; i + len < n; i++)
				sparseTable[k][i] = merge(sparseTable[k - 1][i],
						sparseTable[k - 1][i + len]);
		}
	}
	T query(int l, int r) {
		int k = LOG2(r - l + 1);
		return merge(sparseTable[k][l], sparseTable[k][r - (1 << k) + 1]);
	}
	T query2(int l, int r) {
		T ret = neutral;
		for (int i = 0; i <= LOG2(r - l + 1); i++) {
			if (((r - l + 1) >> i) & 1) {
				ret = merge(ret, sparseTable[i][l]);
				l += (1 << i);
			}
		}
		return ret;
	}
	T query_shifting(int l, int r) {
		T res;
		bool first = true;
		for (int i = (int) sparseTable.size() - 1; i >= 0; i--)
			if (l + (1 << i) - 1 <= r) {
				if (first)
					res = sparseTable[i][l];
				else
					res = merge(res, sparseTable[i][l]);
				first = false;
				l += (1 << i);
			}
		return res;
	}
};