int phi(int n) {
    int res = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            res -= res / i;
        }
    }
    if (n > 1)
        res -= res / n;
    return res;
}

// Euler totient function from 1 to n in O(n*log(log(n)))
vector<int> phi_1_to_n(int n) {
    vector<int> phi(n + 1);
    for (int i = 0; i <= n; i++)
        phi[i] = i;

    for (int i = 2; i <= n; i++) {
        if (phi[i] == i) {
            for (int j = i; j <= n; j += i)
                phi[j] -= phi[j] / i;
        }
    }
    return phi;
}

// Euler totient function from 1 to n in O(n) using linear sieve

const int N = 1e7;
int phi[N + 1];
bool is_prime[N + 1];
vector<int> primes;

void totient() {
    for (int i = 2; i <= N; i++) {
        is_prime[i] = true;
    }
    phi[1] = 1;
    for (int i = 2; i <= N; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            phi[i] = i - 1;
        }
        for (int j = 0; j < sz(primes) && i * primes[j] <= N; j++) {
            int p = primes[j];
            is_prime[i * p] = false;
            if (i % p == 0) {
                phi[i * p] = phi[i] * p;
                break;
            } else {
                phi[i * p] = phi[i] * (p - 1);
            }
        }
    }
}