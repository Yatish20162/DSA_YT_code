#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

class SegmentTree {
private:
    int n;
    vector<ll> tree;
    void build(const vector<ll>& flat_vals, int node, int start, int end) {
        if (start == end) {
            tree[node] = flat_vals[start];
            return;
        }
        int mid = (start + end) / 2;
        build(flat_vals, 2 * node, start, mid);
        build(flat_vals, 2 * node + 1, mid + 1, end);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }
    void update(int node, int start, int end, int idx, ll val) {
        if (start == end) {
            tree[node] = val;
            return;
        }
        int mid = (start + end) / 2;
        if (idx <= mid) update(2 * node, start, mid, idx, val);
        else update(2 * node + 1, mid + 1, end, idx, val);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    ll query(int node, int start, int end, int l, int r) {
        if (r < start || end < l) return 0;
        if (l <= start && end <= r) return tree[node];
        int mid = (start + end) / 2;
        return query(2 * node, start, mid, l, r) + query(2 * node + 1, mid + 1, end, l, r);
    }

public:
    SegmentTree(int size, const vector<ll>& flat_vals) {
        n = size;
        tree.assign(4 * n + 1, 0);
        build(flat_vals, 1, 1, n);
    }

    void update(int idx, ll val) {
        update(1, 1, n, idx, val);
    }

    ll query(int l, int r) {
        return query(1, 1, n, l, r);
    }
};

const int MAXN = 200005;
vector<int> adj[MAXN];
ll node_values[MAXN];
int tin[MAXN], sz[MAXN];
int timer = 0;

void dfs(int u, int p) {
    tin[u] = ++timer;
    sz[u] = 1;
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u);
            sz[u] += sz[v]; 
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    cin >> n >> q;

    for (int i = 1; i <= n; i++) cin >> node_values[i];

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1, 0);

    vector<ll> flat_vals(n + 1);
    for (int i = 1; i <= n; i++) {
        flat_vals[tin[i]] = node_values[i];
    }

    SegmentTree st(n, flat_vals);

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int s; ll x;
            cin >> s >> x;
            st.update(tin[s], x);
        } else {
            int s;
            cin >> s;
            int left = tin[s];
            int right = tin[s] + sz[s] - 1;
            cout << st.query(left, right) << "\n";
        }
    }

    return 0;
}