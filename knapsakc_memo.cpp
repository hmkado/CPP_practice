#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <cstdlib>
#include <memory>

class sack{
    private:
        std::vector<std::vector<int>> memo;
        std::vector<int> selectedItems;
        int W;
        int total;
    public:
        sack(const int weight)
        :W(weight)
        {
            total = 0;
        }
        
        ~sack(){}
        void initializeMemo(int row_sz, int col_sz){
            memo.resize(row_sz, std::vector<int>(col_sz, -1));
        }

        void loadSack(std::vector<int> &val, std::vector<int> &wt, int n){
            this->initializeMemo(n+1, W+1);
            total = ksR(W, val, wt, n, memo);
            this->trackItems(val, wt);
        }

        const int returnTotal() const{
            return total;
        }

        const std::vector<int>& getItems() const {
            return selectedItems;
        }
        
        int ksR(int W, std::vector<int> &val, std::vector<int> &wt, int n, std::vector<std::vector<int>> &memo){
            if (n == 0 || W == 0)
                return 0;
            if (memo[n][W] != -1)
                return memo[n][W];
            int pick = 0;

            if (wt[n-1] <= W){
                pick = val[n-1] + this->ksR(W - wt[n-1], val, wt, n-1, memo);
            }
            int notPick = this->ksR(W, val, wt, n-1, memo);
            return memo[n][W] = std::max(pick, notPick); 
        }

        void trackItems(std::vector<int> &val, std::vector<int> &wt){
            int n = val.size();
            int w = W;
            while(n > 0 && w > 0){
                if (memo[n][w] == memo[n-1][w]){
                    n = n-1;
                }else{
                    selectedItems.push_back(n-1);
                    w = w-wt[n-1];
                    n = n-1;
                }
            }
            std::reverse(selectedItems.begin(),selectedItems.end());
        }
};

int main(){
    int W = 10;
    std::vector<int> val(10);
    std::vector<int> wt(10);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1,10);

    std::unique_ptr<sack> backpack= std::make_unique<sack>(10);

    for (int m = 0; m < val.size(); m++){
        val[m] = distr(gen);
        wt[m] = distr(gen);
    }
    std::cout<<"Item values : "<<std::flush;
    for (auto& v:val)
        std::cout<<v<<' '<<std::flush;
    std::cout<<"Item weights : "<<std::endl;
    for (auto& w:wt)
        std::cout<<w<<' '<<std::flush;
    std::cout<<"\nSelected items : "<<std::endl;
    backpack->loadSack(val, wt, val.size());
    for (auto& si:backpack->getItems())
        std::cout<<si<<' '<<std::flush;
    std::cout<<"\nTotal value : "<<std::endl;
    std::cout<<backpack->returnTotal()<<std::endl;
    return 0;
}