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

unsigned int randxor(){
  static unsigned int x=123456789,y=362436069,z=521288629,w=88675123;
  unsigned int t;
  t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) );
}

int Gemb_index(int y, int x, int L) {
  return y * L + x;
}

int score(vector<vector<int>> &Gadj, vector<int> &phi, int L) {
  int V = Gadj.size();
  int ret = 0;

  REP(i, V)for(int j = i + 1; j < V; ++j) {
    int ya = phi[i] / L;
    int xa = phi[i] % L;
    int yb = phi[j] / L;
    int xb = phi[j] % L;

    if(max(abs(ya - yb), abs(xa - xb)) >= 2) continue;
    ret += Gadj[i][j];
  }
  return ret;
}

void solve_4(int V, int L) {
  vector<int> phi(V);
  int cnt = 0;
  for(int y = 0; y <= 1; ++y) {
    for(int x = 0; x <= 1; ++x) {
      if(cnt >= V) continue;
      phi[cnt] = Gemb_index(y, x, L);
      ++cnt;
    }
  }
  REP(i, V) cout << i + 1 << " " << phi[i] + 1 << endl;
  return;
}

void solve_9(vector<vector<int>> &Gadj, int V, int L) {
  vector<int> phi(V);
  int score_max = -1;

  vector<int> perm = {0, 1, 2, L, L + 1, L + 2, 2 * L, 2 * L + 1, 2 * L + 2};
  do {
    vector<int> phi_tmp(V);
    REP(i, V) phi_tmp[i] = perm[i];
    int score_tmp = score(Gadj, phi_tmp, L);
    if(score_tmp > score_max) {
      score_max = score_tmp;
      REP(i, V) phi[i] = phi_tmp[i];
    }
  }while(next_permutation(ALL(perm)));

  REP(i, V) cout << i + 1 << " " << phi[i] + 1 << endl;
  return;
}

void metropolis(vector<vector<int>> &Gadj, int V, int E, int L, vector<int> &u, vector<int> &v, vector<int> &w, double T0, vector<int> &phi) {
  // When T0 <= 0, do greedy.
  int Vemb = L * L;
  // sort by total weights
  vector<int> total_weights(V, 0);
  vector<pair<int, int>> nodes(V);
  REP(i, E) {
    total_weights[u[i]] += w[i];
    total_weights[v[i]] += w[i];
  }
  int total_weights_max = -1;
  int argmaxi;
  REP(i, V) {
    if(total_weights[i] > total_weights_max) {
      total_weights_max = total_weights[i];
      argmaxi = i;
    }
  }

  phi.assign(V, -1);
  vector<int> inverse(Vemb, -1);
  vector<map<int,int>> gain(V);

  for(int i = 0; i < V; ++i) {
    int max = -1;
    int argmaxj, argmaxq;

    if(i == 0) {
      argmaxj = argmaxi;
      argmaxq = Gemb_index(L / 2, L / 2, L);
    }else{
      REP(j, V) {
        if(phi[j] != -1) continue;
        for (auto itr = gain[j].begin(); itr != gain[j].end(); ++itr) {
          //if(inverse[itr->first] != -1) continue;
          double crt;
          if(T0 > 0) {
            crt = min(1.0, exp((itr->second - max) / T0));
          }else{
            crt = (itr->second > max) ? 1 : 0;
          }

          double p = (double)randxor() / UINT_MAX;
          if(max == -1 || p <= crt) {
            max = itr->second;
            argmaxj = j;
            argmaxq = itr->first;
          }
        }
      }
    }

    phi[argmaxj] = argmaxq;
    inverse[argmaxq] = argmaxj;

    gain[argmaxj].clear();

    REP(k, 8) {
      int yk = argmaxq / L + dy[k];
      int xk = argmaxq % L + dx[k];
      if(yk < 0 || yk >= L || xk < 0 || xk >= L) continue;
      int qtmp = Gemb_index(yk, xk, L);
      if(inverse[qtmp] != -1) continue;
      REP(j, V) {
        if(phi[j] != -1) continue;
        gain[j][qtmp] += Gadj[j][argmaxj];
      }
    }
    REP(j, V) {
      if(phi[j] != -1) continue;
      auto itr = gain[j].begin();
      while(itr != gain[j].end()) {
        if(inverse[itr->first] != -1) {
          itr = gain[j].erase(itr);
        }else{
          ++itr;
        }
      }
    }
  }
  return;
}

