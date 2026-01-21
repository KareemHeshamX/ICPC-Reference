//iterative version
vector<int> sos(1 << n);
vector<vector<int>> dp(1 << n, vector<int>(n + 1));

for (int x = 0; x < (1 << n); x++) {
    dp[x][0] = a[x];
    for (int i = 0; i < n; i++) {
        dp[x][i + 1] = dp[x][i];
        if (x & (1 << i)) { dp[x][i + 1] += dp[x ^ (1 << i)][i]; }
    }
    sos[x] = dp[x][n];
}
//memory optimized, super easy to code.
sos = a;
for (int i = 0; i < n; i++) {
    for (int x = 0; x < (1 << n); x++) {
        if (x & (1 << i)) { sos[x] += sos[x ^ (1 << i)]; }
    }
}