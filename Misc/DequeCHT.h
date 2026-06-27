#define ll long long

// If the problem asks for MINIMUM
// cht.add(-m, -c)
// dp[i] = -cht.query(x)

// If the slopes are strictly DECREASING
// cht.add(-m, c)
// cht.query(-x)

struct DequeCHT {
    struct Line { ll m, c; };
    vector<Line> dq;
    int head, tail;

    // Pre-allocate the exact size needed to avoid resizing overhead
    DequeCHT(int N) {
        dq.resize(N + 5);
        head = 0;
        tail = 0;
    }

    inline ll get(Line l, ll x) {
        return l.m * x + l.c;
    }

    // long double is crucial here to prevent large integer multiplication overflow
    inline long double intersect(Line l1, Line l2) {
        return (long double)(l2.c - l1.c) / (l1.m - l2.m);
    }

    // Add a new line (Assumes slopes are strictly INCREASING)
    void add(ll m, ll c) {
        Line l = {m, c};
        while (tail - head >= 2) {
            // If the intersection of (new line, 2nd-to-last) happens BEFORE
            // the intersection of (last, 2nd-to-last), the last line is useless.
            if (intersect(dq[tail - 2], l) <= intersect(dq[tail - 2], dq[tail - 1])) {
                tail--; // Pop back
            } else {
                break;
            }
        }
        dq[tail++] = l; // Push back
    }

    // Query the maximum value at x (Assumes x is strictly INCREASING)
    ll query(ll x) {
        if (head == tail) return -2e18; // Empty check

        while (tail - head >= 2) {
            // For MAX: If the next line is better than the current line at x, move forward
            if (get(dq[head], x) <= get(dq[head + 1], x)) {
                head++; // Pop front
            } else {
                break;
            }
        }
        return get(dq[head], x);
    }
};