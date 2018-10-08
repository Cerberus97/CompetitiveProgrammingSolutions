/* cerberus97 - Hanit Banga

Idea: Use persistent DSU to keep track of connectivity when adding edges one by one.
For a query, binary search on the max index of edge allowed, then use DSU at that point to check if >= z vertices reachable.
*/

#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

#define pb push_back
#define fast_cin() ios_base::sync_with_stdio(false); cin.tie(NULL)

typedef long long ll;
typedef long double ld;
typedef pair <int, int> pii;
typedef pair <ll, ll> pll;

const int N = 1e5 + 10;

void update(int u, int v, int i);
int query(int v, int u, int i);
int get_root(int u, int i);
int get_size(int u, int i);

vector<pii> sz[N], par[N];

int main() {
	fast_cin();
	int n, m;
	cin >> n >> m;
	for (int i = 1; i <= n; ++i) {
		sz[i].pb({0, 1});
		par[i].pb({0, i});
	}
	for (int i = 1; i <= m; ++i) {
		int u, v;
		cin >> u >> v;
		update(u, v, i);
	}
	int q; cin >> q;
	while (q--) {
		int x, y, z;
		cin >> x >> y >> z;
		int lo = 1, hi = m - 1;
		while (lo <= hi) {
			int mid = (lo + hi) / 2;
			if (query(x, y, mid) >= z) {
				hi = mid - 1;
			} else {
				lo = mid + 1;
			}
		}
		cout << hi + 1 << '\n';
	}
}

void update(int u, int v, int i) {
	u = get_root(u, i - 1);
	v = get_root(v, i - 1);
	if (u != v) {
		int s_u = get_size(u, i - 1);
		int s_v = get_size(v, i - 1);
		if (s_u >= s_v) {
			sz[u].pb({i, s_u + s_v});
			par[v].pb({i, u});
		} else {
			sz[v].pb({i, s_u + s_v});
			par[u].pb({i, v});
		}
	}
}

int query(int v, int u, int i) {
	u = get_root(u, i);
	v = get_root(v, i);
	if (u == v) {
		return get_size(u, i);
	} else {
		return get_size(u, i) + get_size(v, i);
	}
}

int get_root(int u, int i) {
	int n = par[u].size();
	int lo = 1, hi = n - 1;
	while (lo <= hi) {
		int mid = (lo + hi) / 2;
		if (par[u][mid].first <= i) {
			lo = mid + 1;
		} else {
			hi = mid - 1;
		}
	}
	if (par[u][lo - 1].second == u) {
		return u;
	} else {
		int v = get_root(par[u][lo - 1].second, i);
		// par[u][lo - 1].second = v;
		return v;
	}
}

int get_size(int u, int i) {
	int n = sz[u].size();
	int lo = 1, hi = n - 1;
	while (lo <= hi) {
		int mid = (lo + hi) / 2;
		if (sz[u][mid].first <= i) {
			lo = mid + 1;
		} else {
			hi = mid - 1;
		}
	}
	return sz[u][lo - 1].second;
}