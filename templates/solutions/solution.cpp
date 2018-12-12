#include <bits/stdc++.h>

using namespace std;
using di = pair<double, int>;

int main()
{
    ios::sync_with_stdio(false);

    int N;
    double M;

    cin >> N >> M;

    vector<di> xs(N);

    for (int i = 0; i < N; ++i)
    {
        cin >> xs[i].first;
        xs[i].second = i + 1;
    }

    sort(xs.begin(), xs.end());

    vector<int> ans;

    for (const auto& x : xs)
    {
        double v;
        int k;

        tie(v, k) = x;

        if (v <= M)
        {
            ans.push_back(k);
            M -= v;
        }
    }

    cout << ans.size() << endl;

    for (size_t i = 0; i < ans.size(); ++i)
        cout << ans[i] << (i + 1 == ans.size() ? "\n" : " ");

    return 0;
}
