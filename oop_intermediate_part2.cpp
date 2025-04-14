#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <memory>

class Sensor{
    public:
        virtual ~Sensor(){}

        virtual float readData() = 0;

        virtual std::string getType() const{
            return "Generic Sensor";
        }
};

class TemperatureSensor : public Sensor{
    public:
        float readData() override{
            float temp = static_cast<float>((std::rand()%4000))/100.0f;
            return temp;
        }
        std::string getType() const override{
            return "Temperature Sensor";
        }
};

class DistanceSensor : public Sensor{
    public:
        float readData() override{
            float dist = static_cast<float>((std::rand()%1000))/100.0f;
            return dist;
        }
        std::string getType() const override{
            return "Distance Sensor";
        }
};

class Robot{
    private: 
        std::string name;
        std::vector<std::unique_ptr<Sensor>> sensors;
    
    public:
        Robot(const std::string& robot_name)
        :name(robot_name){
            std::cout<<"Robot "<<name<<" Initialized\n";
        }
        
        ~Robot(){
            std::cout<<"Robot "<<name<<" Deconstructed\n";
        }

        void addSensor(std::unique_ptr<Sensor> sensor){
            sensors.push_back(std::move(sensor));
        }

        void readSensors() const{
            std::cout<<"Reading "<<name<<" Sensors...\n";
            for (const auto& s : sensors){
                std::cout<<"- "<<s->getType()<<" : "<<s->readData();
            }
        }

        const std::vector<std::unique_ptr<Sensor>>& getSensor() const {
            return sensors; 
        }

        std::string getName(){
            return name;
        }
};

int main(){
    std::srand(std::time(NULL));

    std::vector<std::unique_ptr<Robot>> robots;
    robots.push_back(std::make_unique<Robot>("R1"));
    robots.push_back(std::make_unique<Robot>("R2"));
    robots.push_back(std::make_unique<Robot>("R3"));
    
    robots[0]->addSensor(std::make_unique<TemperatureSensor>());
    robots[1]->addSensor(std::make_unique<DistanceSensor>());
    robots[1]->addSensor(std::make_unique<TemperatureSensor>());
    robots[2]->addSensor(std::make_unique<DistanceSensor>());
    robots[2]->addSensor(std::make_unique<TemperatureSensor>());
    
    for (const auto& robot : robots){
        std::cout<<"Robot : "<<robot->getName()<<"\n";

        bool hasTemp = false;
        bool hasDist = false;
        for (const auto& sensor : robot->getSensor()){
            auto* tempSensor = dynamic_cast<TemperatureSensor*>(sensor.get());
            auto* distSensor = dynamic_cast<DistanceSensor*>(sensor.get());
            if (tempSensor) {
                std::cout<<"Temperature : "<<sensor->readData()<<" C\n";
                hasTemp = true;
            }
            if (distSensor) {
                std::cout<<"Distance : "<<sensor->readData()<<" m\n";
                hasDist = true;
            }   
        }

        if(!hasTemp){
            std::cout<<"No Temperature Sensor\n";
        }
        if(!hasDist){
            std::cout<<"No Distance Sensor\n";
        }
    } 

    return 0;
}