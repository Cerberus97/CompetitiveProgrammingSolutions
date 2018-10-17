/* cerberus97 - Hanit Banga

Solution: Fix a value of d. Now since d <= 1000, there are at most 4 distinct primes. So we can categorize each
number from 1 to m as a bitmask such that a bit is 1 if that number has the corresponding prime with a power
larger than its power in m. So we can have at most 16 'types' of numbers. Note that for the lcm of a bunch of numbers
to be divisibile by d, for every bit there should be some number with that bit 1. In other words, the bitwise OR of 
all masks of numbers in the array should have all bits set to 1. Now define dp(i, j) as number of arrays of length i
such that the bitwise OR is exactly j. We can calculate all 16 values of dp(i + 1, __) from 16 values of dp(i, __).
So we can now use matrix exponentiation to calculate the answer for large i.

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
typedef vector<vector<ll>> matrix;

const int N = 5e6 + 10, S = 20, mod = 1e9 + 7;

int c[S];

void exponentiate(matrix &m, int n);
matrix matrix_mul(matrix a, matrix b);

int main() {
	fast_cin();
	int t; cin >> t;
	while (t--) {
		int n, m, l, r;
		cin >> n >> m >> l >> r;
		ll res = 0;
		for (int d = l; d <= r; ++d) {
			vector<pii> primes;
			int temp = d;
			for (int j = 2; j <= temp; ++j) {
				if (temp % j == 0) {
					int ctr = 0;
					while (temp % j == 0) {
						temp /= j;
						++ctr;
					}
					primes.pb({j, ctr});
				}
			}
			int s = primes.size();
			memset(c, 0, sizeof(c));
			for (int i = 1; i <= m; ++i) {
				int mask = 0, temp = i;
				for (int j = 0; j < s; ++j) {
					int ctr = 0;
					auto p = primes[j];
					while (temp % p.first == 0) {
						temp /= p.first;
						++ctr;
					}
					if (ctr >= p.second) {
						mask |= (1 << j);
					}
				}
				c[mask]++;
				// cout << i << ' ' << mask << endl;
			}
			// cout << c[0] << ' ' << c[1] << endl;
			int subsets = 1 << s;
			matrix mat(subsets);
			for (int i = 0; i < subsets; ++i) {
				mat[i].resize(subsets);
				for (int j = 0; j < subsets; ++j) {
					mat[i][j] = 0;
					for (int k = 0; k < subsets; ++k) {
						if ((j | k) == i) {
							mat[i][j] += c[k];
						}
					}
					// cout << mat[i][j] << ' ';
				}
				// cout << endl;
			}
			matrix a(subsets);
			for (int i = 0; i < subsets; ++i) {
				a[i].resize(1);
				a[i][0] = (i == 0);
			}
			exponentiate(mat, n);
			for (auto &i : mat) {
				for (auto &j : i) {
					// cout << j << ' ';
				}
				// cout << endl;
			}
			matrix ans = matrix_mul(mat, a);
			// cout << ans[subsets - 1][0] << '\n';
			res += ans[subsets - 1][0];
			res %= mod;
		}
		cout << res << '\n';
	}
}

void exponentiate(matrix &m, int n) {
	--n;
	matrix res = m;
	while (n) {
		if (n & 1) {
			res = matrix_mul(res, m);
		}
		n /= 2;
		m = matrix_mul(m, m);
	}
	m = res;
}

matrix matrix_mul(matrix a, matrix b) {
	int n = a.size(), m = b.size(), p = b[0].size();
	matrix ans(n);
	for (int i = 0; i < n; ++i) {
		ans[i].resize(p);
		for (int j = 0; j < p; ++j) {
			ans[i][j] = 0;
			for (int k = 0; k < m; ++k) {
				ans[i][j] += a[i][k] * b[k][j];
				ans[i][j] %= mod;
			}
		}
	}
	return ans;
}