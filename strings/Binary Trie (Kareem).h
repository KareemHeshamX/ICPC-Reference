struct trie {
    trie* next[2]{};
    int prefix = 0;
    int best = -1;
    trie* root = this;
    void insert(ll x, int k) {
        trie* current = this;
        for (int i = 0; i < k; i++) {
            int bit = (x >> i) & 1;
            if (current -> next[bit] == nullptr) {
                current -> next[bit] = new trie;
            }
            current -> best = -1;
            current = current -> next[bit];
            current -> prefix++;
        }
    }
    void erase(ll x, trie* current, int i, int k) {
        if (i == k) return;
        int bit = (x >> i) & 1;
        current -> best = -1;
        erase(x, current -> next[bit], i + 1, k);
        current -> next[bit] -> prefix--;
        if (current -> next[bit] -> prefix == 0) {
            delete current -> next[bit];
            current -> next[bit] = nullptr;
        }
    }
    int check (trie* current, int bit) {
        if (!current -> next[bit]) return 0;
        return current -> next[bit] ->prefix;
    }
    int max_sum(trie* current, ll prev, int i, int k) {
        if (!current) return 0;
        if (i == k - 1) {
            if (check(current, 1) != prev && check(current, 0) != prev) return -1e8;
            return 0;
        }
        if (~current->best) return current->best;
        int ret = 0;
        if (current->next[1]) ret = check(current, 1) + max_sum(current->next[1], check(current, 1), i + 1, k);
        int ret2 = 0;
        if (current->next[0]) ret2 = check(current, 0) + max_sum(current->next[0], check(current, 0), i + 1, k);
        current->best = max(ret, ret2);
        return current -> best;
    }
};