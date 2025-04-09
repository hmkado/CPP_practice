// Simulated Sensor Data Operation

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

void swap(float* a, float* b){
    float temp = *a;
    *a = *b;
    *b = temp;
}

int partition(float* data, int n, int m){
    swap(&data[(n+m)/2],&data[m]);
    float pivot = data[m];
    int p = (n-1);
    for (int i = n; i < m;i++){
        if (data[i] <= pivot){
            p++;
            swap(&data[i],&data[p]);
        }
    }
    swap(&data[m],&data[++p]);
    return p;
}

void quicksort(float* data,int n, int m){
    int pointer = 0;
    if (n < m){
        pointer = partition(data,n,m);
        quicksort(data,n,pointer-1);
        quicksort(data,pointer+1,m);
    }
}

void fillTemperatures(float* data, int size){
    for (int i = 0; i<size;i++){
        data[i] = (rand()%4000) / 100.0f;
    }
}

float calculateAverage(const float* data, int size){
    float sum = 0.0f;
    for (int i = 0;i<size;i++){
        sum += data[i];
    }
    return sum/size;
}

void findMinMax(const float* data, int size, float* minVal, float* maxVal){
    float copy[size];
    *minVal = std::numeric_limits<float>::max();
    *maxVal = std::numeric_limits<float>::min();
    for(int i = 0;i< size;i++) copy[i] = data[i];
    quicksort(copy,0,size-1);
    *minVal = copy[0];
    *maxVal = copy[size-1];
}

int main(){
    const int N = 10;
    float temperatures[N];
    srand(time(0));

    fillTemperatures(temperatures,N);
    std::cout<<"Sensor Readings:\n";
    for (int i=0;i<N;i++){
        std::cout<<temperatures[i]<<"C\n";
    }
    float avg = calculateAverage(temperatures,N);
    float* minTemp = (float*)malloc(sizeof(float));
    float* maxTemp = (float*)malloc(sizeof(float));
    
    findMinMax(temperatures,N,minTemp,maxTemp);

    std::cout<<"Max Temp: "<<*maxTemp<<"C\n";
    std::cout<<"Min Temp: "<<*minTemp<<"C\n";
    free(minTemp);
    free(maxTemp);
    return 0;
}