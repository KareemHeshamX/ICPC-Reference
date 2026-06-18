#define i64 uint64_t

const i64 MOD = (1ULL << 61) - 1;

inline i64 mul(i64 a, i64 b) {
    __uint128_t p = (__uint128_t)a * b;
    i64 ret = (i64)(p >> 61) + (i64)(p & MOD);
    return ret >= MOD ? ret - MOD : ret;
}

inline i64 add(i64 a, i64 b) {
    return a + b >= MOD ? a + b - MOD : a + b;
}

inline i64 sub(i64 a, i64 b) {
    return a >= b ? a - b : a - b + MOD;
}

struct Hash {
    static vector<i64> pw;
    static i64 BASE;

    vector<i64> pre;

    Hash(const string &s) {
        int n = s.size();
        pre.assign(n + 1, 0);

        while (pw.size() <= n) {
            pw.push_back(mul(pw.back(), BASE));
        }

        for (int i = 0; i < n; i++) {
            pre[i + 1] = add(mul(pre[i], BASE), s[i]);
        }
    }

    i64 getHash(int l, int r) const {
        return sub(pre[r + 1], mul(pre[l], pw[r - l + 1]));
    }
};

vector<i64> Hash::pw = {1};
i64 Hash::BASE = 0;

void initHash() {
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<i64> dist(257, MOD - 2);
    Hash::BASE = dist(rng);
}