bool is_injective(const vector<int> &phi) {
  set<int> st;
  for(int e: phi) {
    st.insert(e);
  }
  return (st.size() == phi.size());
}

bool is_well_defined(const vector<int> &inverse) {
  set<int> st;
  int cnt = 0;
  for(int e: inverse) {
    if(e == -1) continue;
    st.insert(e);
    ++cnt;
  }
  return (st.size() == cnt);
}

void swap_nodes(const vector<vector<int>> &Gadj, int V, int E, int L, vector<int> &phi){
  int Vemb = L * L;
  vector<int> inverse(Vemb, -1);
  REP(i, V) inverse[phi[i]] = i;

  int M = 5000000;
  double T0 = 0.4;
  double Tlast = 0.1;

  REP(i, M) {
    int gain = 0;
    int u = randxor() % V;
    int v = randxor() % V;
    if(u == v) continue;
    int qu = phi[u], qv = phi[v];
    REP(k, 8) {
      int qu_yk = qu / L + dy[k];
      int qu_xk = qu % L + dx[k];
      if(qu_yk < 0 || qu_yk >= L || qu_xk < 0 || qu_xk >= L) continue;
      int qktmp = Gemb_index(qu_yk, qu_xk, L);
      if(inverse[qktmp] == -1 || qktmp == qv) continue;
      gain += Gadj[v][inverse[qktmp]] - Gadj[u][inverse[qktmp]];
    }
    REP(k, 8) {
      int qv_yk = qv / L + dy[k];
      int qv_xk = qv % L + dx[k];
      if(qv_yk < 0 || qv_yk >= L || qv_xk < 0 || qv_xk >= L) continue;
      int qktmp = Gemb_index(qv_yk, qv_xk, L);
      if(inverse[qktmp] == -1 || qktmp == qu) continue;
      gain += Gadj[u][inverse[qktmp]] - Gadj[v][inverse[qktmp]];
    }

    double p = (double)randxor() / UINT_MAX;
    double T = T0 + (double)(Tlast - T0) / M * i;

    if(p <= min(1.0, exp(gain / T))) {
      phi[u] = qv;
      phi[v] = qu;
      inverse[qv] = u;
      inverse[qu] = v;
    }
  }
  return;
}

void solve_by_swap(vector<vector<int>> &Gadj, int V, int E, int L, vector<int> &u, vector<int> &v, vector<int> &w){
  vector<int> phi;
  metropolis(Gadj, V, E, L, u, v, w, 0, phi);
  cerr << score(Gadj, phi, L) << endl;

  swap_nodes(Gadj, V, E, L, phi);

  REP(i, V) cout << i + 1 << " " << phi[i] + 1 << endl;
}

void solve_more_than_9(vector<vector<int>> &Gadj, int V, int E, int L, vector<int> &u, vector<int> &v, vector<int> &w) {
  vector<int> phi;
  metropolis(Gadj, V, E, L, u, v, w, 0, phi);
  int score_max = score(Gadj, phi, L);

  //int N = 0;
  int N = 14;
  double T0 = 0.3;

  REP(i,N) {
    vector<int> phi_tmp;
    metropolis(Gadj, V, E, L, u, v, w, T0, phi_tmp);
    int score_tmp = score(Gadj, phi_tmp, L);
    //cerr << score_max << " " << score_tmp << endl;
    if(score_tmp > score_max) {
      score_max = score_tmp;
      phi = phi_tmp;
    }
  }

  REP(i, V) cout << i + 1 << " " << phi[i] + 1 << endl;
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

  if(V <= 4) {
    solve_4(V, L);
  }else if(V <= 9) {
    solve_9(Gadj, V, L);
  }else {
    //solve_more_than_9(Gadj, V, E, L, u, v, w);
    solve_by_swap(Gadj, V, E, L, u, v, w);
  }
  return 0;
}

