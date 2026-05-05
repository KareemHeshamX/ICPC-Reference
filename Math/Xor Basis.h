const int LOG_A = 60;

struct xor_basis {
    int basis[LOG_A + 1], pos[LOG_A + 1];
    int sz = 0;
    bool reduced = false;
    vector<int> nodes;

    xor_basis() {
        fill(basis, basis + LOG_A + 1, 0);
        fill(pos, pos + LOG_A + 1, 0);
    }

    void insert(int mask, int idx) {
        if (mask == 0) return;
        for (int i = LOG_A; i >= 0; i--) {
            if ((mask >> i) & 1) {
                if (!basis[i]) {
                    basis[i] = mask;
                    pos[i] = idx;
                    sz++;
                    reduced = false;
                    return;
                }
                if (pos[i] > idx) {
                    swap(mask, basis[i]);
                    swap(idx, pos[i]);
                }
                mask ^= basis[i];
            }
        }
    }

    bool contains(int mask, int L) {
        for (int i = LOG_A; i >= 0; i--) {
            if ((mask >> i)) {
                if (!basis[i] || pos[i] > L) return false;
                mask ^= basis[i];
            }
        }
        return mask == 0;
    }

    int max_xor(int start_val) {
        int res = start_val;
        for (int i = LOG_A; i >= 0; i--) {
            if (basis[i]) {
                res = max(res, res ^ basis[i]);
            }
        }
        return res;
    }

    int min_xor(int start_val) {
        int res = start_val;
        for (int i = LOG_A; i >= 0; i--) {
            if (basis[i]) {
                res = min(res, res ^ basis[i]);
            }
        }
        return res;
    }

    void build() {
        if (reduced) return;
        for (int i = 0; i <= LOG_A; i++) {
            if (basis[i]) {
                for (int j = i + 1; j <= LOG_A; j++) {
                    if ((basis[j] >> i) & 1) {
                        basis[j] ^= basis[i];
                    }
                }
            }
        }

        nodes.clear();
        for (int i = 0; i <= LOG_A; i++) {
            if (basis[i]) nodes.push_back(basis[i]);
        }
        reduced = true;
    }

    int kth_smallest(int k) {
        build();
        if (k >= (1ULL << sz)) return -1;

        int res = 0;
        for (int i = 0; i < sz; i++) {
            if ((k >> i) & 1) {
                res ^= nodes[i];
            }
        }
        return res;
    }

    int kth_largest(int k) {
        int total_combinations = (1ULL << sz);
        if (k > total_combinations) return -1;
        return kth_smallest(total_combinations - k);
    }

    void merge(const xor_basis &other) {
        for (int i = 0; i <= LOG_A; i++) {
            if (other.basis[i]) {
                insert(other.basis[i], sz);
            }
        }
        reduced = false;
    }
};