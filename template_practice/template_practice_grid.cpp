#include "grid.hpp"
#include <memory>

int main(){ 
    std::unique_ptr<Grid<int>> intGrid = std::make_unique<Grid<int>>(5,5,0);
    intGrid->print();
    intGrid->set(2,2,42);

    intGrid->print();
    std::cout<<intGrid->get(2,2)<<std::endl;
}