const int N = 1e6 + 9;
struct Node {
    Node *l, *r; int val;
    Node(int val) : l(nullptr), r(nullptr), val(val) {}
    Node(Node *l, Node*r) : l(l), r(r), val(0) {
        if (l) val += l -> val;
        if (r) val += r -> val;
    }
};

Node* getl(Node *node) { return node == nullptr ? nullptr : node -> l; }
Node* getr(Node *node) { return node == nullptr ? nullptr : node -> r; }
int getv(Node *node) { return node == nullptr ? 0 : node -> val; }

Node *update(Node *node, int pos, int val, int s = 0, int e = N) {
    if (s == e) return new Node(val + getv(node));
    int mid = (s + e) / 2;
    if (pos <= mid) return new Node(update(getl(node), pos, val, s, mid), getr(node));
    else return new Node(getl(node), update(getr(node), pos, val, mid + 1, e));
}

int query(Node *node, Node *prev, int l, int r, int s = 0, int e = N) {
    if (s > r || e < l) return 0;
    if (s >= l && e <= r) return getv(node) - getv(prev);
    int mid = (s + e) / 2;
    return query(getl(node), getl(prev), l, r, s, mid) + 
           query(getr(node), getr(prev), l, r, mid + 1, e);
}