struct trie {
    trie* next[2]{};
    int prefix = 0;
    trie* root = this;
    void insert(int x) {
        trie* current = this;
        for (int i = 30; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if (current -> next[bit] == nullptr) {
                current -> next[bit] = new trie;
            }
            current = current -> next[bit];
            current -> prefix++;
        }
    }
    void erase(int x, trie* current, int i = 30) {
        if (i == -1) return;
        int bit = (x >> i) & 1;
        erase(x, current -> next[bit], i - 1);
        current -> next[bit] -> prefix--;
        if (current -> next[bit] -> prefix == 0) {
            delete current -> next[bit];
            current -> next[bit] = nullptr;
        }
    }
    int max_xor(int x, trie* current, int i = 30) {
        if (i == -1 || !current) return 0;
        int bit = (x >> i) & 1;
        int ret = 0;
        if (current->next[1 - bit] && current->next[1 - bit]->prefix > 0) ret |= ((1 << i) | max_xor(x, current->next[1 - bit], i - 1));
        else ret |= max_xor(x, current->next[bit], i - 1);
        return ret;
    }
};