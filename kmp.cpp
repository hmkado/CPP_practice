#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

class stringDictionary{
    private:
        std::string name;
    public:
        stringDictionary(const std::string& sd_name)
        :name(sd_name)
        {

        }
        ~stringDictionary(){}
        std::string getName(){
            return name;
        }
        void init_lps(const std::string& pat, std::vector<int>& lps){
            int len = 0;
            int i = 1;
            lps[0] = 0;
            while (i < pat.length()){
                if(pat[i] == pat[len]){
                    len++;
                    lps[i] = len;
                    i++;
                }else{
                    if (len == 0){
                        lps[i] = 0;
                        i++;
                    }else{
                        len = lps[len-1];
                    }
                }
            }
        }
        std::vector<int> findPat(const std::string& txt, const std::string& pat){
            int n = txt.length();
            int m = pat.length();
            int i = 0;
            int j = 0;
            std::vector<int> lps(m);
            std::vector<int> res;
            this->init_lps(pat, lps);
            while (i < n){
                if (txt[i] == pat[j]){
                    i++;
                    j++;
                    if (j == m){
                        res.push_back(i-j);
                        j = lps[j-1];
                    }
                }else{
                    if (j != 0){
                        j = lps[j-1];
                    }else{
                        i++;
                    }
                }
            }
            return res;    
        }
};

int main(){
    std::string user_input;
    std::string pattern;
    std::vector<int> result;

    std::cout<<"Enter string: "<<std::endl;
    std::getline(std::cin,user_input);
    stringDictionary dict1(user_input);
    std::cout<<"Enter pattern: "<<std::endl;
    std::getline(std::cin, pattern);
    result = dict1.findPat(user_input, pattern);
    for(auto& r:result){
        std::cout<<r<<" ";
    }
    std::cout<<std::endl;
    return 0;
}