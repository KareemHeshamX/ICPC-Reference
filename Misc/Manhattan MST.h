//Manhattan MST
//O(N log N): builds at most 4N candidate edges, then runs Kruskal on them
struct Manhattan_MST {//0-based points
	typedef long long ll;
	struct Edge { ll w; int u, v; };
	int n;
	vector<ll> x, y;
	vector<Edge> mst;//edges of the resulting MST
	vector<int> par;

	Manhattan_MST(const vector<ll>& x, const vector<ll>& y)
		:n(x.size()), x(x), y(y), par(x.size()) {
	}
	int find(int a) { return par[a] == a ? a : par[a] = find(par[a]); }

	ll run() {//returns total MST weight
		const ll INF = LLONG_MAX;
		vector<ll> px = x, py = y;
		vector<Edge> cand;
		vector<int> id(n);
		iota(id.begin(), id.end(), 0);

		for (int dir = 0; dir < 4; dir++) {
			if (dir == 1 || dir == 3) swap(px, py);
			else if (dir == 2) for (auto& v : px) v = -v;

			//process by x descending (ties: y descending)
			sort(id.begin(), id.end(), [&](int a, int b) {
				return px[a] != px[b] ? px[a] > px[b] : py[a] > py[b];
			});

			//compress key = y - x
			vector<ll> key(n);
			for (int i = 0; i < n; i++) key[i] = py[i] - px[i];
			vector<ll> vals = key;
			sort(vals.begin(), vals.end());
			vals.erase(unique(vals.begin(), vals.end()), vals.end());
			int K = vals.size();

			//BIT: prefix-min of (x + y, index); larger key -> smaller position
			vector<pair<ll, int>> bit(K + 1, { INF, -1 });
			for (int i : id) {
				int pos = K - (lower_bound(vals.begin(), vals.end(), key[i]) - vals.begin());
				pair<ll, int> best = { INF, -1 };
				for (int j = pos; j > 0; j -= j & -j) best = min(best, bit[j]);
				if (best.second != -1) {
					int o = best.second;
					cand.push_back({ llabs(px[i] - px[o]) + llabs(py[i] - py[o]), i, o });
				}
				pair<ll, int> cur = { px[i] + py[i], i };
				for (int j = pos; j <= K; j += j & -j) bit[j] = min(bit[j], cur);
			}
		}

		//Kruskal on candidate edges
		sort(cand.begin(), cand.end(), [](const Edge& a, const Edge& b) { return a.w < b.w; });
		iota(par.begin(), par.end(), 0);
		mst.clear();
		ll total = 0;
		for (auto& e : cand) {
			int a = find(e.u), b = find(e.v);
			if (a != b) {
				par[a] = b;
				total += e.w;
				mst.push_back(e);
			}
		}
		return total;
	}
};