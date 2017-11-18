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

bool comp(tuple<int,int,int,int> &lhs, tuple<int,int,int,int> &rhs) {
  return get<0>(lhs) > get<0>(rhs);
}

int Gemb_index(int y, int x, int L) {
  return y * L + x;
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
  return ((int)st.size() == cnt);
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

  vector<int> phi_first(V, -1);
  vector<int> inverse_first(Vemb, -1);
  set<int> cnd_first;

  // first mapped node
  phi_first[nodes[0].second] = Gemb_index(L / 2, L / 2, L);
  inverse_first[Gemb_index(L / 2, L / 2, L)] = nodes[0].second;
  REP(k, 8) {
    int yk = L / 2 + dy[k];
    int xk = L / 2 + dx[k];
    if(yk < 0 || yk >= L || xk < 0 || xk >= L) continue;
    int qtmp = Gemb_index(yk, xk, L);
    cnd_first.insert(qtmp);
  }

  int width = 2;
  vector<pair<int,state*>> beam;

  state* first_node = new state(phi_first, inverse_first, cnd_first);
  beam.push_back(make_pair(0, first_node));

  for(int i = 1; i < V; ++i) {
    vector<tuple<int,int,int,int>> gain_tmp; // (gain, j, q, beamIdx)
    for (auto itr = beam.begin(); itr != beam.end(); ++itr){
      REP(j, V) {
        state* s = itr->second;
        if(s->phi[j] != -1) continue;
        for (int q : s->cnd) {
          if(s->inverse[q] != -1) {
            s->cnd.erase(q);
            continue;
          }
          int gain = itr->first;
          REP(k, 8) {
            int yk = q / L + dy[k];
            int xk = q % L + dx[k];
            if(yk < 0 || yk >= L || xk < 0 || xk >= L) continue;
            int qtmp = Gemb_index(yk, xk, L);
            if(s->inverse[qtmp] == -1) continue;
            gain += Gadj[j][s->inverse[qtmp]];
          }
          gain_tmp.push_back(make_tuple(gain, j, q, distance(beam.begin(), itr)));
        }
      }
    }

    if(width >= gain_tmp.size()) {
      partial_sort(gain_tmp.begin(), gain_tmp.begin() + width, gain_tmp.end(), comp);
    }else{
      sort(gain_tmp.begin(), gain_tmp.end(), comp);
    }

    vector<pair<int,state*>> beam_tmp;
    REP(l, min(width, (int)gain_tmp.size())) {
      int gain, j, q, idx;
      tie(gain, j, q, idx) = gain_tmp[l];
      state* s = beam[idx].second;
      vector<int> phi_tmp(s->phi);
      vector<int> inverse_tmp(s->inverse);
      set<int> cnd_tmp(s->cnd);
      phi_tmp[j] = q;
      inverse_tmp[q] = j;
      cnd_tmp.erase(q);
      REP(k, 8) {
        int yk = q / L + dy[k];
        int xk = q % L + dx[k];
        if(yk < 0 || yk >= L || xk < 0 || xk >= L) continue;
        int qtmp = Gemb_index(yk, xk, L);
        if(s->inverse[qtmp] != -1) continue;
        cnd_tmp.insert(qtmp);
      }
      state* state_tmp = new state(phi_tmp, inverse_tmp, cnd_tmp);
      beam_tmp.push_back(make_pair(gain, state_tmp));
    }
    beam = beam_tmp;
  }

  REP(i, V) cout << i + 1 << " " << beam[0].second->phi[i] + 1 << endl;

  return 0;
}

