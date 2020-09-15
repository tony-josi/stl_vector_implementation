
/** 
 *  @file   test_main.cpp
 *  @brief  Vector implementation test cases.
 *
 *  This file contains tests for the vector Implementation.
 *
 *  @author         Tony Josi   https://tonyjosi97.github.io/profile/
 *  @copyright      Copyright (C) 2020 Tony Josi
 *  @bug            No known bugs.
 */

#include <iostream>
#include "vector/inc/vector.hpp"

using namespace rtw_vect;

class test_class {
    
    public:
        test_class(int x, int y, int z)
            :x{x}
            ,y{y}
            ,z{z} {
                std::cout<<"Default test_class ctor\n";
        }

        test_class(const test_class &rhs) 
            :x{rhs.x}
            ,y{rhs.y}
            ,z{rhs.z} {
                std::cout<<"Copy test_class ctor\n";
        }

        test_class(test_class &&rhs) 
            :x{rhs.x}
            ,y{rhs.y}
            ,z{rhs.z} {
                std::cout<<"Move test_class ctor\n";
        }

        test_class & operator=(const test_class &rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            std::cout<<"Copy assign. test_class ctor\n";
            return *this;
        }

        
        test_class & operator=(test_class &&rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            std::cout<<"Move assign. test_class ctor\n";
            return *this;
        }
        
    private:
        int x, y, z;
};

int main() {

    vector<int> vect_int;
    for(std::size_t i = 0; i < 19; ++i) 
        vect_int.push_back(static_cast<int>(23 * (i + 1)));

    vector<int> vect_int_copy(vect_int);
    vector<int> vect_int_copy_assign;
    vect_int_copy_assign = vect_int_copy;

    std::cout<<"Before move\n";

    for(std::size_t i = 0; i < 19; ++i) 
        std::cout<<i<<".    "<<vect_int_copy[i]<<"\n";

    std::cout<<"---------------------------------------------------------\n";
    std::cout<<"\t\tTesting Copy Assign. \n";
    std::cout<<"---------------------------------------------------------\n";

    vector<double> vect_float;
    for(int i = 0; i < 1000; ++i)
        vect_float.push_back(3.14 * i);

    vector<double> vect_float_copy_assign;
    vect_float_copy_assign = vect_float;
    //vect_float.pop_back();

    std::cout<<"---------------------------------------------------------\n";
    std::cout<<"\t\tTesting == & != \n";
    std::cout<<"---------------------------------------------------------\n";

    if(vect_float_copy_assign == vect_float)
        std::cout<<"Copy assigns equal\n";

    if(vect_float_copy_assign != vect_float)
        std::cout<<"Copy assigns not equal\n";
    else
        std::cout<<"Copy assigns equal\n";

    std::cout<<"---------------------------------------------------------\n";
    std::cout<<"\t\tTesting Move Constructor. \n";
    std::cout<<"---------------------------------------------------------\n";

    /* vect_int_copy is not destroyed right after move, refer to the docs -> c++/readme.md */
    vector<int> moved_obj(std::move(vect_int_copy));

    std::cout<<"After move\n";

    /* Testing pop_back() */
    moved_obj.pop_back();
    moved_obj.push_back(0xFCCD);

    for(std::size_t i = 0; i < 19; ++i) 
        std::cout<<i<<".    "<<moved_obj[i]<<"\n";

    vector<int> init_list_vec {4,5,6,7,8,8,23,65};
    for(std::size_t i = 0; i < 7; ++i) 
        std::cout<<i<<".    "<<init_list_vec[i]<<"\n";

    std::cout<<"---------------------------------------------------------\n";
    std::cout<<"\t\tTesting Init. List Constructor. \n";
    std::cout<<"---------------------------------------------------------\n";

    vector<std::string> init_list_vec_str {"Hi", "My", "Name", "Is", "TONY"};
    init_list_vec_str.push_back("Josi");
    init_list_vec_str.at(5) = "JOSI";
    for(std::size_t i = 0; i < 6; ++i) 
        std::cout<<i<<".    "<<init_list_vec_str.at(i)<<"\n";

    vect_int.reserve(200);

    std::cout<<"---------------------------------------------------------\n";
    std::cout<<"\t\tTesting Emblace Back\n";
    std::cout<<"---------------------------------------------------------\n";

    vector<test_class> test_cls_vect;
    test_cls_vect.emblace_back(1,2,3);
    test_cls_vect.push_back(test_class{3,4,5});

    std::cout<<"---------------------------------------------------------\n";
    std::cout<<"\t\tTesting Move Push Back\n";
    std::cout<<"---------------------------------------------------------\n";

    test_class mov_candidate(7,8,9);
    test_cls_vect.push_back(std::move(mov_candidate));

    std::cout<<"---------------------------------------------------------\n";
    std::cout<<"\t\tTesting pop_back \n";
    std::cout<<"---------------------------------------------------------\n";
    vector<int> pop_test;
    pop_test.pop_back();

    std::cout<<"---------------------------------------------------------\n";
    std::cout<<"\t\tTesting ctor type called during temp assignment\n";
    std::cout<<"---------------------------------------------------------\n";

    test_class tc(1,2,3);
    tc = test_class(3,4,5);

    std::cout<<"---------------------------------------------------------\n";
    std::cout<<"\t\tTesting Done Destroying... \n";
    std::cout<<"---------------------------------------------------------\n";


}
