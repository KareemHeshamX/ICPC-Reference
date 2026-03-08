//sieve--> get primes from 0 to N

const int N = 1e6 + 5;
vector <int> primes;
bool composite[N];
void sieve()
{
	composite[0] = composite[1] = 1;
	for (int i = 2; i < N; ++i)
	{
		if (composite[i])continue;
		primes.push_back(i);
		for (int j = i+i; j < N; j += i)
			composite[j] = true;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

// linear seive -->get primes from 0 to N

const int N = 1e6;
vector <int> primes;
bool composite[N];
void sieve()
{
	composite[0] = composite[1] = 1;
	for (int i = 2; i < N; ++i)
	{
		if (!composite[i])primes.push_back(i);
		for (int j = 0; j < primes.size() && i * primes[j] < N; ++j)
		{
			composite[i * primes[j]] = 1;
			if (i % primes[j] == 0) break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// get prime factors using sieve (less than sqrt(n)) متنساش كود سبف قبلها

vector<int> prime_fact(int n)
{
	vector<int>temp;
	for (int i = 0;primes[i] * 1LL * primes[i] <= n;i++)
	{
		while (n % primes[i] == 0)
			temp.push_back(primes[i]), n /= primes[i];
	}
	if (n > 1)temp.push_back(n);
	return temp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

// get divisors for all numbers from 1 to N

const int N = 1e5+5;
vector<vector<int>>divisors(N);
void generate_divisors()
{
	for (int i = 1;i < N;i++)
	{
		for(int j=i;j<N;j+=i)
			divisors[j].push_back(i);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

// Generate all primes from l to r using segmented sieve in O((r - l) log (r) + sqrt(r))

vector<ll> segmented_sieve(ll l, ll r) {
	if (l == 1) l++;
	int limit = sqrtl(r);
	while ((ll) limit * limit <= r) limit++;
	while ((ll) limit * limit > r) limit--;
	vector<bool> is_prime(r - l + 1, true);
	for (ll p : prime) {
		ll start = max((ll)p * p, (ll)(l + p - 1) / p * p);
		for (ll j = start; j <= r; j += p) {
			is_prime[j - l] = false;
		}
	}
	vector<ll> vec;
	for (ll i = l; i <= r; ++i) {
		if (is_prime[i - l]) {
			vec.push_back(i);
		}
	}
	return vec;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////

//Linear Sieve
const int N = 1e7;
int lpf[N + 1];
vector<int> prime;

void sieve() {
    for (int i = 2; i <= N; i++) {
        if (lpf[i] == 0) {
            lpf[i] = i;
            prime.push_back(i);
        }
        for (int j: prime) {
            if (j > lpf[i] || 1LL * i * j > N)break;
            lpf[i * j] = j;
        }
    }
}