const int INF = 1e9;

struct Dinic {
    struct flowEdge {
        int to, rev;      // rev = index of the reverse edge in adj[to]
        ll cap, flow = 0;
        flowEdge(int to, int rev, ll cap) :
                to(to), rev(rev), cap(cap) {
        }
    };
    int n, source, sink;
    vector<vector<flowEdge>> adj;
    vector<int> level, ptr;
    Dinic(int n, int source, int sink) :
            n(n), source(source), sink(sink), adj(n), level(n), ptr(n) {
    }
    // revCap = capacity of the reverse edge (0 for directed, cap for undirected)
    void addEdge(int u, int v, ll cap, ll revCap = 0) {
        int idU = adj[u].size();
        int idV = adj[v].size() + (u == v);   // handles self-loops safely
        adj[u].emplace_back(v, idV, cap);
        adj[v].emplace_back(u, idU, revCap);
    }
    bool bfs() {
        queue<int> q;
        level.assign(n, -1);
        level[source] = 0;
        q.push(source);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (auto &e : adj[cur]) {
                if (e.cap - e.flow <= 0)
                    continue;
                if (level[e.to] != -1)
                    continue;
                level[e.to] = level[cur] + 1;
                q.push(e.to);
            }
        }
        return level[sink] != -1;
    }
    ll dfs(int node, ll cur_flow) {
        if (cur_flow == 0 || node == sink)
            return cur_flow;
        for (int &cid = ptr[node]; cid < (int)adj[node].size(); cid++) {
            flowEdge &e = adj[node][cid];
            if (level[node] + 1 != level[e.to] || e.cap - e.flow <= 0)
                continue;
            ll tmp = dfs(e.to, min(cur_flow, e.cap - e.flow));
            if (tmp == 0)
                continue;
            e.flow += tmp;
            adj[e.to][e.rev].flow -= tmp;
            return tmp;
        }
        return 0;
    }
    ll flow() {
        ll max_flow = 0;
        while (bfs()) {
            fill(ptr.begin(), ptr.end(), 0);
            while (ll pushed = dfs(source, INF))
                max_flow += pushed;
        }
        return max_flow;
    }
};