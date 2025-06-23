#include <cstdio>
#include <memory>
#include <tuple>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <random>

const float GRAVITY = 9.81;

std::vector<std::vector<float>> vecmult(const std::vector<std::vector<float>>& A, const std::vector<std::vector<float>>& B) {
    int rows = A.size();
    int cols = B[0].size();
    int inner = B.size();
    std::vector<std::vector<float>> result(rows, std::vector<float>(cols, 0.0f));

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            for (int k = 0; k < inner; ++k)
                result[i][j] += A[i][k] * B[k][j];

    return result;
}

std::vector<std::vector<float>> vecadd(const std::vector<std::vector<float>>& A, const std::vector<std::vector<float>>& B) {
    int rows = A.size();
    int cols = A[0].size();
    std::vector<std::vector<float>> result(rows, std::vector<float>(cols, 0.0f));

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            result[i][j] = A[i][j] + B[i][j];

    return result;
}

std::vector<std::vector<float>> vecsub(const std::vector<std::vector<float>>& A, const std::vector<std::vector<float>>& B) {
    int rows = A.size();
    int cols = A[0].size();
    std::vector<std::vector<float>> result(rows, std::vector<float>(cols, 0.0f));

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            result[i][j] = A[i][j] - B[i][j];

    return result;
}

std::vector<std::vector<float>> vectranspose(const std::vector<std::vector<float>>& A) {
    int rows = A.size();
    int cols = A[0].size();
    std::vector<std::vector<float>> result(cols, std::vector<float>(rows, 0.0f));

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            result[j][i] = A[i][j];

    return result;
}

std::vector<std::vector<float>> inv22(const std::vector<std::vector<float>>& A) {
    float det = A[0][0]*A[1][1] - A[0][1]*A[1][0];
    if (std::fabs(det) < 1e-6f)
        throw std::runtime_error("Matrix not invertible");

    float invDet = 1.0f / det;

    std::vector<std::vector<float>> result(2, std::vector<float>(2));
    result[0][0] =  A[1][1] * invDet;
    result[0][1] = -A[0][1] * invDet;
    result[1][0] = -A[1][0] * invDet;
    result[1][1] =  A[0][0] * invDet;

    return result;
}
class Particle{
    protected:
        float x;
        float y;
        float v;
        float t;
        float res;
        float angle;
    public:
        Particle(float initX, float initY, float initV, float a, float r)
        :x(initX),y(initY),v(initV),angle(a*M_PI/180.0f),res(r)
        {
            t = 0.0;
        }
        const std::tuple<float,float> getLoc(){
            return std::make_tuple(x,y);
        }
        virtual void update(){
            t = t + res;
            x = v * std::cos(angle) * t;
            y = v * std::sin(angle) * t;
        }
        ~Particle(){};
};

class Projectile : public Particle{
    private:

    public:
        Projectile(float initX, float initY, float initV, float a, float r)
        :Particle(initX,initY,initV,a,r)
        {
        }
        void update(){
            t = t + res;
            x = v * std::cos(angle) * t; 
            y = v * std::sin(angle) * t - (.5f)*GRAVITY*(t*t);
        }
        ~Projectile(){};
};

class Sensor{
    private:
        std::string name;
    public:
        Sensor(const std::string& assign)
        :name(assign)
        {}
        const std::string getName() const{
            return name;
        }
        virtual std::string getType() const{
            return "Generic Sensor";
        }
        ~Sensor(){}
};

class GPS : public Sensor {
    private:
        std::shared_ptr<Particle> target;
        float recx;
        float recy;
        float noise_level;
    public:
        GPS(const std::string& assign, std::shared_ptr<Particle> t, float noise)
        :target(t), Sensor(assign), noise_level(noise){};
        std::string getType() const{
            return "GPS";
        }
        std::tuple<float,float> readLoc(){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::normal_distribution<float> distr(0.0f,noise_level);
            auto [locx,locy] = target->getLoc();
            return std::make_tuple(locx+distr(gen),locy+distr(gen)); 
        }
};

class kalmanFilter {
    private:
        float dt;
        float ax, ay;

        std::shared_ptr<GPS> gps;
        std::vector<std::vector<float>> state;
        std::vector<std::vector<float>> P;
        std::vector<std::vector<float>> F;
        std::vector<std::vector<float>> U;
        std::vector<std::vector<float>> H;
        std::vector<std::vector<float>> Q;
        std::vector<std::vector<float>> R;
        std::vector<std::vector<float>> K;
        std::vector<std::vector<float>> I;

