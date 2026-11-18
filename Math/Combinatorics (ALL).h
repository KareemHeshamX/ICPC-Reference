// ============================================================================
//  COMBINATORICS CHEAT SHEET FOR CP  (identities + C++ templates)
//  Each template section is independent: copy only what you need.
//  Compile self-tests with:  g++ -std=c++17 -DSELFTEST file.cpp
// ============================================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// ============================================================================
//  PART 1: IDENTITIES AND FORMULAS (reference only)
// ============================================================================
/*
--- BINOMIAL IDENTITIES -----------------------------------------------------
Pascal             C(n,k) = C(n-1,k-1) + C(n-1,k)
Symmetry           C(n,k) = C(n,n-k)
Absorption         k*C(n,k) = n*C(n-1,k-1)
Subset of subset   C(n,k)*C(k,j) = C(n,j)*C(n-j,k-j)
Row sum            sum_k C(n,k) = 2^n
Alternating        sum_k (-1)^k C(n,k) = 0                      (n >= 1)
Even/odd subsets   sum_{k even} C(n,k) = sum_{k odd} C(n,k) = 2^(n-1)   (n >= 1)
Weighted sums      sum_k k*C(n,k) = n*2^(n-1)
                   sum_k k(k-1)*C(n,k) = n(n-1)*2^(n-2)
Squares            sum_k C(n,k)^2 = C(2n,n)
Vandermonde        sum_k C(m,k)*C(n,r-k) = C(m+n,r)
Hockey stick       sum_{i=r}^{n} C(i,r) = C(n+1,r+1)
Parallel sum       sum_{i=0}^{k} C(n+i,i) = C(n+k+1,k)
Upper negation     C(-n,k) = (-1)^k * C(n+k-1,k)
Binomial theorem   (x+y)^n = sum_k C(n,k) x^k y^(n-k)
Power sums         sum_k C(n,k) k^m = sum_j S2(m,j) * n^(falling j) * 2^(n-j)

--- COUNTING PRINCIPLES -----------------------------------------------------
Sum rule, product rule, bijection, double counting, pigeonhole.
Linearity of expectation: count = sum over objects of P(object is "good").
Inclusion-exclusion: |U Ai| = sum_{S nonempty} (-1)^(|S|+1) |intersection Ai|
Stars and bars (x1+..+xk = n):
    nonneg solutions   C(n+k-1, k-1)
    positive solutions C(n-1, k-1)
    with xi <= ci      inclusion-exclusion over the violated set
Multinomial        n! / (k1! k2! ... km!)
Circular arrangements of n distinct items: (n-1)!
Binomial inversion f(n) = sum_k C(n,k) g(k)  <=>  g(n) = sum_k (-1)^(n-k) C(n,k) f(k)
    ("at least" counts f  ->  "exactly" counts g)

--- LATTICE PATHS, CATALAN --------------------------------------------------
Paths (0,0)->(a,b) with right/up steps: C(a+b,a)
Reflection principle: paths touching line y=x+c  =  paths from reflected start.
Never go above y=x, (0,0)->(n,n): Catalan.
Ballot: a votes for A, b for B, a > b. A strictly ahead always:
    (a-b)/(a+b) * C(a+b,a)
Catalan  Cat(n) = C(2n,n)/(n+1) = C(2n,n) - C(2n,n+1)
         Cat(n+1) = sum_i Cat(i)*Cat(n-i)
         Counts: Dyck paths, balanced parens, binary trees (n nodes),
                 triangulations of an (n+2)-gon, non-crossing partitions.
LGV lemma: non-intersecting path systems = det of path-count matrix.

--- NAMED SEQUENCES ---------------------------------------------------------
Derangements   D(n) = (n-1)(D(n-1)+D(n-2)) = n*D(n-1) + (-1)^n    D0=1, D1=0
Involutions    I(n) = I(n-1) + (n-1)*I(n-2)
Surjections n -> k:   k! * S2(n,k)
Cayley         n^(n-2) labeled trees; with degrees d_i: (n-2)! / prod (d_i-1)!
Forests / rooted trees: n^(n-1) rooted labeled trees.
Partitions p(n): pentagonal number theorem -> O(n sqrt n) DP.

--- STIRLING / BELL / LAH ---------------------------------------------------
S2(n,k) = k*S2(n-1,k) + S2(n-1,k-1)         (partitions into k blocks)
S2(n,k) = (1/k!) sum_{j=0..k} (-1)^(k-j) C(k,j) j^n
c(n,k)  = (n-1)*c(n-1,k) + c(n-1,k-1)       (unsigned; perms with k cycles)
x^n          = sum_k S2(n,k) * x^(falling k)
x^(rising n) = sum_k c(n,k) * x^k            [x(x+1)...(x+n-1)]
sum_k c(n,k) = n!        B(n) = sum_k S2(n,k)
B(n+1) = sum_k C(n,k) B(k)       EGF of Bell: exp(e^x - 1)
Touchard: B(n+p) = B(n) + B(n+1)  (mod p)
Lah L(n,k) = C(n-1,k-1) * n!/k!   (ordered partitions into k non-empty lists)

--- SYMMETRY ----------------------------------------------------------------
Burnside: #orbits = (1/|G|) * sum_{g in G} Fix(g)
Necklaces, k colors, rotations only:   (1/n) sum_{d|n} phi(d) k^(n/d)
Bracelets add reflections: n odd -> add n*k^((n+1)/2); n even -> add
    (n/2)*(k^(n/2+1) + k^(n/2)); divide the total by 2n.

--- nCr MOD p: NUMBER THEORY ------------------------------------------------
Wilson    (p-1)! = -1 (mod p)
Fermat    a^(p-1) = 1 (mod p) -> inv(a) = a^(p-2)
Lucas     C(n,k) = prod C(n_i,k_i) (mod p), over base-p digits
Legendre  v_p(n!) = sum_{i>=1} floor(n / p^i)
Kummer    v_p(C(n,k)) = number of carries when adding k and n-k in base p
Composite modulus: factor M = prod p^e, compute C mod p^e by removing p-factors
    from factorials, then combine with CRT.
n huge (1e18), k small: product formula in O(k) (see Cbig).
Prefix sums of a row, S(n,k) = sum_{i<=k} C(n,i):
    S(n+1,k) = 2*S(n,k) - C(n,k)      S(n,k+1) = S(n,k) + C(n,k+1)
    -> Mo's-style walk over (n,k) for offline queries.

--- GENERATING FUNCTIONS ----------------------------------------------------
1/(1-x)^k = sum C(n+k-1,k-1) x^n        e^x = sum x^n/n!    (EGF of constant 1)
Catalan GF   C(x) = (1 - sqrt(1-4x)) / (2x)
OGF: unordered/choose.  EGF: labeled structures (product = convolution of binomials)
Bell: exp(e^x - 1)    Derangements: e^(-x)/(1-x)    Involutions: exp(x + x^2/2)
Lagrange inversion: [x^n] H(F^-1(x)) = (1/n) [x^(n-1)] H'(x) (x/F(x))^n
*/

