namespace FFT {
    const int mod = 998244353; // 1451753786686046209
    const int root = 3;
    const int invRoot = 332748118;
 
    int fp(int b, int e) {
        int res = 1;
        while(e) {
            if (e & 1) res = int(b * 1LL * res % mod);
            b = int(b * 1LL * b % mod);
            e >>= 1;
        }
        return res;
    }
    void primitiveRoot() {
        int phi = mod - 1;
        vector<int> fac;
        for(int i = 2; i * 1LL * i < phi; i++) {
            if(phi % i == 0) {
                fac.push_back(i);
                while(phi % i == 0) phi /= i;
            }
        }
        if(phi > 1) fac.push_back(phi);
 
        for (int g = 2; g < mod; g++) {
            for (int pr : fac) {
                if (fp(g, (mod - 1) / pr) == 1) {
                    goto bad;
                }
            }
            cout << "const int root = " << g << ";\n";
            cout << "const int invRoot = " << fp(g, mod - 2) << ";\n";
            return;
            bad:;
        }
        cerr << "404\n";
    }
 
    using cd = complex<double>;
    double pi = acos(-1);
 
    void fft(vector<cd> &a, bool invert) {
        int n = (int)a.size();
 
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for(; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if(i < j) swap(a[i], a[j]);
        }
 
        for (int len = 2; len <= n; len <<= 1) {
            double ang = 2 * pi / len * (invert ? -1 : 1);
            cd w1(cos(ang), sin(ang));
            for (int i = 0; i < n; i += len) {
                cd w(1);
                for(int j = 0; j * 2 < len; j++) {
                    cd u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w *= w1;
                }
            }
        }
        if (invert) {
            for (cd & x : a)
                x /= n;
        }
    }
    vector<int64_t> mul(const vector<int> &a, const vector<int> &b) {
        int N = 1 << (__lg(a.size() + b.size()) + 1);
 
        vector<cd> ta(a.begin(), a.end()), tb(b.begin(), b.end());
        ta.resize(N);
        tb.resize(N);
 
        fft(ta, false), fft(tb, false);
 
        for(int i = 0; i < N; i++)
            ta[i] *= tb[i];
 
        fft(ta, true);
 
        vector<int64_t> ans(N);
        for(int i = 0; i < N; i++) {
            ans[i] = (int64_t)round(ta[i].real());
        }
 
        return ans;
    }
 
    void ntt(vector<int> &a, bool invert) {
        int n = (int)a.size();
 
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
 
        for (int len = 2; len <= n; len <<= 1) {
            int w1 = fp(invert ? invRoot : root, (mod - 1) / len);
 
            for (int i = 0; i < n; i += len) {
                int w = 1;
                for (int j = 0; j < len / 2; j++) {
                    int u = a[i + j], v = int(a[i + j + len / 2] * 1LL * w % mod);
                    a[i + j] = u + v < mod ? u + v : u + v - mod;
                    a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + mod;
                    w = int(w * 1LL * w1 % mod);
                }
            }
        }
 
        if (invert) {
            int n_1 = fp(n, mod - 2);
            for (int & x : a)
                x = int(x * 1LL * n_1 % mod);
        }
    }
    vector<int> mulMod(vector<int> a, vector<int> b) {
        int N = 1 << (__lg(a.size() + b.size()) + 1);
        a.resize(N);
        b.resize(N);
 
        ntt(a, false), ntt(b, false);
 
        for(int i = 0; i < N; i++)
            a[i] = int(a[i] * 1LL * b[i] % mod);
 
        ntt(a, true);
 
        return a;
    }
}