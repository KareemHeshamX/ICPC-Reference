template<typename T = int, int B = 31>
struct Basis {
    array<T, B>basis;
    int sz;

    void clear() {
        for (int i = 0; i < B; i++) basis[i] = 0;
        sz = 0;
    }

    Basis() {
        clear();
    }

    void insert(T x) {
        for (int i = B - 1; i >= 0; i--) {
            if (x >> i & 1) {
                if (basis[i]) x ^= basis[i];
                else {
                    basis[i] = x;
                    sz++;
                    break;
                }
            }
        }
    }
};