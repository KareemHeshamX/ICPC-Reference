int n, timer = 0;
vector<int>v, vis, low, dfsn;
vector<vector<int>>adj, scc;
stack<int>stk;


void tarjan(int u) {
    vis[u] = 1;
    dfsn[u] = low[u] = timer++;
    stk.push(u);
    for (auto it:adj[u]) {
        if (!vis[it]) {
            tarjan(it);
            low[u] = min(low[u], low[it]);
        }
        else if (vis[it] == 1) {
            low[u] = min(low[u], low[it]);
        }
    }
    if (dfsn[u] == low[u]) {
        scc.push_back(vector<int>());
        while (true) {
            int x = stk.top(); stk.pop();
            vis[x] = 2;
            scc.back().push_back(v[x]);
            if (x == u) break;
        }
    }
}