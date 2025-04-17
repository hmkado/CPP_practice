#include <iostream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <vector>
#include <cstdlib>
#include <tuple>
#include <queue>
#include <sstream>
#include <climits>

struct Node{
    int weight;
    std::tuple<int, int> position;
    Node(int w,std::tuple<int,int> p)
    :weight(w),position(p)
    {}
    bool operator<(const Node& other) const{
        return weight > other.weight;
    }
};

std::tuple<int, int> addTuples(const std::tuple<int, int>& tuple_a, const std::tuple<int, int>& tuple_b){
    return {std::get<0>(tuple_a)+std::get<0>(tuple_b),std::get<1>(tuple_a)+std::get<1>(tuple_b)};
}

class gridMap{
    private:
        int rows;
        int cols;
        int robotCount;
        std::vector<std::vector<Node>> map2d;
        std::vector<std::tuple<int, int>> RobotLocations;
    public:
        gridMap(int n, int m)
        : rows(n), cols(m), robotCount(0), map2d(n, std::vector<Node>(m,Node(0,{-1,-1})))
        {
            for (int i = 0;i<rows;i++){
                for (int j=0;j<cols;j++){
                    map2d[i][j].position = std::make_tuple(i,j);
                }
            }
            /*
            map2d.resize(n, std::vector<Node>(m));
            for (int i = 0;i<n;i++){
                for(int j = 0;j<m;j++){
                    map2d[i][j] = Node(0,0,{-1,-1});
                }
            }
            */
            RobotLocations.clear(); 
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
        void addRandCosts(){
            for (int i = 0;i<rows;i++){
                for (int j=0;j<cols;j++){
                    map2d[i][j].weight = static_cast<int>(std::rand()%10);
                }
            }
        }
        const std::vector<std::vector<Node>>& provideMap() const {
            return map2d;
        }
        int getRow(){
            return rows;
        }
        int getCol(){
            return cols;
        }
        Node getNode(const std::tuple<int, int>& location){
            return map2d[std::get<0>(location)][std::get<1>(location)];
        }
        void registerRobot(const std::tuple<int, int>& rloc){
            robotCount += 1;
            RobotLocations.push_back(rloc);
        }
};

class Robot{
    protected:
        std::vector<std::tuple<int, int>> moves = {{0,1},{0,-1},{1,0},{-1,0}};
    private:
        std::string robot_name;
        std::shared_ptr<gridMap> sharedMap;
        std::vector<std::vector<bool>> visited;
        std::vector<std::vector<std::tuple<int, int>>> parent;
        std::tuple<int, int> starting_point;
        std::tuple<int, int> current_location;
        std::tuple<int, int> goal_point;
        std::vector<std::tuple<int,int>> path;
    public:
    Robot(const std::string& name, const std::shared_ptr<gridMap> inMap)
    : robot_name(name), current_location(0,0), sharedMap(inMap){
        sharedMap->registerRobot(current_location);
        std::cout<<"Robot Initialized : "<<robot_name<<"\n";
    }
    void setStartingPoint(int x, int y){
        if (x < 0 || y < 0 || x >= sharedMap->getRow() || y >= sharedMap->getCol()){
            std::cerr<<"Starting point out of bounds"<<std::endl;
            return;
        } else {
            std::get<0>(current_location) = std::get<0>(starting_point) = x;
            std::get<1>(current_location) = std::get<1>(starting_point) = y;
        }
    }
    void setGoalPoint(int x, int y){
        if (x < 0 || y < 0 || x >= sharedMap->getRow() || y >= sharedMap->getCol()){
            std::cerr<<"Goal point out of bounds"<<std::endl;
            return;
        } else {
            std::get<0>(goal_point) = x;
            std::get<1>(goal_point) = y;
        }
    }
    void move(const std::string& input){
        if (input == "up"){
            current_location = addTuples(current_location,moves[1]);
        } else if (input == "down") {
            current_location = addTuples(current_location,moves[3]);
        } else if (input == "right") {
            current_location = addTuples(current_location,moves[0]);
        } else if (input == "left") {
            current_location = addTuples(current_location,moves[2]);
        } else {
            std::cout<<"Invalid Input.\n";
        }
    }
    const std::tuple<int, int>& getLocation() const {
        return current_location; 
    }
    void initializeSearch(){
        visited.assign(sharedMap->getRow(), std::vector<bool>(sharedMap->getCol(), false));
        parent.assign(sharedMap->getRow(), std::vector<std::tuple<int, int>>(sharedMap->getCol(), {-1, -1}));
    }
    void searchPath(){
        int new_cost = 0;
        std::tuple<int,int> popped;
        std::tuple<int,int> moved_location;
        std::priority_queue<Node> bfs_queue;
        std::vector<std::vector<int>> g_cost(sharedMap->getRow(),std::vector<int>(sharedMap->getCol(),INT_MAX));
        this->initializeSearch();
        g_cost[std::get<0>(current_location)][std::get<1>(current_location)] = 0;
        bfs_queue.push(Node(g_cost[std::get<0>(current_location)][std::get<1>(current_location)],current_location));
        while(!bfs_queue.empty()){
            popped = bfs_queue.top().position;
            bfs_queue.pop();
            if (popped == goal_point){
                break;;
            }
            for (auto& move : moves){
                moved_location = addTuples(popped,move);
                if (std::get<0>(moved_location) >= 0 && std::get<1>(moved_location) >= 0
                && std::get<0>(moved_location) < sharedMap->getRow() && std::get<1>(moved_location) < sharedMap->getCol()){
                    new_cost = g_cost[std::get<0>(popped)][std::get<1>(popped)] + sharedMap->getNode(moved_location).weight;
                    if (new_cost < g_cost[std::get<0>(moved_location)][std::get<1>(moved_location)]){
                        g_cost[std::get<0>(moved_location)][std::get<1>(moved_location)] = new_cost; 
                        parent[std::get<0>(moved_location)][std::get<1>(moved_location)] = popped;
                        bfs_queue.push(Node(new_cost,moved_location));
                    }
                }
            }
        }
    }
    void constructPath(){
        path.clear();
        std::tuple<int, int> current = goal_point;
        while(current != starting_point){
            path.push_back(current);
            current = parent[std::get<0>(current)][std::get<1>(current)];
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
        for (int i=sharedMap->getCol()-1;i>=0;i--){
            for (int j=0;j<sharedMap->getRow();j++){
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

int main(){
    int rows;
    int cols;
    int s_x, s_y, g_x, g_y;
    std::string line;
    std::istringstream iss;

    /*
    iss.clear();
    std::cout<<"Type row and col size : ";
    std::getline(std::cin, line);
    iss.str(line);

    if ((iss>>rows) && (iss.peek() == ' ') && (iss>>cols)){
        std::cout<<"Rows and cols entered"<<std::endl;
    } else {
        std::cerr<<"Invalid input"<<std::endl;
        return 0;
    }

    line.clear();
    iss.clear();
    std::cout<<"Type robot starting point : ";
    std::getline(std::cin, line);
    iss.str(line);

    if ((iss>>s_x) && (iss.peek() == ' ') && (iss>>s_y)){
        if (s_x < 0 || s_x >= rows || s_y < 0 || s_y >= cols){
            std::cerr<<"Input out of bounds"<<std::endl;
            return 0;
        }
        std::cout<<"Robot starting point entered"<<std::endl;
    } else {
        std::cerr<<"Invalid input"<<std::endl;
        return 0;
    }

    line.clear();
    iss.clear();
    std::cout<<"Type robot goal point : ";
    std::getline(std::cin, line);
    iss.str(line);
    
    if ((iss>>g_x) && (iss.peek() == ' ') && (iss>>g_y)){
        if (g_x < 0 || g_x >= rows || g_y < 0 || g_y >= cols){
            std::cerr<<"Input out of bounds"<<std::endl;
            return 0;
        }
        std::cout<<"Robot goal point entered"<<std::endl;
    } else {
        std::cerr<<"Invalid input"<<std::endl;
        return 0;
    }

    line.clear();
    iss.clear();
    */
    srand(time(0));
    rows = 10;
    cols = 15;
    s_x = static_cast<int>(std::rand()%rows); s_y = static_cast<int>(std::rand()%cols);
    g_x = static_cast<int>(std::rand()%rows); g_y = static_cast<int>(std::rand()%cols);

    std::shared_ptr<gridMap> gMap = std::make_shared<gridMap>(rows, cols);
    gMap->addRandCosts();
    gMap->printMap();
    std::unique_ptr<Robot> R1 = std::make_unique<Robot>("R1",gMap);
    R1->setStartingPoint(s_x,s_y);
    R1->setGoalPoint(g_x,g_y);
    R1->searchPath();
    R1->constructPath();
    R1->printPathMap();
    return 0;
}