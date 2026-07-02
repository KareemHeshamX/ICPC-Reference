struct KMP {
	string pattern;
	vector<int> longestPrefix;
	KMP(const string& str) :pattern(str) {
		failure_function();
	}
	int fail(int k, char nxt) {
		while (k > 0 && pattern[k] != nxt)
			k = longestPrefix[k - 1];
		if (nxt == pattern[k])
			k++;
		return k;
	}
	void failure_function() {
		int n = pattern.size();
		longestPrefix = vector<int>(n);
		for (int i = 1, k = 0; i < n; i++)
			longestPrefix[i] = k = fail(k, pattern[i]);
	}
	void match(const string& str) {
		int n = str.size();
		int m = pattern.size();
		for (int i = 0, k = 0; i < n; i++) {
			k = fail(k, str[i]);
			if (k == m) {
				cout << i - m + 1 << endl; //0-based
				k = longestPrefix[k - 1]; // if you want next match
			}
		}
	}
};


vector<bool> suffix_pal(string s) { //[i..n-1] pal?
	string r = s;
	reverse(all(r));
	vector<bool> v(s.size());
	v[0] = (s == r);
	string pattern = r + "#" + s;
	int n = pattern.size();
	vector<int> longestPrefix(n);
	int k = 0;
	for (int i = 1; i < n; i++) {
		while (k > 0 && pattern[k] != pattern[i])
			k = longestPrefix[k - 1];
		if (pattern[i] == pattern[k])
			k++;
		longestPrefix[i] = k;
	}
	while (k > 0) {
		v[s.size() - k] = true;
		k = longestPrefix[k - 1];
	}
	return v;
}

vector<bool> prefix_pal(string s) { // [0..i] pal?
	string r = s;
	reverse(all(r));
	vector<bool> v(s.size());
	v.back() = (s == r);
	string pattern = s + "#" + r;
	int n = pattern.size();
	vector<int> longestPrefix(n);
	int k = 0;
	for (int i = 1; i < n; i++) {
		while (k > 0 && pattern[k] != pattern[i])
			k = longestPrefix[k - 1];
		if (pattern[i] == pattern[k])
			k++;
		longestPrefix[i] = k;
	}
	while (k > 0) {
		v[k - 1] = true;
		k = longestPrefix[k - 1];
	}
	return v;
}

//frq[i] = number of occur s[0..i] in s
vector<int> build_fre_prefix(const string& s) {
	KMP kmp(s);
	kmp.failure_function();
	vector<int> f = kmp.longestPrefix;
	int n = sz(s);
	vector<int> frq(n);
	for (int i = n - 1; i >= 0; i--)
		if (f[i])
			frq[f[i] - 1] += frq[i] + 1;
	for (auto& it : frq)it++;
	return frq;
}

void testCase() {
	string s; cin >> s;
	vector<int>lps(sz(s));
	for (int i = 1, j = 0; i < sz(s); i++) {
		while (j && s[i] != s[j]) j = lps[j - 1];
		if (s[i] == s[j]) j++;
		lps[i] = j;
	}
	vector<int>fin;
	// get all periods (ignoring partial remaining suffix)
	int crnt = lps[sz(s) - 1];
	while (crnt) {
		fin.push_back(sz(s) - crnt);
		crnt = lps[crnt - 1];
	}
	for (auto it:fin) cout << it << ' ';
	cout << sz(s);
}

// automaton
vector<vector<int>>atm(m + 1, vector<int>(26));
for (int i = 0; i <= m; i++) {
	for (int ch = 0; ch < 26; ch++) {
		if (i < m && s[i] - 'a' == ch) atm[i][ch] = i + 1;
		else atm[i][ch] = (i == 0 ? 0 : atm[pi[i - 1]][ch]);
	}
}