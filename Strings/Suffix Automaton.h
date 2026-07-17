struct suffix_automaton {
    static const int alpha = 26, offset = 'a';
    struct state {
        int len = 0, link = 0, cnt = 0;
        int next[alpha];
        state(int len = 0) : len(len) {
            memset(next, -1, sizeof(next));
        }
        state(int len, const state& other) : len(len) {
            link = other.link;
            for(int i = 0; i < alpha; i++) {
                next[i] = other.next[i];
            }
        }
    };
 
    vector<state> st;
    vector<ll> dp;
    int last = 0;
    suffix_automaton() {
        st.push_back(state());
        st[0].link = -1;
    }
 
    suffix_automaton(const string &s) : suffix_automaton() {
        for (char ch : s) extend(ch - offset);
        dp = vector<ll>(sz(st), -1);
        calc_number_of_occurrences();
    }
 
    void extend(int c) {
        int cur = sz(st), p = last;
        st.push_back(state(st[last].len + 1));
        st[cur].cnt = 1;
        for(; ~p && st[p].next[c] == -1; p = st[p].link) {
            st[p].next[c] = cur;
        }
        if(p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if(st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = sz(st);
                st.push_back(state(st[p].len + 1, st[q]));
                for(; ~p && st[p].next[c] == q; p = st[p].link) {
                    st[p].next[c] = clone;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }
 
    void calc_number_of_occurrences() {
        vector<pair<int, int>> v;
        for (int i = 1; i < sz(st); i++)
            v.emplace_back(st[i].len, i);
 
        sort(v.begin(), v.end(), greater<>());
 
        for (int i = 0; i < sz(st) - 1; i++) {
            int suf = st[v[i].second].link;
            st[suf].cnt += st[v[i].second].cnt;
        }
    }
 
    ll num_different_substrings() {
        ll ans = 0;
        for(int i = 1; i < sz(st); i++) {
            ans += st[i].len - st[st[i].link].len;
        }
        return ans;
    }
 
    ll rec(int i) {
        auto& ret = dp[i];
        if(~ret) return ret;
        ret = !!i;
        for(int j = 0; j < alpha; j++) {
            if(~st[i].next[j]) {
                ret += rec(st[i].next[j]);
            }
        }
        return ret;
    }
 
    string kth_substring(ll k) { // non-repeated
        assert(k <= rec(0));
        string ans;
        int cur = 0;
        while(k > 0) {
            for(int c = 0; c < 26; c++) {
                if(st[cur].next[c] == -1) continue;
                int j = st[cur].next[c];
                if(dp[j] >= k) {
                    ans += (char)(c + offset); cur = j;
                    k--; break;
                }
                k -= dp[j];
            }
        }
        return ans;
    }
};

// GSAM (Generalized SAM for multiple strings)

// --- NEW: Data structures for string IDs ---
set<int> ids;
int distinct_count = 0; // Will hold the final count of unique strings

// constructor
suffix_automaton(const vector<string> &strings) : suffix_automaton() {
    // 1. Build and Tag
    for (int i = 0; i < sz(strings); i++) {
        int current_last = 0;
        for (char ch : strings[i]) {
            current_last = extend(ch - offset, current_last);
            // Tag this state with the current string ID
            st[current_last].ids.insert(i);
        }
    }

    // 2. Propagate up the link tree
    propagate_ids();
}

// extend function
if (st[last_node].next[c] != -1) {
    int q = st[last_node].next[c];
    if (st[last_node].len + 1 == st[q].len) {
        return q;
    }

    int clone = sz(st);
    st.push_back(state(st[last_node].len + 1, st[q]));
    st[clone].cnt = 0;

    for(int p = last_node; ~p && st[p].next[c] == q; p = st[p].link) {
        st[p].next[c] = clone;
    }
    st[q].link = clone;
    return clone;
} // else add normal extend


// --- NEW: Small-to-Large Merging Logic ---
void propagate_ids() {
    // Create an array of state indices
    vector<int> order(sz(st));
    iota(order.begin(), order.end(), 0);

    // Sort states by length descending (leaves to root)
    sort(order.begin(), order.end(), [&](int a, int b) {
        return st[a].len > st[b].len;
    });

    // Traverse from longest to shortest
    for (int u : order) {
        // Save the final count for this state before we swap/modify its set
        st[u].distinct_count = st[u].ids.size();

        int p = st[u].link;
        if (p != -1) {
            // Small-to-Large Merging: Always merge the smaller set into the larger one
            if (st[u].ids.size() > st[p].ids.size()) {
                swap(st[u].ids, st[p].ids); // O(1) swap
            }
            for (int id : st[u].ids) {
                st[p].ids.insert(id);
            }
            // Optional: Clear the set to save memory since we don't need it anymore
            st[u].ids.clear();
        }
    }
}