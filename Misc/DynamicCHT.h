typedef int64_t ll;

const ll inf = ll(4e18);
struct Line {
    mutable ll k, m, p;
    bool operator<(const Line &o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};

struct HullDynamic : multiset<Line, less<> > {
    // (for doubles, use inf = 1/.0, div(a,b) = a/b)
    ll type = -1; //1 -> maximum, -1 -> minimum
    ll div(ll a, ll b) {
        // floored division
        return a / b - ((a ^ b) < 0 && a % b);
    }

    bool isect(iterator x, iterator y) {
        if (y == end()) {
            x->p = inf;
            return false;
        }
        if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
        else x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }

    void add(ll k, ll m) {
        k *= type;
        m *= type;
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            isect(x, erase(y));
    }

    ll query(ll x) {
        if (empty())return -inf;
        auto l = *lower_bound(x);
        return type * (l.k * x + l.m);
    }
};