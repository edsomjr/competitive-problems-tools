#include <bits/stdc++.h>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);

    int N;
    double M;

    cin >> N >> M;

    vector<int> ans;

    for (int i = 1; i <= N; ++i)
    {
        double x;
        cin >> x;

        if (x <= M)
        {
            ans.push_back(i);
            M -= x;
        }
    }

    cout << ans.size() << endl;

    for (size_t i = 0; i < ans.size(); ++i)
        cout << ans[i] << (i + 1 == ans.size() ? "\n" : " ");

    return 0;
}
