#define ot long double
#define ld long double
#define vec(a, b) ((b)-(a))
#define len(v) (hypotl((v).y, (v).x))

const ld eps = 1e-12, PI = acosl(-1.0);

struct P {
   ot x, y;

   void read() {
      cin >> x >> y;
   }

   P operator-(P const &he) const {
      return {x - he.x, y - he.y};
   }

   // cross
   ot operator^(P const &he) const {
      return x * he.y - y * he.x;
   }

   // dot
   ot operator*(P const &he) const {
      return x * he.x + y * he.y;
   }
};

istream& operator>>(istream &in, P &p) {
   return in >> p.x >> p.y;
}

ostream& operator<<(ostream &out, P &p) {
   return out << p.x << ' ' << p.y;
}