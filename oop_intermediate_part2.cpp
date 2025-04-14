#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <memory>
#include <algorithm>

class Sensor{
    protected:
        std::string id;
    public:
        Sensor(const std::string& sensor_id)
        :id(sensor_id)
        {
            
        }
        virtual ~Sensor(){}

        virtual float readData() = 0;

        virtual std::string getType() const = 0;
        std::string getID() const{
            return id;
        }
};

class TemperatureSensor : public Sensor{
    public:
        TemperatureSensor(const std::string& id) : Sensor(id){}
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
        DistanceSensor(const std::string& id) : Sensor(id){}
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

        void addSensor2(const std::string& sensor_name, const std::string& sensor_type){
            if (sensor_type == "t"){
                sensors.push_back(std::make_unique<TemperatureSensor>(sensor_name));
            }else{
                sensors.push_back(std::make_unique<DistanceSensor>(sensor_name));
            }
        }

        void removeSensorByID(const std::string& id){
            sensors.erase(std::remove_if(sensors.begin(),sensors.end(),[&](const std::unique_ptr<Sensor>& s){
                return s->getID() == id;
            }),sensors.end());
        }

        void printSensors() const{
            std::cout<<"Reading "<<name<<" Sensors...\n";
            for (const auto& s : sensors){
                std::cout<<"- ID : "<<s->getID()<<" "<<s->getType()<<" : "<<s->readData()<<"\n";
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
    Robot robot1("R1");
    
    robot1.addSensor2("T1","t");
    robot1.addSensor2("D1","d");
    
    robot1.printSensors();

    robot1.removeSensorByID("T1");

    robot1.printSensors();

    return 0;
}