    public:
        kalmanFilter(std::shared_ptr<GPS> g, float res, float Q_mod, float R_mod)
        :gps(g),dt(res)
        {
            ax = 0;
            ay = 0;
            state = std::vector<std::vector<float>>(4,std::vector<float>(1,0.0f));
            F = std::vector<std::vector<float>>(4,std::vector<float>(4,0.0f));
            P = std::vector<std::vector<float>>(4,std::vector<float>(4,0.0f));
            U = std::vector<std::vector<float>> (4,std::vector<float>(1,0.0f));
            Q = std::vector<std::vector<float>>(4,std::vector<float>(4,0.0f));
            K = std::vector<std::vector<float>>(4,std::vector<float>(2,0.0f));
            H = std::vector<std::vector<float>>(2,std::vector<float>(4,0.0f));
            R = std::vector<std::vector<float>>(2,std::vector<float>(2,0.0f));
            I = std::vector<std::vector<float>>(4,std::vector<float>(4,0.0f));

            for (int n = 0; n < 4; n++){
                for (int m = 0; m < 4; m++){
                    F[n][m] = (n == m) ? 1.0f : 0.0f;
                    P[n][m] = (n == m) ? 1.0f : 0.0f;
                    Q[n][m] = (n == m) ? Q_mod : 0.0f;
                    I[n][m] = (n == m) ? 1.0f : 0.0f;
                }
            }
            F[0][2] = dt;
            F[1][3] = dt;

            U[0][0] = (1/2) * ax * (dt * dt);
            U[1][0] = (1/2) * ay * (dt * dt);
            U[2][0] = ax * (dt * dt);
            U[3][0] = ay * (dt * dt);

            for (int n = 0; n < 2; n++){
                for (int m = 0; m < 2; m++){
                    R[n][m] = (n == m) ? R_mod:0.0f;
                    H[n][m] = (n == m) ? 1.0f:0.0f;
                }
            }
            auto initial = gps->readLoc();
            state[0][0] = std::get<0>(initial);
            state[1][0] = std::get<1>(initial);
        }
        void predict(){
           state = vecadd(vecmult(F, state),U);
           P = vecadd(vecmult(vecmult(F,P),vectranspose(F)),Q);
        }

        void update(const std::tuple<float, float> &result){
            std::vector<std::vector<float>> y(2,std::vector<float>(1,0.0f));
            std::vector<std::vector<float>> S(2,std::vector<float>(2,0.0f));

            y[0][0] = std::get<0>(result);
            y[1][0] = std::get<1>(result);
            y = vecsub(y,vecmult(H,state));
            S = vecadd(vecmult(vecmult(H,P),vectranspose(H)),R);
            K = vecmult(vecmult(P,vectranspose(H)),inv22(S));
            state = vecadd(state,vecmult(K,y));
            P = vecmult(vecsub(I,vecmult(K,H)),P);
        }

        std::tuple<float, float> filterSensor(){
            std::tuple<float, float> rawval = gps->readLoc();
            this->predict();
            this->update(rawval);
            return std::make_tuple(state[0][0],state[1][0]);
        }
};

