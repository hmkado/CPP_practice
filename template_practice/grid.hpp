#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename T>
class Grid{
    private:
        std::vector<std::vector<T>> data;
        int rows, cols;

    public:
        Grid(int r, int c, T default_value = T())
        :rows(r), cols(c), data(r, std::vector<T>(c, default_value))
        {

        }
        void set(int row, int col, T value){
            checkBounds(row, col);
            data[row][col] = value;
        }
        T get(int row, int col) const {
            checkBounds(row,col);
            return data[row][col];
        }
        void print() const {
            for (int i = rows - 1; i >= 0; i--){
                for (int j = 0; j < cols; j++){
                    std::cout<<std::setw(4)<<data[i][j];
                }
                std::cout<<std::endl;
            }
        }

        int numRows() const{return rows;}
        int numCols() const{return cols;}

    private:
        void checkBounds(int row, int col) const {
            if(row < 0 || row >= rows || col < 0 || col >= cols){
                throw std::out_of_range("Grid index out of bounds");
            }
        }
};
