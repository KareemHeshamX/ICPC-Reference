class LCA {
public:
	int n, logN, root;
	vector<int> depth;
	vector<vector<int>> adj, lca;
	void dfs(int node, int parent) {
		lca[0][node] = parent;
		depth[node] = (~parent ? depth[parent] + 1 : 0);
		for (int k = 1; k <= logN; k++) {
			int up_parent = lca[k - 1][node];
			if (~up_parent) lca[k][node] = lca[k - 1][up_parent];
		}
		for (int child : adj[node])
			if (child != parent)
				dfs(child, node);
	}
	LCA(const vector<vector<int>> &_adj, int root = 1) : root(root), adj(_adj) {
		adj = _adj;
		n = adj.size() - 1;
		logN = log2(n);
		lca = vector<vector<int>>(logN + 1, vector<int>(n + 1, -1));
		depth = vector<int>(n + 1);
		dfs(root, -1);
	}
	int get_LCA(int x, int y) {
		if (depth[x] < depth[y]) swap(x, y);
		for (int k = logN; k >= 0; k--)
			if (depth[x] - (1 << k) >= depth[y]) x = lca[k][x];
		if (x == y) return x;
		for (int k = logN; k >= 0; k--) {
			if (lca[k][x] != lca[k][y]) {
				x = lca[k][x];
				y = lca[k][y];
			}
		}
		return lca[0][x];
	}
	int get_distance(int u, int v) {
		return depth[u] + depth[v] - 2 * depth[get_LCA(u, v)];
	}
	//Do you even lift, bro?
	int kth_ancestor(int node, int dist) {
		for (int i = logN; i >= 0 && ~node; i--)
			if (dist & (1 << i)) node = lca[i][node];
		return node;
	}
};