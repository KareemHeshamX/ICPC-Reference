using pii = pair<int,int>;
struct Centroid { // 1-based
	int n;
	vector<int> siz, rank;
	vector<vector<int>> adj;
	vector<vector<pair<int,int>>> centroids;

	Centroid(vector<vector<int>> &adj)
	: n(sz(adj)), adj(adj), siz(n), rank(n, n+1), centroids(n) {
		rank[0] = 0;
		decompose(1, 0);
	}

	int calcSize(int u, int p){
		siz[u] = 1;
		for(auto v : adj[u]){
			if(v == p || rank[v] < rank[u]) continue;
			siz[u] += calcSize(v, u);
		}
		return siz[u];
	}

	int getCentroid(int u, int p, int tsiz){
		for(auto v : adj[u]){
			if(v == p || rank[v] < rank[u]) continue;
			if(siz[v] * 2 > tsiz)   return getCentroid(v, u, tsiz);
		}
		return u;
	}

	void pushCentroid(int u, int p, int c, int d){
		centroids[u].emplace_back(c, d);
		for(auto v : adj[u]){
			if(v == p || rank[v] < rank[u]) continue;
			pushCentroid(v, u, c, d+1);
		}
	}

	void decompose(int u, int p){
		int subSize = calcSize(u, -1);
		u = getCentroid(u, -1, subSize);
		pushCentroid(u, -1, u, 0);
		rank[u] = rank[p] + 1;

		for(auto v : adj[u]){
			if(rank[v] < rank[u])   continue;
			decompose(v, u);
		}
	}
};