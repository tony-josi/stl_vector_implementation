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

    /* Testing pop_back() */
    moved_obj.pop_back();
    moved_obj.push_back(0xFCCD);

    for(int i = 0; i < 19; ++i) 
        std::cout<<i<<".    "<<moved_obj[i]<<"\n";

    vector<int> init_list_vec {4,5,6,7,8,8,23,65};
    for(int i = 0; i < 7; ++i) 
        std::cout<<i<<".    "<<init_list_vec[i]<<"\n";

    vector<std::string> init_list_vec_str {"Hi", "My", "Name", "Is", "TONY"};
    init_list_vec_str.push_back("Josi");
    for(int i = 0; i < 6; ++i) 
        std::cout<<i<<".    "<<init_list_vec_str.at(i)<<"\n";

}
