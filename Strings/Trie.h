struct trie {
	trie* nxt[26]{};
	bool endOfWord = false;
	void insert(const string& s) {
		trie* current = this;
		for (auto ch : s) {
			int i = ch - 'a';
			if (current->nxt[i] == nullptr) current->nxt[i] = new trie;
			current = current->nxt[i];
		}
		current->endOfWord = true;
	}
	bool search(const string& s) {
		trie* current = this;
		for (auto ch : s) {
			int i = ch - 'a';
			if (current->nxt[i] == nullptr) return false;
			current = current->nxt[i];
		}
		return current->endOfWord;
	}
};

struct trie {
	trie* next[2]{};
	int prefix = 0;
	trie* root = this;
	void insert(int x) {
		trie* current = this;
		for (int i = 30; i >= 0; i--) {
			int bit = (x >> i) & 1;
			if (current -> next[bit] == nullptr) {
				current -> next[bit] = new trie;
			}
			current = current -> next[bit];
			current -> prefix++;
		}
	}
	void erase(int x, trie* current, int i = 30) {
		if (i == -1) return;
		int bit = (x >> i) & 1;
		erase(x, current -> next[bit], i - 1);
		current -> next[bit] -> prefix--;
		if (current -> next[bit] -> prefix == 0) {
			delete current -> next[bit];
			current -> next[bit] = nullptr;
		}
	}
	int max_xor(int x, trie* current, int i = 30) {
		if (i == -1 || !current) return 0;
		int bit = (x >> i) & 1;
		int ret = 0;
		if (current->next[1 - bit] && current->next[1 - bit]->prefix > 0) ret |= ((1 << i) | max_xor(x, current->next[1 - bit], i - 1));
		else ret |= max_xor(x, current->next[bit], i - 1);
		return ret;
	}
};

// Trie pools (Indexed)

const int MAXN = 3e7, oo = 1e9;
int K;

struct Node {
	int nxt[2], pref, best;
	Node() {reset();}
	void reset() {
		nxt[0] = nxt[1] = -1;
		best = -oo;
		pref = 0;
	}
};

static Node pool[MAXN];
static int ptr;
static vector<int> freeList;

int new_node() {
	if (!freeList.empty()) {
		int id = freeList.back();
		freeList.pop_back();
		pool[id].reset();
		return id;
	}
	assert(ptr < MAXN);
	pool[ptr].reset();
	return ptr++;
}

void init_trie() {
	ptr = 0;
	freeList.clear();
	new_node(); // root will be index 0
}

void insert_trie(ll x, int add) {
	int cur = 0;
	vector<int>path = {0};
	pool[0].best = -oo;
	for (int b = 0; b < K; b++) {
		int bit = x >> b & 1;
		if (pool[cur].nxt[bit] == -1) pool[cur].nxt[bit] = new_node();
		cur = pool[cur].nxt[bit];
		path.push_back(cur);
		pool[cur].pref += add;
		pool[cur].best = -oo;
		if (!pool[cur].pref) pool[cur].best = 0;
	}
	path.pop_back();
	while (!path.empty()) {
		int node = path.back();
		for (auto child:pool[node].nxt) {
			if (child == -1) {
				pool[node].best = max(pool[node].best, pool[node].pref);
				continue;
			}
			pool[node].best = max(pool[node].best, pool[child].best + pool[node].pref);
		}
		path.pop_back();
	}
}