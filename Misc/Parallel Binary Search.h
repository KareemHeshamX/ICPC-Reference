//Parallel Binary Search (offline)
//For each query q finds the smallest t in [lo, hi] with check(q, t) == true,
//or hi + 1 if there is none. check must be monotone in t (false..false true..true).
//O(log(T) * (T * apply + Q * check)), T = hi - lo + 1
struct Parallel_BS {//queries are 0-based: 0..Q-1, times are lo..hi
    int Q, lo, hi;
    vector<int> L, R;//answer of query q lies in [L[q], R[q]]
    Parallel_BS(int Q, int lo, int hi)
        :Q(Q), lo(lo), hi(hi), L(Q, lo), R(Q, hi + 1) {
    }
    //reset()     : empty the structure
    //apply(t)    : add event t to the structure
    //check(q, t) : predicate for query q with events lo..t applied
    template<class Reset, class Apply, class Check>
    vector<int> run(Reset reset, Apply apply, Check check) {
        int T = hi - lo + 1;
        vector<int> head(T), nxt(Q);
        while (true) {
            fill(head.begin(), head.end(), -1);
            int last = lo - 1;
            for (int q = 0; q < Q; q++)
                if (L[q] < R[q]) {
                    int mid = (L[q] + R[q]) >> 1;//lo <= mid <= hi
                    nxt[q] = head[mid - lo];
                    head[mid - lo] = q;
                    last = max(last, mid);
                }
            if (last < lo)break;//every query is finished
            reset();
            for (int t = lo; t <= last; t++) {
                apply(t);
                for (int q = head[t - lo]; q != -1; q = nxt[q])
                    if (check(q, t))R[q] = t;
                    else L[q] = t + 1;
            }
        }
        return L;//ans[q] in [lo, hi], or hi + 1 if never true
    }
};