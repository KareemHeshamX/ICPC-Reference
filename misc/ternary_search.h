vector<int> a;
int can(int mid) {
    ll sum = 0;
    for (auto i: a)
        sum += abs(i - mid);
    return sum;
}
 
int ternary_search(int l, int r) {
    int ans, cnt = 500;
    while (cnt--) {
        int mid1 = l + (r - l) / 3;
        int mid2 = r - (r - l) / 3;
        if (can(mid1) < can(mid2)) r = mid2;
        else l = mid1;
    }
    for (int i = l; i <= r; i++)
        ans = min(ans, can(i));
    return ans;
}