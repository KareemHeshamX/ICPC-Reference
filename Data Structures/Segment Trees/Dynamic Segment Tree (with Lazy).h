struct Node {
    ll val = 0;
    Node(ll v = 0) : val(v) {}
} neutral;

Node merge(Node a, Node b) {
    Node ret;
    ret.val = a.val + b.val;
    return ret;
}

struct segtree {
    segtree *left = nullptr, *right = nullptr;
    Node node;
    ll lazy = 0; // Lazy tag
    int start, end;

    segtree(int l = 0, int r = 0) : start(l), end(r) {}

    void extend() {
       if (left == nullptr) {
          int mid = start + ((end - start) >> 1);
          left = new segtree(start, mid);
          right = new segtree(mid + 1, end);
       }
    }

    // Pushes pending updates to child nodes
    void pushdown() {
        if (lazy != 0) {
            extend(); // Ensure children exist before pushing down

            // Apply lazy value to left child (val * length of segment)
            left->node.val += lazy * (left->end - left->start + 1);
            left->lazy += lazy;

            // Apply lazy value to right child (val * length of segment)
            right->node.val += lazy * (right->end - right->start + 1);
            right->lazy += lazy;

            // Clear current node's lazy tag
            lazy = 0;
        }
    }

    Node pushup(Node a, Node b) {
       return merge(a, b);
    }

    // Range update: Adds 'val' to all elements in range [l, r]
    void update(int l, int r, ll val) {
       if (start > r || end < l) return;

       // If current segment is completely within the update range
       if (start >= l && end <= r) {
          node.val += val * (end - start + 1);
          lazy += val;
          return;
       }

       extend();
       pushdown(); // Push pending updates down before modifying children

       left->update(l, r, val);
       right->update(l, r, val);
       node = pushup(left->node, right->node);
    }

    Node query(int l, int r) {
       if (r < start || end < l) return neutral;

       // If current segment is completely within the query range
       if (l <= start && end <= r) return node;

       extend();
       pushdown(); // Push pending updates down to ensure children are accurate

       return pushup(left->query(l, r), right->query(l, r));
    }

    ~segtree() {
       delete left;
       delete right;
    }
};