const int MAXN = 3e7, oo = 1e9;
int K;
 
struct Node {
    int nxt[2], pref, best;
    Node() {reset();}
    void reset() {
        nxt[0] = nxt[1] = -1;
        best = -oo;
        pref = 0;
    }
};
 
static Node pool[MAXN];
static int ptr;
static vector<int> freeList;
 
int new_node() {
    if (!freeList.empty()) {
        int id = freeList.back();
        freeList.pop_back();
        pool[id].reset();
        return id;
    }
    assert(ptr < MAXN);
    pool[ptr].reset();
    return ptr++;
}
 
void init_trie() {
    ptr = 0;
    freeList.clear();
    new_node(); // root will be index 0
}
 
void insert_trie(ll x, int add) {
    int cur = 0;
    vector<int>path = {0};
    pool[0].best = -oo;
    for (int b = 0; b < K; b++) {
        int bit = x >> b & 1;
        if (pool[cur].nxt[bit] == -1) pool[cur].nxt[bit] = new_node();
        cur = pool[cur].nxt[bit];
        path.push_back(cur);
        pool[cur].pref += add;
        pool[cur].best = -oo;
        if (!pool[cur].pref) pool[cur].best = 0;
    }
    path.pop_back();
    while (!path.empty()) {
        int node = path.back();
        for (auto child:pool[node].nxt) {
            if (child == -1) {
                pool[node].best = max(pool[node].best, pool[node].pref);
                continue;
            }
            pool[node].best = max(pool[node].best, pool[child].best + pool[node].pref);
        }
        path.pop_back();
    }
}