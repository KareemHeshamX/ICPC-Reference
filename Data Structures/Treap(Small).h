mt19937 rng(1337);

template<typename T>
class Treap {
private:
    struct Node {
        T key;
        int prior, size, freq;
        Node *l, *r;
        Node(T val) : key(val), prior(rng()), size(1), freq(1), l(nullptr), r(nullptr) {}
        ~Node() { delete l; delete r; } // Cascading delete
    };

    Node* root = nullptr;

    int getsz(Node* t) { return t ? t->size : 0; }

    void pull(Node* t) {
        if (t) t->size = getsz(t->l) + getsz(t->r) + t->freq;
    }

    void split(Node* t, T key, Node*& l, Node*& r) {
        if (!t) { l = r = nullptr; return; }
        if (t->key <= key) {
            split(t->r, key, t->r, r);
            l = t;
        } else {
            split(t->l, key, l, t->l);
            r = t;
        }
        pull(t);
    }

    Node* merge(Node* l, Node* r) {
        if (!l || !r) return l ? l : r;
        if (l->prior > r->prior) {
            l->r = merge(l->r, r);
            pull(l); return l;
        } else {
            r->l = merge(l, r->l);
            pull(r); return r;
        }
    }

    bool increment(Node* t, T key) {
        if (!t) return false;
        if (t->key == key) { t->freq++; pull(t); return true; }
        bool res = increment(key < t->key ? t->l : t->r, key);
        pull(t); return res;
    }

    void erase_node(Node*& t, T key) {
        if (!t) return;
        if (t->key == key) {
            if (--t->freq == 0) {
                Node* temp = t;
                t = merge(t->l, t->r);
                temp->l = temp->r = nullptr;
                delete temp;
            }
        } else {
            erase_node(key < t->key ? t->l : t->r, key);
        }
        pull(t);
    }

    T kth(Node* t, int k) {
        int left_sz = getsz(t->l);
        if (k <= left_sz) return kth(t->l, k);
        if (k <= left_sz + t->freq) return t->key;
        return kth(t->r, k - left_sz - t->freq);
    }

    int order(Node* t, T key) {
        if (!t) return 0;
        if (key < t->key) return order(t->l, key);
        if (key == t->key) return getsz(t->l);
        return getsz(t->l) + t->freq + order(t->r, key);
    }

public:
    Treap() : root(nullptr) {}
    ~Treap() { delete root; }

    void insert(T x) {
        if (!increment(root, x)) {
            Node* l; Node* r;
            split(root, x, l, r);
            root = merge(merge(l, new Node(x)), r);
        }
    }

    void erase(T x) { erase_node(root, x); }
    T get_kth(int k) { assert(k >= 1 && k <= getsz(root)); return kth(root, k); }
    int order_of_key(T x) { return order(root, x); }
    int size() { return getsz(root); }
};