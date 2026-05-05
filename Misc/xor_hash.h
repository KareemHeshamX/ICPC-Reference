#include <bits/stdc++.h>
using namespace std;

using ll = int_fast64_t;
using ld = long double;
#define sz(s) (int)(s.size())
// #define int ll

int dx[] = {0, 0, -1, 1, -1, -1, 1, 1};
int dy[] = {-1, 1, 0, 0, -1, 1, -1, 1};

#include <chrono> // keep-include
#include <random> // keep-include
//write this line once in top
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count() * ((uint64_t) new char | 1));
// use this instead of rand()
template<typename T>
T Rand(T low, T high) {
    return uniform_int_distribution<T>(low, high)(rng);
}

const int N = 5e5;
ll rnd[N + 1];

void testCase() {
    int n; cin >> n;
    vector<int>v(n);
    for (int &x:v) cin >> x;
    map<ll, int>mp;
    mp[0] = 1;
    ll ans = 0;
    vector<int>freq(n + 1), freq2(n + 1);
    vector<ll>hash(n + 1);
    int l = 0;
    for (int i = 0; i < n; i++) {
        int num = v[i];
        freq2[num]++;
        while (freq2[num] > 3) {
            mp[hash[l]]--;
            freq2[v[l++]]--;
        }
        int temp = freq[num];
        freq[num] = (freq[num] + 1) % 3;
        hash[i + 1] = hash[i] + (freq[num] - temp) * rnd[num];
        ans += mp[hash[i + 1]]++;
    }
    cout << ans;
}

signed main(int argc, char **argv) {
    cin.tie(0)->sync_with_stdio(0);
#ifdef LOCAL
    freopen(argv[1], "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    int t = 1;
    for (int i = 1; i <= N; i++) rnd[i] = Rand<ll>(1ll, 1ll << 63);
    // cin >> t;
    while (t--) testCase();
    return 0;
}