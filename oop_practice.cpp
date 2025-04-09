#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>

class Robot{
    private:
        std::string name;
        float x,y,z;
    public:
        Robot(const std::string& robot_name)
        : name(robot_name), x(.0),y(.0),z(.0) {
            std::cout<<"Robot "<< name << " initialized.\n";
        }

        ~Robot(){
            std::cout<<"Robot "<< name << " destructed. \n";
        }

        void setName(const std::string& name){
            this->name = name;
        }

        void move(float dx, float dy, float dz){
            x += dx;
            y += dy;
            z += dz;
        }
        void printPosition() const {
            std::cout<<"Robot "<<name<<" location : "<<x<<", "<<y<<", "<<z<<"\n";
        }
        std::string getName() const {
            return name;
        }
};

int main(){
    std::srand(std::time(NULL));
    std::vector<Robot> robotSwarm;
    robotSwarm.emplace_back("R1");
    robotSwarm.emplace_back("R2");
    robotSwarm.emplace_back("R3");

    for(auto& robot:robotSwarm){
        float dx = static_cast<float>(std::rand()%10);
        float dy = static_cast<float>(std::rand()%10);
        float dz = static_cast<float>(std::rand()%10);
        robot.move(dx,dy,dz);
        robot.printPosition();
    }
    return 0;
}