#include <bits/stdc++.h>
using namespace std;

#define REP(i,n) for(int i=0;i<(int)(n);i++)
#define ALL(x) (x).begin(), (x).end()

typedef long long ll;
typedef long double ld;

const int INF = 1e9;
const ld EPS = 1e-8;

int dx[] = {1, 1, 0, -1, -1, -1, 0, 1};
int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

int Gemb_index(int y, int x, int L) {
  return y * L + x;
}

int main(){
  // read
  int V, E, Vemb, Eemb;
  cin >> V >> E;
  vector<int> u(E), v(E), w(E);
  REP(i, E) {
    cin >> u[i] >> v[i] >> w[i];
    --u[i]; --v[i];
  }
  cin >> Vemb >> Eemb;
  vector<int> a(Eemb), b(Eemb);
  REP(i, Eemb) {
    cin >> a[i] >> b[i];
    --a[i]; --b[i];
  }
  int L = sqrt(Vemb);

  // adjacency matrix for G
  vector<vector<int>> Gadj(V, vector<int> (V, 0));
  REP(i, E) {
    Gadj[u[i]][v[i]] = w[i];
    Gadj[v[i]][u[i]] = w[i];
  }

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

  vector<int> phi(V, -1);
  vector<int> inverse(Vemb, -1);
  phi[nodes[0].second] = Gemb_index(L / 2, L / 2, L);
  inverse[Gemb_index(L / 2, L / 2, L)] = nodes[0].second;

  set<int> cdd;
  REP(k, 8) {
    int yk = L / 2 + dy[k];
    int xk = L / 2 + dx[k];
    if(yk < 0 || yk >= L || xk < 0 || xk >= L) continue;
    int qtmp = Gemb_index(yk, xk, L);
    cdd.insert(qtmp);
  }

  for(int i = 1; i < V; ++i) {
    int max = -1;
    int argmaxj, argmaxq;
    REP(j, V) {
      if(phi[j] != -1) continue;
      for (int q : cdd) {
        if(inverse[q] != -1) {
          cdd.erase(q);
          continue;
        }
        int gain = 0;
        REP(k, 8) {
          int yk = q / L + dy[k];
          int xk = q % L + dx[k];
          if(yk < 0 || yk >= L || xk < 0 || xk >= L) continue;
          int qtmp = Gemb_index(yk, xk, L);
          if(inverse[qtmp] == -1) continue;
          gain += Gadj[j][inverse[qtmp]];
        }
        if(gain > max) {
          max = gain;
          argmaxj = j;
          argmaxq = q;
        }
      }
    }
    phi[argmaxj] = argmaxq;
    inverse[argmaxq] = argmaxj;

    cdd.erase(argmaxq);
    REP(k, 8) {
      int yk = argmaxq / L + dy[k];
      int xk = argmaxq % L + dx[k];
      if(yk < 0 || yk >= L || xk < 0 || xk >= L) continue;
      int qtmp = Gemb_index(yk, xk, L);
      cdd.insert(qtmp);
    }
  }

  REP(i, V) cout << i + 1 << " " << phi[i] + 1 << endl;

  return 0;
}