// ============================================================================
//  PART 2: TEMPLATES
// ============================================================================
const ll MOD = 998244353;                 // or 1000000007

ll pw(ll b, ll e, ll m = MOD) {
    ll r = 1; b %= m; if (b < 0) b += m;
    for (; e > 0; e >>= 1, b = b * b % m) if (e & 1) r = r * b % m;
    return r;
}
ll inv(ll a) { return pw(a, MOD - 2); }   // MOD must be prime

// ---- Factorials, nCr, nPr: O(N) precompute, O(1) query ---------------------
const int N = 2000005;                    // raise to 2*maxN if you need C(2n,n)
ll fact[N], ifact[N];
void initFact(int n = N - 1) {
    fact[0] = 1;
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i % MOD;
    ifact[n] = inv(fact[n]);
    for (int i = n; i > 0; i--) ifact[i - 1] = ifact[i] * i % MOD;
}
ll C(ll n, ll k) {
    if (n < 0 || k < 0 || k > n) return 0;
    return fact[n] * ifact[k] % MOD * ifact[n - k] % MOD;
}
ll P(ll n, ll k) {
    if (n < 0 || k < 0 || k > n) return 0;
    return fact[n] * ifact[n - k] % MOD;
}
ll multinomial(const vector<int>& ks) {
    int s = 0; ll r = 1;
    for (int x : ks) { s += x; r = r * ifact[x] % MOD; }
    return fact[s] * r % MOD;
}
ll starsBars(ll n, ll k) {                // non-negative solutions of x1+..+xk = n
    if (k == 0) return n == 0;
    return C(n + k - 1, k - 1);
}
ll starsBarsPos(ll n, ll k) { return C(n - 1, k - 1); }   // positive solutions
ll catalan(int n) { return (C(2 * n, n) - C(2 * n, n + 1) + MOD) % MOD; }
ll ballot(ll a, ll b) {                   // A always strictly ahead, a > b
    if (a <= b) return 0;
    return (C(a + b - 1, a - 1) - C(a + b - 1, a) + MOD) % MOD;  // = (a-b)/(a+b) * C(a+b,a)
}

// ---- C(n,k) with n up to ~1e18, k small (k < MOD): O(k + log) --------------
ll Cbig(ll n, ll k) {
    if (k < 0 || k > n) return 0;
    ll num = 1, den = 1;
    for (ll i = 0; i < k; i++) {
        num = num * ((n - i) % MOD) % MOD;
        den = den * ((i + 1) % MOD) % MOD;
    }
    return num * inv(den) % MOD;
}

// ---- Lucas: C(n,k) mod p, p small prime (<= ~1e6), n,k up to 1e18 ----------
ll lucasSmall(ll n, ll k, int p) {
    static vector<ll> f, fi; static int cp = 0;
    if (cp != p) {
        f.assign(p, 1); fi.assign(p, 1);
        for (int i = 1; i < p; i++) f[i] = f[i - 1] * i % p;
        fi[p - 1] = pw(f[p - 1], p - 2, p);
        for (int i = p - 1; i > 0; i--) fi[i - 1] = fi[i] * i % p;
        cp = p;
    }
    ll res = 1;
    while (n || k) {
        ll a = n % p, b = k % p;
        if (b > a) return 0;
        res = res * f[a] % p * fi[b] % p * fi[a - b] % p;
        n /= p; k /= p;
    }
    return res;
}

