// unweighted graph
const int INF = 1e5;
int n;
vector<vector<int>>adj;

int get_shortest_cycle(int start) {
    vector<int> dist(n + 1, INF), parent(n + 1, -1);
    queue<int> q;
    dist[start] = 0;
    q.push(start);
    int mn_cycle = INF;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                parent[v] = u;
                q.push(v);
            } else if (parent[u] != v) {
                mn_cycle = min(mn_cycle, dist[u] + dist[v] + 1);
            }
        }
    }
    return mn_cycle;
}

// weighted graph
const ll INF = 1e18;
int n;
vector<vector<pair<int, ll>>> adj;

ll get_shortest_cycle(int start) {
    vector<ll> dist(n + 1, INF);
    vector<int> parent(n + 1, -1);
    // Min-heap priority queue stores pairs of {current_distance, node}
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    dist[start] = 0;
    pq.push({0, start});
    ll mn_cycle = INF;
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        for (auto edge : adj[u]) {
            int v = edge.first;
            ll w = edge.second;
            if (dist[v] > dist[u] + w) {
                // Node v is unvisited, or we found a shorter path to it
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            } else if (parent[u] != v) {
                // Node v is visited, and it's NOT the parent we just came from.
                // We found a cross-edge forming a cycle!
                // Cycle length = distance to u + distance to v + weight of the connecting edge
                mn_cycle = min(mn_cycle, dist[u] + dist[v] + w);
            }
        }
    }
    return mn_cycle;
}