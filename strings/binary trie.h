struct trie {
    trie* next[2]{};
    int prefix = 0;
    trie* root = this;
    void insert(int x) {
        trie* current = this;
        for (int i = 30; i >= 0; i--) {
            int bit = (1 << i) & x;
            if (current -> next[bit] == nullptr) {
                current -> next[bit] = new trie;
            }
            current = current -> next[bit];
            current -> prefix++;
        }
    }
    void erase(int x, trie* current, int i = 30) {
        if (i == -1) return;
        int bit = (1 << i) & x;
        erase(x, current -> next[bit], i - 1);
        current -> next[bit] -> prefix--;
        if (current -> next[bit] -> prefix == 0) {
            delete current -> next[bit];
            current -> next[bit] = nullptr;
        }
    }
};