int main(){
    float velocity = 100.0;
    float deg = 45.0;
    float altitude = 0; 
    float n = 0; 
    float resolution = .1;
    float Qmod = .01;
    float Rmod = 5.0;
    float noise_lvl = 10.0;
    int duration = 100;
    int count = 0;
    std::string LorP = "L";
    std::vector<std::vector<float>> output(1,std::vector<float>(6,0.0f));
    std::vector<float> output_line(6,0.0f);
    std::shared_ptr<Particle>P1;
    std::shared_ptr<GPS> GPS1;
    std::unique_ptr<kalmanFilter> KF;

    /*
    std::cout<<"Linear or Projectile (L/P): "<<std::endl;
    std::cin>>LorP;
    std::cout<<"Enter Velocity : "<<std::endl;
    std::cin>>velocity;
    std::cout<<"Enter Angle (deg) : "<<std::endl;
    std::cin>>deg;
    std::cout<<"Enter Resolution : "<<std::endl;
    std::cin>>resolution;
    std::cout<<"Enter Noise : "<<std::endl;
    std::cin>>noise_lvl;
    std::cout<<"Kalman Q : "<<std::endl;
    std::cin>>Qmod;
    std::cout<<"Kalman R : "<<std::endl;
    std::cin>>Rmod;
    */

    if (LorP == "L"){
        P1 = std::make_shared<Particle>(0,0,velocity,deg,resolution);
        std::cout<<"Duration : "<<std::endl;
        std::cin>>duration;
    }else{
        P1 = std::make_shared<Projectile>(0,0,velocity,deg,resolution);
    }
    
    GPS1 = std::make_shared<GPS>("GPS1",P1, noise_lvl);
    KF = std::make_unique<kalmanFilter>(GPS1,resolution, Qmod, Rmod);
    std::cout<<"sensor ("<<GPS1->getType()<<") : "<<GPS1->getName()<<std::endl;

    std::ofstream file("trajectory.csv");

    auto loc = GPS1->readLoc();
    auto true_loc = P1->getLoc();
    auto filtered_loc = KF->filterSensor();
    std::cout<<"t ("<<n<<")\n"<<std::left<<std::setw(10)<<"RAW"<<"x : "<<std::get<0>(loc)<<" y : "<<std::get<1>(loc)<<std::endl;
    std::cout<<std::left<<std::setw(10)<<"FILTERED"<<"x : "<<std::get<0>(filtered_loc)<<" y : "<<std::get<1>(filtered_loc)<<std::endl;
    std::cout<<std::left<<std::setw(10)<<"TRUE"<<"x : "<<std::get<0>(true_loc)<<" y : "<<std::get<1>(true_loc)<<std::endl;

    output_line[0] = std::get<0>(loc);
    output_line[1] = std::get<1>(loc);
    output_line[2] = std::get<0>(filtered_loc);
    output_line[3] = std::get<1>(filtered_loc);
    output_line[4] = std::get<0>(true_loc);
    output_line[5] = std::get<1>(true_loc);
    output.push_back(output_line);

    n = n + resolution;
    P1->update();
    loc = GPS1->readLoc();
    true_loc = P1->getLoc();
    filtered_loc = KF->filterSensor();
    altitude = std::get<1>(loc);
    std::cout<<"t ("<<n<<")\n"<<std::left<<std::setw(10)<<"RAW"<<"x : "<<std::get<0>(loc)<<" y : "<<std::get<1>(loc)<<std::endl;
    std::cout<<std::left<<std::setw(10)<<"FILTERED"<<"x : "<<std::get<0>(filtered_loc)<<" y : "<<std::get<1>(filtered_loc)<<std::endl;
    std::cout<<std::left<<std::setw(10)<<"TRUE"<<"x : "<<std::get<0>(true_loc)<<" y : "<<std::get<1>(true_loc)<<std::endl;

    output_line[0] = std::get<0>(loc);
    output_line[1] = std::get<1>(loc);
    output_line[2] = std::get<0>(filtered_loc);
    output_line[3] = std::get<1>(filtered_loc);
    output_line[4] = std::get<0>(true_loc);
    output_line[5] = std::get<1>(true_loc);
    output.push_back(output_line);

    while(((altitude >= 0 || n < 1.0) && LorP == "P") || (n < duration && LorP == "L") ){
        n = n + resolution;
        P1->update();
        loc = GPS1->readLoc();
        true_loc = P1->getLoc();
        filtered_loc = KF->filterSensor();
        altitude = std::get<1>(true_loc);
        std::cout<<"t ("<<n<<")\n"<<std::left<<std::setw(10)<<"RAW"<<"x : "<<std::get<0>(loc)<<" y : "<<std::get<1>(loc)<<std::endl;
        std::cout<<std::left<<std::setw(10)<<"FILTERED"<<"x : "<<std::get<0>(filtered_loc)<<" y : "<<std::get<1>(filtered_loc)<<std::endl;
        std::cout<<std::left<<std::setw(10)<<"TRUE"<<"x : "<<std::get<0>(true_loc)<<" y : "<<std::get<1>(true_loc)<<std::endl;

        output_line[0] = std::get<0>(loc);
        output_line[1] = std::get<1>(loc);
        output_line[2] = std::get<0>(filtered_loc);
        output_line[3] = std::get<1>(filtered_loc);
        output_line[4] = std::get<0>(true_loc);
        output_line[5] = std::get<1>(true_loc);
        output.push_back(output_line);
    }

    for (auto &outl : output){
        count = 0;
        for (auto &out : outl){
            std::cout<<out<<" ";
            file<<out;
            if (count != 5)
                file<<",";
            count++;
        }
        std::cout<<std::endl;
        file<<"\n";
    }
    file.close();

    return 0;   
}