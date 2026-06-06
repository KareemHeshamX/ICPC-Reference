// get mobius value of n in O(sqrt(N))
int mobius(int n) {
    if (n == 1) return 1;
    int cnt = 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            cnt++;
            n /= i;
            if (n % i == 0) { // n is not square-free
                return 0; 
            }
        }
    }
    if (n > 1) cnt++;
    return (cnt % 2 == 0) ? 1 : -1; // even prime -> 1, odd primes -> 1
}

// compute mobius array up to n in O(N*log(log(N)))

vector<int> mobius(int n) {
    vector<int> mob(n + 1, 1), prime(n + 1, 1);
    prime[0] = prime[1] = 0;
    for (int i = 2; i <= n; i++) {
        if (prime[i]) {
            mob[i] = -1;
            for (int j = i + i; j <= n; j += i) {
                prime[j] = 0;
                if (j % (1LL * i * i)) mob[j] *= -1;
                else mob[j] = 0;
            }            
        }
    }
    return mob;
}