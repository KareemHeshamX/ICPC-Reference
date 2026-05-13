template<typename T, int MAX_NODES>
class Treap {
private:
    struct Node {
        T key;
        int prior, size, freq;
        int l, r;
        Node() { prior = size = freq = l = r = 0; }
    };

    Node pool[MAX_NODES];
    int root, pool_cnt;
    vector<int> recycle_bin;
    mt19937 rng;

    int getsz(int u) { return u ? pool[u].size : 0; }

    void pull(int u) {
        if (u) pool[u].size = getsz(pool[u].l) + getsz(pool[u].r) + pool[u].freq;
    }

    int alloc_node(const T& val) {
        int id = recycle_bin.empty() ? ++pool_cnt : recycle_bin.back();
        if (!recycle_bin.empty()) recycle_bin.pop_back();

        pool[id].key = val;
        pool[id].prior = rng();
        pool[id].size = pool[id].freq = 1;
        pool[id].l = pool[id].r = 0;
        return id;
    }

    void split(int u, T key, int& l, int& r) {
        if (!u) { l = r = 0; return; }
        if (pool[u].key <= key) {
            split(pool[u].r, key, pool[u].r, r);
            l = u;
        } else {
            split(pool[u].l, key, l, pool[u].l);
            r = u;
        }
        pull(u);
    }

    int merge(int l, int r) {
        if (!l || !r) return l ? l : r;
        if (pool[l].prior > pool[r].prior) {
            pool[l].r = merge(pool[l].r, r);
            pull(l); return l;
        } else {
            pool[r].l = merge(l, pool[r].l);
            pull(r); return r;
        }
    }

    bool increment(int u, T key) {
        if (!u) return false;
        if (pool[u].key == key) { pool[u].freq++; pull(u); return true; }
        bool res = increment(key < pool[u].key ? pool[u].l : pool[u].r, key);
        pull(u); return res;
    }

    void erase_node(int& u, T key) {
        if (!u) return;
        if (pool[u].key == key) {
            if (--pool[u].freq == 0) {
                int temp = u;
                u = merge(pool[u].l, pool[u].r);
                recycle_bin.push_back(temp); // Recycle memory instantly
            }
        } else {
            erase_node(key < pool[u].key ? pool[u].l : pool[u].r, key);
        }
        pull(u);
    }

    T kth(int u, int k) {
        int left_sz = getsz(pool[u].l);
        if (k <= left_sz) return kth(pool[u].l, k);
        if (k <= left_sz + pool[u].freq) return pool[u].key;
        return kth(pool[u].r, k - left_sz - pool[u].freq);
    }

    int order(int u, T key) {
        if (!u) return 0;
        if (key < pool[u].key) return order(pool[u].l, key);
        if (key == pool[u].key) return getsz(pool[u].l);
        return getsz(pool[u].l) + pool[u].freq + order(pool[u].r, key);
    }

public:
    Treap(int seed = 1337) : root(0), pool_cnt(0), rng(seed) {
        pool[0] = Node(); // Sentinel at index 0 ensures safe null checks
    }

    // Call this before a new test case to achieve O(1) reset
    void clear() {
        root = pool_cnt = 0;
        recycle_bin.clear();
    }

    void insert(T x) {
        if (!increment(root, x)) {
            int l, r;
            split(root, x, l, r);
            root = merge(merge(l, alloc_node(x)), r);
        }
    }

    void erase(T x) { erase_node(root, x); }
    T get_kth(int k) { assert(k >= 1 && k <= getsz(root)); return kth(root, k); }
    int order_of_key(T x) { return order(root, x); }
    int size() { return getsz(root); }
};