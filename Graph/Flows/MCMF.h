struct MCMF {
    struct edge {
        int to, cap, flow;
        ll cost;
        int backEdge;
    };

    int n, src, sink;
    vector<vector<edge> > adj;
    vector<ll> pot, dis;
    vector<int> par, from_edge;

    // nodes are 0..n (inclusive), as in the original template
    MCMF(int n)
        : n(n), adj(n + 1), pot(n + 1), dis(n + 1), par(n + 1), from_edge(n + 1) {
    }

    void addEdge(int u, int v, int cap, ll cost) {
        adj[u].push_back({v, cap, 0, cost, (int) adj[v].size()});
        adj[v].push_back({u, 0, 0, - cost, (int) adj[u].size() - 1});
    }

    // Initial potentials via queue-based Bellman-Ford (SPFA) from src.
    // Only runs if some residual edge has negative cost.
    // Returns false if a negative cycle is reachable from src.
    bool initPotentials() {
        fill(pot.begin(), pot.end(), 0);
        bool neg = false;
        for (int u = 0; u <= n && !neg; ++u)
            for (edge &e : adj[u])
                if (e.flow < e.cap && e.cost < 0) { neg = true; break; }
        if (!neg)
            return true;
        fill(dis.begin(), dis.end(), LLONG_MAX);
        vector<int> cnt(n + 1, 0);
        vector<char> inq(n + 1, 0);
        deque<int> q;
        dis[src] = 0;
        q.push_back(src);
        inq[src] = 1;
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            inq[u] = 0;
            for (edge &e : adj[u]) {
                if (e.flow == e.cap)
                    continue;
                if (dis[e.to] > dis[u] + e.cost) {
                    dis[e.to] = dis[u] + e.cost;
                    if (!inq[e.to]) {
                        if (++cnt[e.to] > n + 1)
                            return false; // negative cycle
                        inq[e.to] = 1;
                        q.push_back(e.to);
                    }
                }
            }
        }
        for (int i = 0; i <= n; ++i)
            pot[i] = dis[i] == LLONG_MAX ? 0 : dis[i]; // unreachable nodes stay unreachable
        return true;
    }

    bool dijkstra() {
        fill(dis.begin(), dis.end(), LLONG_MAX);
        fill(par.begin(), par.end(),- 1);
        dis[src] = 0;
        priority_queue<pair<ll, int>, vector<pair<ll, int> >, greater<pair<ll, int>>>
                pq;
        pq.emplace(0, src);
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dis[u])
                continue;
            for (int i = 0; i < (int) adj[u].size(); ++i) {
                edge &e = adj[u][i];
                if (e.flow == e.cap)
                    continue;
                ll new_cost = e.cost + pot[u] - pot[e.to];
                if (dis[e.to] > dis[u] + new_cost) {
                    dis[e.to] = dis[u] + new_cost;
                    par[e.to] = u;
                    from_edge[e.to] = i;
                    pq.emplace(dis[e.to], e.to);
                }
            }
        }
        return dis[sink] != LLONG_MAX;
    }

    // Returns {flow, cost}; returns {-1, 0} if a negative cycle is reachable from src.
    // anyFlow = true  -> min cost over ANY flow value (stop when the path cost >= 0).
    // anyFlow = false -> min-cost MAX-flow (up to maxFlow units).
    pair<int, ll> minCostMaxFlow(int _src, int _sink, bool anyFlow = false,
                                 int maxFlow = INT_MAX) {
        src = _src;
        sink = _sink;
        if (!initPotentials())
            return {-1, 0};
        int flow = 0;
        ll cost = 0;
        while (flow < maxFlow && dijkstra()) {
            for (int i = 0; i <= n; ++i)
                if (dis[i] < LLONG_MAX)
                    pot[i] += dis[i];
            if (anyFlow && pot[sink] - pot[src] >= 0)
                break;
            int push = maxFlow - flow, cur = sink;
            while (cur != src) {
                int p = par[cur], e = from_edge[cur];
                push = min(push, adj[p][e].cap - adj[p][e].flow);
                cur = p;
            }
            cur = sink;
            while (cur != src) {
                int p = par[cur], e = from_edge[cur];
                adj[p][e].flow += push;
                adj[cur][adj[p][e].backEdge].flow -= push;
                cost += 1LL * push * adj[p][e].cost;
                cur = p;
            }
            flow += push;
        }
        return {flow, cost};
    }
};