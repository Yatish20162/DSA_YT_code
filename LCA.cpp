struct LCA {
    vector<int> height, euler, first, segtree;
    vector<bool> visited;
    int n;

    LCA(vector<vector<int>> &adj, int root = 0) {
        n = adj.size();
        height.resize(n);
        first.resize(n);
        euler.reserve(n * 2);
        visited.assign(n, false);
        dfs(adj, root);
        int m = euler.size();
        segtree.resize(m * 4);
        build(1, 0, m - 1);
    }

    void dfs(vector<vector<int>> &adj, int node, int h = 0) {
        visited[node] = true;
        height[node] = h;
        first[node] = euler.size();
        euler.push_back(node);
        for (auto to : adj[node]) {
            if (!visited[to]) {
                dfs(adj, to, h + 1);
                euler.push_back(node);
            }
        }
    }

    void build(int node, int b, int e) {
        if (b == e) {
            segtree[node] = euler[b];
        } else {
            int mid = (b + e) / 2;
            build(node << 1, b, mid);
            build(node << 1 | 1, mid + 1, e);
            int l = segtree[node << 1], r = segtree[node << 1 | 1];
            segtree[node] = (height[l] < height[r]) ? l : r;
        }
    }

    int query(int node, int b, int e, int L, int R) {
        if (b > R || e < L)
            return -1;
        if (b >= L && e <= R)
            return segtree[node];
        int mid = (b + e) >> 1;

        int left = query(node << 1, b, mid, L, R);
        int right = query(node << 1 | 1, mid + 1, e, L, R);
        if (left == -1) return right;
        if (right == -1) return left;
        return height[left] < height[right] ? left : right;
    }

    int lca(int u, int v) {
        int left = first[u], right = first[v];
        if (left > right)
            swap(left, right);
        return query(1, 0, euler.size() - 1, left, right);
    }
};



Kth Ancestor

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int MAXN = 200005;
const int LOG = 20; // 2^20 is > 10^6, enough for N=2*10^5

int up[MAXN][LOG];
vector<int> adj[MAXN];

// DFS to set up[u][0] (the immediate parent)
void dfs(int u, int p) {
    up[u][0] = p;
    for (int i = 1; i < LOG; i++) {
        if (up[u][i - 1] != -1)
            up[u][i] = up[up[u][i - 1]][i - 1];
        else
            up[u][i] = -1;
    }

    for (int v : adj[u]) {
        if (v != p) dfs(v, u);
    }
}

int get_kth_ancestor(int u, int k) {
    for (int i = 0; i < LOG; i++) {
        // If the i-th bit of k is set
        if ((k >> i) & 1) {
            u = up[u][i];
            if (u == -1) break; // If we jump above the root
        }
    }
    return u;
}

int main() {
    int n, q;
    cin >> n >> q;

    // For simplicity, assuming a parent array is given or build adj list
    // Node 1 is root, up[1][0] = -1
    for (int i = 2; i <= n; i++) {
        int p; cin >> p;
        adj[p].push_back(i);
        adj[i].push_back(p);
    }

    dfs(1, -1);

    while (q--) {
        int u, k;
        cin >> u >> k;
        cout << get_kth_ancestor(u, k) << endl;
    }

    return 0;
}