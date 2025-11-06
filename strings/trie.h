struct trie {
	trie* next[26]{};
	trie* root = this;
	bool endOfWord = false;
	void insert(string &s) {
		trie *current = this;
		for (char &ch : s) {
			int i = ch - 'a';
			if (current -> next[i] == nullptr) {
				current -> next[i] = new trie;
			}
			current = current -> next[i];
		}
		current -> endOfWord = true;
	}
	bool search(string &s) {
		trie *current = this;
		for (char &ch : s) {
			int i = ch - 'a';
			if (current -> next[i] == nullptr) {
				return false;
			}
			current = current -> next[i];
		}
		return current -> endOfWord;
	}
	bool erase(string& s, int idx = 0) {
		if (idx == s.size()) {
			if (!endOfWord) return false;
			endOfWord = false;
			for (auto child : next)
				if (child) return false;
			return true;
		}
		int i = s[idx] - 'a';
		if (!next[i]) return false;
		bool shouldDeleteChild = next[i]->erase(s, idx + 1);
		if (shouldDeleteChild) {
			delete next[i];
			next[i] = nullptr;
			for (auto child : next)
				if (child) return false;
			return !endOfWord;
		}
		return false;
	}
};