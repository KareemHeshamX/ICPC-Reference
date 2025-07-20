int n, timer;
vector<int> dfsn, low;
vector<bool> isArt;
vector<array<int,2>> bridges;
vector<vector<int>> adj;

void tarjan(int u, int p) {
    dfsn[u] = low[u] = ++timer;
    int childCount = 0;

    for (int v: adj[u]) {
        if (!dfsn[v]) {
            ++childCount;
            tarjan(v, u);

            if (low[v] > dfsn[u]) {
                bridges.push_back({u, v});
            }
            if (p != -1 && low[v] >= dfsn[u]) {
                isArt[u] = true;
            }
            low[u] = min(low[u], low[v]);
        }
        else if (v != p) {
            low[u] = min(low[u], dfsn[v]);
        }
    }

    if (p == -1 && childCount > 1) {
        isArt[u] = true;
    }
}