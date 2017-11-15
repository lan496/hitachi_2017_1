#include <bits/stdc++.h>
using namespace std;

#define REP(i,n) for(int i=0;i<(int)(n);i++)
#define ALL(x) (x).begin(), (x).end()

typedef long long ll;
typedef long double ld;

const int INF = 1e9;
const ld EPS = 1e-8;

int Gemb_index(int y, int x, int L) {
  return y * L + x;
}

int main(){
  // read
  int V, E, Vemb, Eemb;
  cin >> V >> E;
  vector<int> u(E), v(E), w(E);
  REP(i, E) cin >> u[i] >> v[i] >> w[i];
  cin >> Vemb >> Eemb;
  vector<int> a(Eemb), b(Eemb);
  REP(i, Eemb) cin >> a[i] >> b[i];
  int L = sqrt(Vemb);

  // sort by total weights
  vector<int> total_weights(V, 0);
  vector<pair<int, int>> nodes(V);
  REP(i, E) {
    total_weights[u[i]] += w[i];
    total_weights[v[i]] += w[i];
  }
  REP(i, V) {
    nodes[i] = make_pair(total_weights[i], i);
  }
  sort(nodes.rbegin(), nodes.rend());

  REP(i, V) {
    cout << nodes[i].first << " " << nodes[i].second << endl;
  }

  vector<vector<bool>> used(L, vector<bool> (L, false));
  queue<pair<int,int>> que;

  vector<int> s(V), t(V);

  que.push(make_pair(L / 2, L / 2));
  used[L/2][L/2] = true;
  s[0] = nodes[0].second;
  t[0] = Gemb_index(L/2, L/2, L);

  for(int i = 0; i < V; ++i) {
    int y, x;
    if(i == 0) {
      y = L / 2;
      x = L / 2;
    }else{
      while(1) {
        auto q = que.front(); que.pop();
        y = q.second / L;
        x = q.second % L;
        if(!used[y][x]) break;
      }
    }
    used[y][x] = true;
    s[i] = nodes[i].second;
    t[i] = Gemb_index(y, x, L);

    for(int dy = -1; dy <= 1; ++dy) {
      for(int dx = -1; dx <= 1; ++dx) {
        if(abs(dx) + abs(dy) == 0 || abs(dx) + abs(dy) > 2) continue;
        int yi = y + dy, xi = x + dx;
        if(yi < 0 || yi >= L || xi < 0 || xi >= L) continue;
        que.push(make_pair(yi, xi));
      }
    }
  }

  REP(i, V) cout << s[i] + 1 << " " << t[i] + 1 << endl;

  return 0;
}

