template <typename T, typename Compare = std::less<T>>
struct MonoQueue {
    // Stores {value, original_index}
    deque<pair<T, int>> dq;
    Compare cmp;

    // 1. Push a new element and maintain monotonicity
    void push(T val, int idx) {
        // If cmp is std::less (default), it pops when dq.back() < val.
        // If you need strictly decreasing/increasing, use std::less_equal / std::greater_equal.
        while (!dq.empty() && cmp(dq.back().first, val)) {
            dq.pop_back();
        }
        dq.push_back({val, idx});
    }

    // 2. Pop elements that fall outside your valid window or constraint
    void pop_outdated(int left_limit_idx) {
        while (!dq.empty() && dq.front().second < left_limit_idx) {
            dq.pop_front();
        }
    }

    // 3. Query the optimal value
    T get_opt() const {
        return dq.front().first;
    }

    // 4. Query the index of the optimal value
    int get_opt_idx() const {
        return dq.front().second;
    }

    bool empty() const {
        return dq.empty();
    }

    void clear() {
        dq.clear();
    }
};