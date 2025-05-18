#include <iostream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstdlib>
#include <tuple>
#include <queue>
#include <sstream>

struct Node{
    int weight;
    bool visited;
    std::tuple<int, int> parent;
    Node(int w, int v, std::tuple<int,int> p)
    :weight(w),visited(v),parent(p)
    {}
};

std::tuple<int, int> addTuples(const std::tuple<int, int>& tuple_a, const std::tuple<int, int>& tuple_b){
    return {std::get<0>(tuple_a)+std::get<0>(tuple_b),std::get<1>(tuple_a)+std::get<1>(tuple_b)};
}

class Robot{
    private:
        std::string robot_name;
        std::tuple<int, int> starting_point;
        std::tuple<int, int> current_location;
        std::tuple<int, int> goal_point;
        std::vector<std::tuple<int,int>> path;
        std::vector<std::vector<Node>> internalMap;
        std::queue<std::tuple<int, int>> bfs_queue;
        int dx[4] = {1, 0, -1, 0};
        int dy[4] = {0, 1, 0, -1};
    public:
    Robot(const std::string& name)
    : robot_name(name), current_location(0,0){
        std::cout<<"Robot Initialized : "<<robot_name<<"\n";
    }
    void setStartingPoint(int x, int y){
        if (x < 0 || y < 0 || x >= internalMap.size() || y >= internalMap[0].size()){
            std::cerr<<"Starting point out of bounds"<<std::endl;
            return;
        } else {
            std::get<0>(current_location) = std::get<0>(starting_point) = x;
            std::get<1>(current_location) = std::get<1>(starting_point) = y;
        }
    }
    void setGoalPoint(int x, int y){
        if (x < 0 || y < 0 || x >= internalMap.size() || y >= internalMap[0].size()){
            std::cerr<<"Goal point out of bounds"<<std::endl;
            return;
        } else {
            std::get<0>(goal_point) = x;
            std::get<1>(goal_point) = y;
        }
    }
    void move(const std::string& input){
        if (input == "up"){
            std::get<1>(current_location) += dy[1];
        } else if (input == "down") {
            std::get<1>(current_location) += dy[3];
        } else if (input == "right") {
            std::get<0>(current_location) += dx[0];
        } else if (input == "left") {
            std::get<0>(current_location) += dx[2];
        } else {
            std::cout<<"Invalid Input.\n";
        }
    }
    const std::tuple<int, int>& getLocation() const {
        return current_location; 
    }
    void loadMap(const std::vector<std::vector<Node>>& iMap){
        internalMap = iMap;
    }
    void searchPath(){
        std::tuple<int,int> popped;
        std::tuple<int,int> moved_location;
        std::vector<std::tuple<int, int>> moves = {{0,1},{0,-1},{1,0},{-1,0}};
        for (auto& row : internalMap){
            for (auto& element : row){
                element.visited = false;
                element.parent = {-1,-1};
            }
        }
        internalMap[std::get<0>(current_location)][std::get<1>(current_location)].visited = true;
        bfs_queue.push(current_location);
        while(!bfs_queue.empty()){
            popped = bfs_queue.front();
            bfs_queue.pop();
            if (popped == goal_point){
                return;
            }
            for (auto& move : moves){
                moved_location = addTuples(popped,move);
                if (std::get<0>(moved_location) >= 0 && std::get<1>(moved_location) >= 0
                && std::get<0>(moved_location) < internalMap.size() && std::get<1>(moved_location) < internalMap[0].size()
                && internalMap[std::get<0>(moved_location)][std::get<1>(moved_location)].visited == false){
                    internalMap[std::get<0>(moved_location)][std::get<1>(moved_location)].parent = popped;
                    internalMap[std::get<0>(moved_location)][std::get<1>(moved_location)].visited = true;
                    bfs_queue.push(moved_location);
                }
            }
        }
    }
    void constructPath(){
        path.clear();
        std::tuple<int, int> current = goal_point;
        while(current != starting_point){
            path.push_back(current);
            current = internalMap[std::get<0>(current)][std::get<1>(current)].parent;
        }
        path.push_back(current);
        std::reverse(path.begin(),path.end());
    }
    void printPath(){
        std::cout<<"Path : \n";
        for(auto& step : path){
            std::cout<<std::get<0>(step)<<", "<<std::get<1>(step)<<"\n";
        }
    }
    void printPathMap(){
        for (int i=internalMap[0].size()-1;i>=0;i--){
            for (int j=0;j<internalMap.size();j++){
                if (std::find(path.begin(),path.end(),std::make_tuple(j,i)) != path.end()){
                    std::cout<<std::setw(3)<<'*'<<std::flush;
                }else{
                    std::cout<<std::setw(3)<<'.'<<std::flush;
                }
            }
            std::cout<<std::endl;
        }
    }
};

class gridMap{
    private:
        int rows;
        int cols;
        std::vector<std::vector<Node>> map2d;
    public:
        gridMap(int n, int m)
        : rows(n), cols(m), map2d(n, std::vector<Node>(m,Node(0,false,{-1,-1})))
        {
            /*
            map2d.resize(n, std::vector<Node>(m));
            for (int i = 0;i<n;i++){
                for(int j = 0;j<m;j++){
                    map2d[i][j] = Node(0,0,{-1,-1});
                }
            }
            */
            std::cout<<"Map initialized : ("<<n<<", "<<m<<")\n";
        }
        void printMap(){
            for (int i=cols-1;i>=0;i--){
                for (int j=0;j<rows;j++){
                    std::cout<<std::setw(3)<<map2d[j][i].weight;
                }
                std::cout<<"\n";
            }
        }
        std::vector<std::vector<Node>> provideMap(){
            return map2d;
        }
};

int main(){
    int rows;
    int cols;
    int s_x, s_y, g_x, g_y;
    std::string line;
    std::istringstream iss;

    rows = 10;
    cols = 15;
    s_x = 1; s_y = 2;
    g_x = 9; g_y = 9;

    std::unique_ptr<gridMap> gMap = std::make_unique<gridMap>(rows, cols);
    gMap->printMap();
    std::unique_ptr<Robot> R1 = std::make_unique<Robot>("R1");
    R1->loadMap(gMap->provideMap());
    R1->setStartingPoint(s_x,s_y);
    R1->setGoalPoint(g_x,g_y);
    R1->searchPath();
    R1->constructPath();
    R1->printPathMap();
    R1->printPath();
    return 0;
}
