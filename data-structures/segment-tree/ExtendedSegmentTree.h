struct Node {
	int val = 1;
} neutral;

struct segtree {
	segtree *left = nullptr, *right = nullptr;

	Node node = {};

	int start, end;

	segtree(int l = 0, int r = 0) : start(l), end(r) {}

	void extend() {
		if (left == nullptr) {
			int mid = start + end >> 1;
			left = new segtree(start, mid);
			right = new segtree(mid + 1, end);
		}
	}

	Node pushup(Node a, Node b) {
		Node ret;
		ret.val = a.val + b.val;
		return ret;
	}

	void update(int idx, int val) {
		if (start > idx || end < idx)
			return;
		if (start == end) {
			node.val = val;
			return;
		}
		extend();
		left->update(idx, val);
		right->update(idx, val);
		node = pushup(left->node, right->node);
	}

	Node query(int l, int r) {
		if (r < start || end < l)
			return neutral;
		extend();
		if (l <= start && end <= r)
			return node;
		Node ret = pushup(left->query(l , r), right->query(l , r));
		return ret;
	}

	~segtree() {
		if (left == nullptr)return;
		delete left;
		delete right;
	}
};