struct AhoCorasick {
    static const int alpha = 26;
    char offset = 'a';

    struct Node {
        array<int, alpha> nxt{};
        int fail = 0, idx = -1, up = 0;
        Node(){ nxt.fill(-1); }
    };

    vector<Node> trie{Node()};
    vector<int> pat_len;

    int add_pattern(const string& s){
        int cur = 0;
        for(auto& ch : s){
            int c = ch - offset;
            if(trie[cur].nxt[c] == -1){
                trie[cur].nxt[c] = (int)trie.size();
                trie.emplace_back();
            }
            cur = trie[cur].nxt[c];
        }
        if(trie[cur].idx == -1){
            trie[cur].idx = sz(pat_len);
            pat_len.push_back(sz(s));
        }
        return trie[cur].idx;
    }

    int compF(int u, int c){
        while(trie[u].nxt[c] == -1) u = trie[u].fail;
        return trie[u].nxt[c];
    }

    int getNxt(int u){
        if(!u) return u;
        int& v = trie[u].up;
        return (~trie[v].idx ? v : v = getNxt(v));
    }

    void buildAhoTree(){
        queue<int> q;
        for(int c = 0; c < alpha; ++c){
            int u = trie[0].nxt[c];
            if(u == -1) trie[0].nxt[c] = 0;
            else{
                trie[u].fail = 0;
                q.push(u);
            }
        }
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(int c = 0; c < alpha; ++c){
                int v = trie[u].nxt[c];
                if(v == -1) continue;
                trie[v].up = trie[v].fail = compF(trie[u].fail, c);
                q.push(v);
            }
        }
    }

    vector<vector<int>> match(const string& s){
        buildAhoTree();
        vector<vector<int>> res(sz(pat_len));
        int cur = 0;
        for(int i = 0; i < sz(s); ++i){
            int c = s[i] - offset;
            cur = compF(cur, c);
            for(int u = cur; u ; u = getNxt(u)){
                if(trie[u].idx != -1){
                    res[trie[u].idx].push_back(i - pat_len[trie[u].idx] + 1);
                }
            }
        }
        return res;
    }
};