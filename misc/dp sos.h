const int LG = 22;
const int M = 1 << LG;
// subset contribute to its superset

void forward1(vector<ll>&dp) {
    for (int bt = 0; bt < LG; ++bt) {
        for (int m = 0; m < M; ++m) {
            if (m >> bt & 1){
                dp[m] += dp[m ^ (1 << bt)];
            }
        }
    }
}
// superset contribute to its subset

void forward2(vector<ll>&dp) {
    for (int bt = 0; bt < LG; ++bt) {
        for (int m = M - 1; m >= 0; m--) {
            if (m >> bt &1){
                dp[m ^ (1 << bt)] += dp[m];
            }
        }
    }
}
// remove subset contribution from superset

void backward1(vector<ll>&dp) {
    for (int bt = 0; bt < LG; bt++){
        for (int m = M - 1; m >= 0; m--){
            if (m >> bt &1){
                dp[m] -= dp[m ^ (1 << bt)];
            }
        }
    }
}
// remove superset contribution from subset

void backward2(vector<ll> &dp) {
    for (int bt = 0; bt < LG; bt++){
        for (int m = 0; m < M; m++){
            if (m >> bt &1){
                dp[m ^ (1 << bt)] -= dp[m];
            }
        }
    }
}