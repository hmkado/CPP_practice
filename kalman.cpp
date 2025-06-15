#include <cstdio>
#include <memory>
#include <tuple>
#include <iostream>
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

class Projectile{
    private:
        float x;
        float y;
        float v;
        float angle;
        float t;
        float res;
    public:
        Projectile(float initX, float initY, float initV, float a, float r)
        :x(initX),y(initY),v(initV),angle(a*M_PI/180.0f),res(r)
        {
            t = 0.0;
        }
        std::tuple<float,float> getLoc(){
            return std::make_tuple(x,y);
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
        Sensor(std::string assign)
        :name(assign)
        {}
        std::string getName() const{
            return name;
        }
        virtual std::string getType() const{
            return "Generic Sensor";
        }
        ~Sensor(){}
};

class GPS : public Sensor {
    private:
        float recx;
        float recy;
    public:
        GPS(const std::string& assign)
        :Sensor(assign){};
        std::string getType() const{
            return "GPS";
        }
        std::tuple<float,float> readLoc(std::shared_ptr<Projectile> target){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> distr(-0.5f,0.5f);
            auto [locx,locy] = target->getLoc();
            return std::make_tuple(locx+distr(gen),locy+distr(gen)); 
        }
};

class kalmanFilter {
    private:
        float dt;

        std::vector<float> state;
        std::vector<std::vector<float>> P;
        std::vector<std::vector<float>> F;
        std::vector<std::vector<float>> H;
        std::vector<std::vector<float>> Q;
        std::vector<std::vector<float>> R;
        std::vector<std::vector<float>> K;

    public:
        kalmanFilter(float res)
        :dt(res)
        {
            state = std::vector<float>(4, 0.0f);
            F = std::vector<std::vector<float>>(4,std::vector<float>(4,0.0f));
            P = std::vector<std::vector<float>>(4,std::vector<float>(4,0.0f));
            Q = std::vector<std::vector<float>>(4,std::vector<float>(4,0.0f));
            K = std::vector<std::vector<float>>(4,std::vector<float>(2,0.0f));
            H = std::vector<std::vector<float>>(2,std::vector<float>(4,0.0f));
            R = std::vector<std::vector<float>>(2,std::vector<float>(2,0.0f));

            for (int n = 0; n < 4; n++){
                for (int m = 0; m < 4; m++){
                    F[n][m] = (n == m) ? 1.0f : 0.0f;
                    P[n][m] = (n == m) ? 1.0f : 0.0f;
                    Q[n][m] = (n == m) ? 0.01f : 0.0f;
                }
            }
            F[0][2] = dt;
            F[1][3] = dt;
            for (int n = 0; n < 2; n++){
                for (int m = 0; m < 2; m++){
                    R[n][m] = (n == m) ? .1f:0.0f;
                }
            }

        }
        void predict(){
            std::vector<float> state_pred(4.0, 0.0f);
            state = state;
        }

};

int main(){
    float velocity;
    float deg;
    float altitude;
    float n = 0;
    float resolution = .01;
    std::cout<<"Enter Velocity : "<<std::endl;
    std::cin>>velocity;
    std::cout<<"Enter Angle (deg) : "<<std::endl;
    std::cin>>deg;

    std::shared_ptr<Projectile>P1 = std::make_shared<Projectile>(0,0,velocity,deg,resolution);
    std::unique_ptr<GPS> GPS1 = std::make_unique<GPS>("GPS1");
    std::cout<<"sensor ("<<GPS1->getType()<<") : "<<GPS1->getName()<<std::endl;

    auto loc = P1->getLoc();
    std::cout<<"t ("<<n<<") "<<"x : "<<std::get<0>(loc)<<" y : "<<std::get<1>(loc)<<std::endl;
    n = n + resolution;
    P1->update();
    loc = GPS1->readLoc(P1);
    altitude = std::get<1>(loc);
    std::cout<<"t ("<<n<<") "<<"x : "<<std::get<0>(loc)<<" y : "<<std::get<1>(loc)<<std::endl;

    while(altitude >= 0 || n < 1.0){
        n = n + resolution;
        P1->update();
        loc = GPS1->readLoc(P1);
        altitude = std::get<1>(loc);
        std::cout<<"t ("<<n<<") "<<"x : "<<std::get<0>(loc)<<" y : "<<std::get<1>(loc)<<std::endl;
    }
    return 0;   
}