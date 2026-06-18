
struct cd { // replace with complex if the time is tight
    double x, y;
    cd(double x = 0, double y = 0) : x(x), y(y) {}
    cd operator+(const cd& o) const { return cd(x + o.x, y + o.y); }
    cd operator-(const cd& o) const { return cd(x - o.x, y - o.y); }
    cd operator*(const cd& o) const { return cd(x * o.x - y * o.y, x * o.y + y * o.x); }
};

namespace FFT {
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
        int N = 1;
        while (N < sz(a) + sz(b)) N <<= 1;

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
}