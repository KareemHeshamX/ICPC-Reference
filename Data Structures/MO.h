struct MO {
	int n, sq;
	int curAns;
	MO(int n_) : n(n_), sq((int)sqrt(n_)) {}
	struct query {
		int l, r, qIdx, blockId;
		query(int l_, int r_, int id_, int sq_) :
			l(l_), r(r_), qIdx(id_), blockId(l_ / sq_) {}
		bool operator<(const query &other) {
			if (blockId != other.blockId) return blockId < other.blockId;
			return (blockId & 1 ? r < other.r : r > other.r);
		}
	};
	vector<query> Q;
	void addQuery(int l, int r, int id) {
		Q.emplace_back(l, r, id, sq);
	}
	void add(int idx) {}
	void remove(int idx) {}
	int curL, curR;
	void solve_query(int l, int r) {
		while (curL > l) add(--curL);
		while (curR < r) add(++curR);
		while (curL < l) remove(curL++);
		while (curR > r) remove(curR--);
	}
	vector<int> process() {
		vector<int> rt(Q.size());
		sort(Q.begin(), Q.end());
		curL = 1, curR = 0, curAns = 0;
		for (auto &it : Q) {
			solve_query(it.l, it.r);
			rt[it.qIdx] = curAns;
		}
		return rt;
	}
};
