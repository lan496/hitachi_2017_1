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

class state{
public:
  vector<int> phi;
  vector<int> inverse;
  set<int> cnd;
  state() : phi(), inverse(), cnd() {}
  state(vector<int> &phi, vector<int> &inverse, set<int> &cnd) : phi(phi), inverse(inverse), cnd(cnd) {}
  state(const state &rhs) : phi(rhs.phi), inverse(rhs.inverse), cnd(rhs.cnd) {}
  ~state() {}
};

bool comp(pair<int,state> &lhs, pair<int,state> &rhs) {
  return lhs.first > rhs.first;
}

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
  set<int> cnd;

  // first mapped node
  phi[nodes[0].second] = Gemb_index(L / 2, L / 2, L);
  inverse[Gemb_index(L / 2, L / 2, L)] = nodes[0].second;
  REP(k, 8) {
    int yk = L / 2 + dy[k];
    int xk = L / 2 + dx[k];
    if(yk < 0 || yk >= L || xk < 0 || xk >= L) continue;
    int qtmp = Gemb_index(yk, xk, L);
    cnd.insert(qtmp);
  }

  int width = 2;
  vector<pair<int,state>> beam;

  beam.push_back(make_pair(0, state(phi, inverse, cnd)));

  for(int i = 1; i < V; ++i) {
    vector<pair<int,state>> beam_tmp;
    for (auto b : beam){
      REP(j, V) {
        if(b.second.phi[j] != -1) continue;
        for (int q : b.second.cnd) {
          if(b.second.inverse[q] != -1) {
            b.second.cnd.erase(q);
            continue;
          }
          int gain = b.first;
          REP(k, 8) {
            int yk = q / L + dy[k];
            int xk = q % L + dx[k];
            if(yk < 0 || yk >= L || xk < 0 || xk >= L) continue;
            int qtmp = Gemb_index(yk, xk, L);
            if(b.second.inverse[qtmp] == -1) continue;
            gain += Gadj[j][inverse[qtmp]];
          }
          // add temp. state
          vector<int> phi_tmp(b.second.phi);
          vector<int> inverse_tmp(b.second.inverse);
          set<int> cnd_tmp(b.second.cnd);
          phi_tmp[j] = q;
          inverse_tmp[q] = j;
          cnd_tmp.erase(q);
          REP(k, 8) {
            int yk = q / L + dy[k];
            int xk = q % L + dx[k];
            if(yk < 0 || yk >= L || xk < 0 || xk >= L) continue;
            int qtmp = Gemb_index(yk, xk, L);
            cnd_tmp.insert(qtmp);
          }
          beam_tmp.push_back(make_pair(gain, state(phi_tmp, inverse_tmp, cnd_tmp)));
        }
      }
    }
    partial_sort(beam_tmp.begin(), beam_tmp.begin() + width, beam_tmp.end(), comp);
    beam.clear();
    REP(j, width) beam.push_back(beam_tmp[j]);
  }

  REP(i, V) cout << i + 1 << " " << beam[0].second.phi[i] + 1 << endl;

  return 0;
}

