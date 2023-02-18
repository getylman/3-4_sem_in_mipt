#include <iostream>
#include <vector>
using namespace std;

int main() {
  int n;
    cin >> n;
    vector<int64_t> a(n);
    vector<int64_t> res;
    for(int i = 0; i < n; ++i)
        cin >> a[i];
    int i = 0;
    while(i < n){
        if(a[i] < 0){
            int64_t mn = -1'000'000'000'000;
            while(a[i] < 0 && i < n)
                mn = max(a[i], mn), ++i;
            res.push_back(mn);
        }
        else{
            int64_t mn = 0;
            while(a[i] > 0 && i < n)
                mn = max(a[i], mn), ++i;
            res.push_back(mn);
        }
    }
    int64_t sum = 0;
    for(int i = 0; i < res.size(); ++i)
        sum += res[i];
    cout << sum;
}