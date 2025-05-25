#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

int ksR(int W, std::vector<int> &val, std::vector<int> &wt, int n){
    if (n == 0 || W == 0)
        return 0;
    int pick = 0;

    if (wt[n-1] <= W){
        pick = val[n-1] + ksR(W - wt[n-1], val, wt, n-1);
    }
    int notPick = ksR(W, val, wt, n-1);
    return std::max(pick, notPick);
}

int ks(int W, std::vector<int> &val, std::vector<int> &wt){
    int n = val.size();
    return ksR(W, val, wt, n);
}

int main(){
    int W = 10;
    std::vector<int> val(10);
    std::vector<int> wt(10);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1,10);
    for (int m = 0; m < val.size(); m++){
        val[m] = distr(gen);
        wt[m] = distr(gen);
    }
    for (auto& v:val)
        std::cout<<v<<' '<<std::flush;
    std::cout<<std::endl;
    for (auto& w:wt)
        std::cout<<w<<' '<<std::flush;
    std::cout<<std::endl;
    std::cout<<ksR(W, val, wt, val.size())<<std::endl;
    return 0;
}