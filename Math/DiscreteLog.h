int power(int a, int b, int m) {
    int res = 1;
    while (b > 0) {
        if (b & 1) res = (1LL * res * a) % m;
        a = (1LL * a * a) % m;
        b >>= 1;
    }
    return res;
}

// return one of the possible solutions
int discrete_log(int a, int b, int m) {
    a %= m, b %= m;
    int n = sqrt(m) + 1;
    unordered_map<int, int> vals;
    for (int p = 1; p <= n; ++p)
        vals[power(a, p * n, m)] = p;
    for (int q = 0; q <= n; ++q) {
        int cur = (1LL * power(a, q, m) * b) % m;
        if (vals.count(cur)) {
            int ans = vals[cur] * n - q;
            return ans;
        }
    }
    return -1;
}

// Returns minimum x for which a ^ x % m = b % m, a and m are coprime.
int discrete_log(int a, int b, int m) {
    a %= m, b %= m;
    int n = sqrt(m) + 1;

    int an = 1;
    for (int i = 0; i < n; ++i)
        an = (an * 1ll * a) % m;

    unordered_map<int, int> vals;
    for (int q = 0, cur = b; q <= n; ++q) {
        vals[cur] = q;
        cur = (cur * 1ll * a) % m;
    }

    for (int p = 1, cur = 1; p <= n; ++p) {
        cur = (cur * 1ll * an) % m;
        if (vals.count(cur)) {
            int ans = n * p - vals[cur];
            return ans;
        }
    }
    return -1;
}


// (General Solution) when a and m are not coprime
// Returns minimum x for which a ^ x % m = b % m.
int discrete_log(int a, int b, int m) {
    a %= m, b %= m;
    int k = 1, add = 0, g;
    while ((g = __gcd(a, m)) > 1) {
        if (b == k)
            return add;
        if (b % g)
            return -1;
        b /= g, m /= g, ++add;
        k = (k * 1ll * a / g) % m;
    }

    int n = sqrt(m) + 1;
    int an = 1;
    for (int i = 0; i < n; ++i)
        an = (an * 1ll * a) % m;

    unordered_map<int, int> vals;
    for (int q = 0, cur = b; q <= n; ++q) {
        vals[cur] = q;
        cur = (cur * 1ll * a) % m;
    }

    for (int p = 1, cur = k; p <= n; ++p) {
        cur = (cur * 1ll * an) % m; // to get the minimum x
        if (vals.count(cur)) {
            int ans = n * p - vals[cur] + add;
            return ans;
        }
    }
    return -1;
}