// ============================================================
//  CDQ + NTT  cheat-sheet
//  Idea: split [l,r) at m.  Solve left, let left CONTRIBUTE to right
//  with ONE convolution, solve right.   Cost O(n log^2 n).
// ============================================================

// ============================================================
// PATTERN 1: online / self-dependent convolution
//     f[i] = base[i] + sum_{j<i} f[j] * g[i-j]
// (f[i] needs the finished f[0..i-1]; g is known upfront)
// ============================================================
vector<int> f, g, base_;     // size N; f starts as copy of base_

void cdq1(int l, int r) {    // half-open [l, r)
    if (r - l == 1) {
        // f[l] is now final (everything j<l has been added).
        // Put "finalize" logic here, e.g. if (l==0) f[0]=1;
        return;
    }
    int m = (l + r) / 2;
    cdq1(l, m);
    // left f[l..m)  ->  right f[m..r)
    vector<int> A(f.begin() + l, f.begin() + m);         // A[j-l] = f[j]
    vector<int> B(g.begin(), g.begin() + (r - l));       // B[k]   = g[k], k = i-j in [1, r-l-1]
    vector<int> C = multiply(A, B);                      // C[i-l] = sum f[j] g[i-j]
    for (int i = m; i < r; i++) f[i] = (f[i] + C[i - l]) % MOD;
    cdq1(m, r);
}

// ============================================================
// PATTERN 2: offline "left key < right key" (dominance on one axis)
//     count pairs (item p, item q) with key(p) < key(q) [or <=],
//     contributing  P[value(p)] * Q[value(q)]  to  F[value(p)+value(q)]
// Split the KEY axis, NOT the value axis:
//     p's with key in [l,m)  x  q's with key in [m,r)   -> all valid
// ============================================================
//  solve(l, r):                     // half-open over KEY axis
//      if nothing on one side: return        // PRUNE
//      if r-l == 1: handle equal-key pairs directly; return
//      m = (l+r)/2
//      cross(l, m, r)                // 1 convolution
//      solve(l, m); solve(m, r)
//
//  cross():  shrink to actual value span, shift indices, convolve:
//      pa[v - vminA] = cntA[v];  pb[w - vminB] = cntB[w]
//      C = multiply(pa, pb)
//      F[vminA + vminB + k] += C[k]          // remember the offset!
//  If (#nonzero A)*(#nonzero B) is small, do the double loop instead.

// ---------------- quick self-test of PATTERN 1 ----------------
int main() {
    int N = 1000;
    mt19937 rng(7);
    g.assign(N, 0); base_.assign(N, 0);
    for (int i = 0; i < N; i++) g[i] = rng() % MOD, base_[i] = rng() % MOD;
    f = base_;
    cdq1(0, N);
    vector<ll> h(N);                                     // O(N^2) check
    for (int i = 0; i < N; i++) {
        ll s = base_[i];
        for (int j = 0; j < i; j++) s = (s + h[j] * g[i - j]) % MOD;
        h[i] = s;
    }
    bool ok = true;
    for (int i = 0; i < N; i++) ok &= (h[i] == f[i]);
    puts(ok ? "pattern 1 OK" : "pattern 1 WRONG");
}