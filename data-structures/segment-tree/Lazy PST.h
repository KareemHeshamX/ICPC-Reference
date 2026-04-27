struct segtree {
    segtree *left, *right;
    bool dirty = false;
    ll sum = 0, lazy = 0;
 
    segtree(ll val = 0) : sum(val) {
        left = right = this;
    }
 
    segtree(segtree *L, segtree *R) : left(L), right(R) {
        sum = L->sum + R->sum;
    }
};
 
class persistent_segment_tree {
#define MID ((start+end)>>1)
 
    segtree *apply(segtree *root, int start, int end, int val) {
        segtree *rt = new segtree(*root);
        rt->dirty = true;
        rt->sum += (end - start + 1) * val;
        rt->lazy = val;
        return rt;
    }
 
    void pushdown(segtree *root, int start, int end) {
        if (!root->dirty || start == end) return;
        root->left = apply(root->left, start, MID, root->lazy);
        root->right = apply(root->right, MID + 1, end, root->lazy);
        root->lazy = 0;
        root->dirty = false;
    }
 
    segtree *build(int start, int end, const vector<int> &v) {
        if (start == end) return new segtree(v[start]);
        return new segtree(build(start,MID, v), build(MID + 1, end, v));
    }
 
    segtree *update_range(segtree *root, int start, int end, int l, int r, int val) {
        pushdown(root, start, end);
        if (r < start || end < l) return root;
        if (l <= start && end <= r) return apply(root, start, end, val);
        return new segtree(
            update_range(root->left, start, MID, l, r, val),
            update_range(root->right, MID + 1, end, l, r, val)
        );
    }
 
    ll query_range(segtree *root, int start, int end, int l, int r) {
        pushdown(root, start, end);
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return root->sum;
        return query_range(root->left, start, MID, l, r)
               + query_range(root->right, MID + 1, end, l, r);
    }
 
    int query_kth(segtree *u, segtree *v, int start, int end, int k) {
        if (start == end) return start;
        int left_count = v->left->sum - u->left->sum;
        if (left_count >= k)
            return query_kth(u->left, v->left, start, MID, k);
        return query_kth(u->right, v->right, MID + 1, end, k - left_count);
    }
 
public:
    int start, end;
    vector<segtree *> versions;
 
    persistent_segment_tree(const vector<int> &v) {
        start = 0;
        end = 1e6 + 1;
        versions.push_back(build(start, end, v));
    }
 
    persistent_segment_tree(int s, int e) {
        start = s;
        end = e;
        versions.push_back(new segtree());
    }
 
    void update(int version_id, int l, int r, int val) {
        versions[version_id] =
                update_range(versions[version_id], start, end, l, r, val);
    }
 
    int query(int t, int l, int r) {
        return query_range(versions[t], start, end, l, r);
    }
 
    int get_kth(int u_version, int v_version, int k) {
        return query_kth(versions[u_version], versions[v_version], start, end, k);
    }
};
#undef MID