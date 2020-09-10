#include <iostream>
#include "vector/inc/vector.hpp"

using namespace rtw_vect;

int main() {

    vector<int> vect_int;
    for(int i = 0; i < 19; ++i) 
        vect_int.push_back(23 * (i + 1));

    vector<int> vect_int_copy(vect_int);
    vector<int> vect_int_copy_assign;
    vect_int_copy_assign = vect_int_copy;

    std::cout<<"Before move\n";

    for(int i = 0; i < 19; ++i) 
        std::cout<<i<<".    "<<vect_int_copy[i]<<"\n";

    /* vect_int_copy is not destroyed right after move, refer to the docs -> c++/readme.md */
    vector<int> moved_obj(std::move(vect_int_copy));

    std::cout<<"After move\n";

    for(int i = 0; i < 19; ++i) 
        std::cout<<i<<".    "<<moved_obj[i]<<"\n";

}
