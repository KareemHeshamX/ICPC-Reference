#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

#define ordered_set tree<ll, null_type, less<ll>, rb_tree_tag, tree_order_statistics_node_update>

// oset.find_by_order(x) --> value in index x
// oset.order_of_key(x) --> index of first value of x


bool Exist(ordered_set &st,int x) {
    if((st.upper_bound(x)) == st.end()) return 0;
    return ((*st.upper_bound(x)) == x);
}

void Erase(ordered_set &s,int x) {
    if(Exist(s,x)) s.erase(s.upper_bound(x));
}