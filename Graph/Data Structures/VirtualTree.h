#define all(v) v.begin(), v.end()

struct VirtualTree{ // 1-based
    int n, LG, id = -1, root;
    vector<int> in, out, depth;
    vector<vector<int>> adj, vt, up;

    VirtualTree(vector<vector<int>> &adj, int root = 1)
    : n(sz(adj)-1), adj(adj), vt(n+1), in(n+1), out(n+1), depth(n+1) {
        LG = __lg(n) + 1;
        up.resize(n+1, vector<int>(LG, root));
        init(root, 0);
    }

    vector<int> operator[](int u) {return vt[u];}

    void init(int u, int p){
        for(int k=1; k<LG; k++)
            up[u][k] = up[up[u][k-1]][k-1];

        in[u] = ++id;
        depth[u] = depth[p] + 1;
        for(auto &v : adj[u]){
            if(v == p)  continue;
            up[v][0] = u;
            init(v, u);
        }
        out[u] = id;
    }

    inline bool upper(int u, int v){
        return in[u] <= in[v] && out[v] <= out[u];
    }

    int lca(int u, int v){
        if(upper(u, v)) return u;
        if(upper(v, u)) return v;
        for(int i = LG - 1; i >= 0; i--){
            if(!upper(up[u][i], v))
                u = up[u][i];
        }
        return up[u][0];
    }

    int build(vector<int> nodes){
        sort(all(nodes), [&](int u, int v){
            return in[u] < in[v];
        });

        int k = sz(nodes);
        for(int i=1; i<k; i++)
            nodes.push_back(lca(nodes[i-1], nodes[i]));

        sort(all(nodes), [&](int u, int v){
            return in[u] < in[v];
        });
        nodes.erase(unique(all(nodes)), nodes.end());

        for(auto &u : nodes) vt[u].clear();

        stack<int> stk;
        for(auto &u : nodes){
            while(sz(stk) >= 2 && !upper(stk.top(), u)){
                int v = stk.top();  stk.pop();
                vt[stk.top()].push_back(v);
                vt[v].push_back(stk.top());
            }
            stk.push(u);
        }

        while(sz(stk) >= 2){
            int v = stk.top();  stk.pop();
            vt[stk.top()].push_back(v);
            vt[v].push_back(stk.top());
        }

        return root = stk.top();
    }
};