// ---- Binomial inversion, O(n^2): given f[0..n] ("at least") get g ("exactly")
// f(n) = sum_k C(n,k) g(k)  =>  g(n) = sum_k (-1)^(n-k) C(n,k) f(k)
vector<ll> binomInvert(const vector<ll>& f) {
    int n = f.size();
    vector<ll> g(n);
    for (int i = 0; i < n; i++) {
        ll s = 0;
        for (int k = 0; k <= i; k++) {
            ll t = C(i, k) * f[k] % MOD;
            s = ((i - k) & 1) ? (s - t + MOD) % MOD : (s + t) % MOD;
        }
        g[i] = s;
    }
    return g;
}

// ---- Stirling numbers of the 2nd kind --------------------------------------
vector<vector<ll>> stirling2Table(int n) {          // O(n^2), S[i][k]
    vector<vector<ll>> S(n + 1, vector<ll>(n + 1, 0));
    S[0][0] = 1;
    for (int i = 1; i <= n; i++)
        for (int k = 1; k <= i; k++)
            S[i][k] = (k * S[i - 1][k] + S[i - 1][k - 1]) % MOD;
    return S;
}
ll stirling2Single(ll n, int k) {                   // O(k log n), needs fact up to k
    ll s = 0;
    for (int j = 0; j <= k; j++) {
        ll t = C(k, j) * pw(j, n) % MOD;
        s = ((k - j) & 1) ? (s - t + MOD) % MOD : (s + t) % MOD;
    }
    return s * ifact[k] % MOD;
}
// Full row S2(n, 0..n) in O(n log n): convolve a_j = j^n/j! with b_i = (-1)^i/i!
// using NTT, then S2(n,k) = (a*b)[k].

// ---- Stirling numbers of the 1st kind (unsigned) ---------------------------
vector<vector<ll>> stirling1Table(int n) {          // O(n^2)
    vector<vector<ll>> c(n + 1, vector<ll>(n + 1, 0));
    c[0][0] = 1;
    for (int i = 1; i <= n; i++)
        for (int k = 1; k <= i; k++)
            c[i][k] = ((i - 1) * c[i - 1][k] + c[i - 1][k - 1]) % MOD;
    return c;
}

// ---- Bell numbers via the Bell triangle, O(n^2), no factorials needed ------
vector<ll> bellNumbers(int n) {                     // returns B[0..n]
    vector<ll> B(n + 1), prev = {1}, cur;
    B[0] = 1;
    for (int i = 1; i <= n; i++) {
        cur.assign(i + 1, 0);
        cur[0] = prev.back();
        for (int j = 1; j <= i; j++) cur[j] = (cur[j - 1] + prev[j - 1]) % MOD;
        B[i] = cur[0];
        prev = cur;
    }
    return B;
}

// ---- Derangements, involutions, Lah, Cayley --------------------------------
vector<ll> derangements(int n) {
    vector<ll> D(max(n + 1, 2));
    D[0] = 1; D[1] = 0;
    for (int i = 2; i <= n; i++) D[i] = (i - 1) * ((D[i - 1] + D[i - 2]) % MOD) % MOD;
    return D;
}
vector<ll> involutions(int n) {
    vector<ll> I(max(n + 1, 2));
    I[0] = I[1] = 1;
    for (int i = 2; i <= n; i++) I[i] = (I[i - 1] + (i - 1) * I[i - 2]) % MOD;
    return I;
}
ll lah(int n, int k) {                              // needs fact up to n
    if (k < 1 || k > n) return n == 0 && k == 0;
    return C(n - 1, k - 1) * fact[n] % MOD * ifact[k] % MOD;
}
ll cayley(ll n) { return n <= 1 ? 1 : pw(n, n - 2); }   // labeled trees on n nodes

// ---- Burnside: necklaces with k colors, rotations only ---------------------
ll phi(ll x) {
    ll r = x;
    for (ll p = 2; p * p <= x; p++)
        if (x % p == 0) { while (x % p == 0) x /= p; r -= r / p; }
    if (x > 1) r -= r / x;
    return r;
}
ll necklaces(int n, ll k) {
    ll s = 0;
    for (int d = 1; d <= n; d++)
        if (n % d == 0) s = (s + phi(d) % MOD * pw(k, n / d)) % MOD;
    return s * inv(n) % MOD;
}

// ---- Inclusion-exclusion over subsets, O(2^m * cost) -----------------------
// Count objects satisfying NONE of m bad properties:
//   ans = sum_{S subset} (-1)^|S| * cntAll(S)
// where cntAll(S) = #objects satisfying every property in S.
template <class F>
ll inclusionExclusion(int m, F cntAll) {
    ll ans = 0;
    for (int mask = 0; mask < (1 << m); mask++) {
        ll c = cntAll(mask) % MOD;
        ans = (__builtin_popcount(mask) & 1) ? (ans - c + MOD) % MOD : (ans + c) % MOD;
    }
    return ans;
}