const int N = 1e5+9;
struct Node{
    Node* l, *r;
    int s;
    Node(ll s = 0, Node* l = NULL, Node* r = NULL):
        s(s) , l(l) , r(r){} 
};
Node* getL(Node* x){ return x == NULL ? x : x->l; }
Node* getR(Node* x){ return x == NULL ? x : x->r; }
int   getS(Node* x){ return x == NULL ? 0 : x->s; }
Node* newLeaf(int val){ return new Node(val); }
 
Node* newPar(Node* l, Node* r){
    Node* res = new Node();
    res->l = l; res->r = r;
    res->s = getS(l) + getS(r);
    return res;
} 
 
Node* upd(int i , int v , Node* x , int l = 0 , int r = N){
    if(r - l == 1) return newLeaf( v + getS(x) );
    int m = (l+r)/2;
    if(i < m){
        return newPar(upd(i,v,getL(x),l,m), getR(x));
    }else{
        return newPar(getL(x) , upd(i,v,getR(x),m,r));
    }
}
 
int sumSeg(Node*u , Node*v , Node* lca, Node* par_lca){
    return getS(u) + getS(v) - getS(lca) - getS(par_lca);
}
 
int kth(int k , Node*u , Node*v, Node* lca, Node* par_lca, int l = 0 , int r = N){
    if(r-l==1) return l;
    int m = (l+r)/2;
 
    int sumL = sumSeg( getL(u)  , getL(v)  , getL(lca) , getL(par_lca) );
 
    if(sumL >= k) 
        return kth(k  , getL(u)  , getL(v), getL(lca), getL(par_lca) , l , m);
    else
        return kth(k-sumL , getR(u) , getR(v) , getR(lca) , getR(par_lca) , m , r);
}