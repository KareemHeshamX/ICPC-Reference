namespace MTT {
    // Three NTT-friendly primes (all have primitive root = 3)
    constexpr int P1 = 998244353;
    constexpr int P2 = 1004535809;
    constexpr int P3 = 469762049;

    // Fast modular exponentiation
    ll power(ll base, ll exp, ll mod) {
        ll res = 1;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base) % mod;
            base = (base * base) % mod;
            exp /= 2;
        }
        return res;
    }

    // Modular inverse
    ll modInverse(ll n, ll m) {
        return power(n, m - 2, m);
    }

    // Standard Number Theoretic Transform
    template <int MOD>
    void ntt(vector<int>& a, bool invert) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            ll wlen = power(3, (MOD - 1) / len, MOD); // 3 is the primitive root
            if (invert) wlen = modInverse(wlen, MOD);
            for (int i = 0; i < n; i += len) {
                ll w = 1;
                for (int j = 0; j < len / 2; j++) {
                    ll u = a[i + j];
                    ll v = (a[i + j + len / 2] * w) % MOD;
                    a[i + j] = (u + v < MOD ? u + v : u + v - MOD);
                    a[i + j + len / 2] = (u - v >= 0 ? u - v : u - v + MOD);
                    w = (w * wlen) % MOD;
                }
            }
        }
        if (invert) {
            ll n_inv = modInverse(n, MOD);
            for (int& x : a) x = (x * n_inv) % MOD;
        }
    }

    // Convolution for a specific prime
    template <int MOD>
    vector<int> multiply(const vector<int>& a, const vector<int>& b) {
        vector<int> fa = a, fb = b;
        int n = 1;
        while (n < a.size() + b.size()) n <<= 1;
        fa.resize(n); fb.resize(n);

        ntt<MOD>(fa, false); 
        ntt<MOD>(fb, false);
        for (int i = 0; i < n; i++) fa[i] = 1LL * fa[i] * fb[i] % MOD;
        ntt<MOD>(fa, true);

        vector<int> res(a.size() + b.size() - 1);
        for (size_t i = 0; i < res.size(); i++) res[i] = fa[i];
        return res;
    }

    // The main function combining everything via Garner's Algorithm
    vector<int> convMod(const vector<int>& a, const vector<int>& b, int target_mod) {
        if (a.empty() || b.empty()) return {};

        // 1. Convolve the arrays under the three different prime modulos
        vector<int> r1 = multiply<P1>(a, b);
        vector<int> r2 = multiply<P2>(a, b);
        vector<int> r3 = multiply<P3>(a, b);

        ll m1 = P1, m2 = P2, m3 = P3;
        
        // Precompute inverses for Garner's
        ll inv1_2 = modInverse(m1, m2);
        ll inv12_3 = modInverse(m1 * m2 % m3, m3);

        vector<int> res(r1.size());
        
        for (size_t i = 0; i < res.size(); i++) {
            // Garner's Algorithm to reconstruct the exact number modulo M1*M2*M3
            
            // Step 1: Reconstruct x mod (m1 * m2)
            ll x1 = r1[i];
            ll x2 = (r2[i] - x1) % m2;
            if (x2 < 0) x2 += m2;
            x2 = (x2 * inv1_2) % m2;
            
            ll x12 = x1 + x2 * m1; // This is exact value modulo m1*m2

            // Step 2: Reconstruct x mod (m1 * m2 * m3)
            ll x3 = (r3[i] - x12 % m3) % m3;
            if (x3 < 0) x3 += m3;
            x3 = (x3 * inv12_3) % m3;

            // The exact answer before any modulo is: x12 + x3 * m1 * m2
            // Since we only want it modulo the target_mod, we apply the target modulo to the parts
            ll m1m2_mod = (m1 % target_mod) * (m2 % target_mod) % target_mod;
            ll ans = (x12 % target_mod) + (x3 % target_mod) * m1m2_mod % target_mod;
            
            res[i] = ans % target_mod;
        }
        return res;
    }
}