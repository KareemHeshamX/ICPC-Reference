template <typename T, typename Compare = std::less<T>>
struct MonoQueue {
    deque<pair<T, int>> dq;
    Compare cmp;

    void push(T val, int idx) {
        while (!dq.empty() && cmp(dq.back().first, val)) {
            dq.pop_back();
        }
        dq.push_back({val, idx});
    }

    void pop_outdated(int left_limit_idx) {
        while (!dq.empty() && dq.front().second < left_limit_idx) {
            dq.pop_front();
        }
    }

    T get_opt() const { return dq.front().first; }
    bool empty() const { return dq.empty(); }
    void clear() { dq.clear(); }
};