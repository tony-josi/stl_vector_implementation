
/** 
 *  @file   vector.hpp
 *  @brief  Vector implementation header.
 *
 *  This file contains the Header Code for the vector Implementation.
 *
 *  @author         Tony Josi   https://tonyjosi97.github.io/profile/
 *  @copyright      Copyright (C) 2020 Tony Josi
 *  @bug            No known bugs.
 */

#ifndef __VECT_IMPL__HEADER__
#define __VECT_IMPL__HEADER__

#include <type_traits>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <iostream>

/* namespace: Reinventing the wheel (rtw) vector */
namespace rtw_vect {

#define     __VECT_IMPL_DEBUG_OP 

#ifdef      __VECT_IMPL_DEBUG_OP 
    #define     _rtw_DEBUG_OP(_str__)    std::cout<<_str__;
#else
    #define     _rtw_DEBUG_OP(_str__)
#endif      /* __VECT_IMPL_DEBUG_OP */

    /* Vector class */
    template <typename T>
    class vector {

        public:
            
            /* Constructors, Assignment operators & Destructor. */

            vector(std::size_t sz = 5)
                :size__         {     sz  }
                ,len__          {     0   }
                ,mem_buff__     {     static_cast<T *>(::operator new(sizeof(T) * size__))   } {
                    _rtw_DEBUG_OP("Default ctor\n");
            }

            vector(vector const &rhs)
                :size__         {   rhs.size__  }
                ,len__          {   0           }
                ,mem_buff__     {   static_cast<T *>(::operator new(sizeof(T) * size__))    } {

                try {

                    /* push_back each element to the destination object (lhs) 
                    from rhs object */
                    for(std::size_t i = 0; i < rhs.len__; ++i)
                        push_back(rhs.mem_buff__[i]);
                
                }
                catch(...) {
                    /* Delete mem_buff__  before throwing, uses t_buff_destrutor() functor to 
                    delete the allocated memory */
                    std::unique_ptr<T, t_buff_destructor>      dctor_obj(mem_buff__, t_buff_destructor());

                    /* SFINAE based overload for destroying individual elements */
                    destroy_items<T>();

                    /* finally throw exception */
                    throw;

                }

                _rtw_DEBUG_OP("Copy ctor\n");

            }

            vector& operator=(vector const &rhs) {

                _rtw_DEBUG_OP("Copy assign. ctor\n");
                
                /* SFINAE based overload for copying elements based on T */
                copy_assign<T>(rhs);
                return *this;
            
            }

            vector(vector &&rhs)                    noexcept 
                :size__         {   0   }
                ,len__          {   0   }
                ,mem_buff__     {   nullptr } {

                /* Swap the resources so that rhs object will have 
                no resources pointed by it and also reset the len__ to zero as the destroy_items()
                won't try to free those moved elements. */
                rhs.swap(*this);
                _rtw_DEBUG_OP("Move ctor\n");

            }

            vector& operator=(vector &&rhs)         noexcept {

                rhs.swap(*this);
                _rtw_DEBUG_OP("Move assign. ctor\n");

                /* Return the current object. */
                return *this;

            }

            ~vector() {

                /* Using std::unique_ptr for ensuring resource
                deallocation even if exception occur during the destruction of
                individual vector elements. 
                NOTE: For std::unique_ptr<T> the type doesn't need pointer operator (*) 
                as type inside std::unique_ptr class itself is a pointer. */
                std::unique_ptr<T, t_buff_destructor>   dctor_obj(mem_buff__, t_buff_destructor());
                destroy_items<T>();
                _rtw_DEBUG_OP("Dctor\n");

            }

            /* Public member functions */

            void push_back(T const& val) {

                if(len__ == size__)
                    resize__();

                push_back_copy__(val);

            }

            void pop_back() {

                --len__;
                /* Calling T's destructor on the deleted object. */
                mem_buff__[len__].~T();

            }

            T & operator[](std::size_t idx) {
                return mem_buff__[idx];
            }

            T const & operator[](std::size_t idx)   const {
                return mem_buff__[idx];
            }

        private:

            /* Internal data members */

            std::size_t     size__;
            std::size_t     len__;
            T              *mem_buff__;


            /* Internal helper member functions */

            void swap(vector &rhs)                  noexcept {

                using std::swap;
                swap(size__,        rhs.size__);
                swap(len__,         rhs.len__);
                swap(mem_buff__,    rhs.mem_buff__);
            
            }

            template <typename U>
            typename std::enable_if <(std::is_trivially_destructible <U>::value == false), void>::type
            destroy_items() {

                /* Call destructor of each element in reverse order. */
                for(std::size_t i = len__; i > 0; --i)
                    mem_buff__[i - 1].~T();
            
            }

            template <typename U>
            typename std::enable_if <(std::is_trivially_destructible <U>::value == true), void>::type
            destroy_items() {

                /* Nothing to do, as each elements are trivially destructible */
                return;
            
            }

            template <typename U>
            typename std::enable_if <((std::is_trivially_destructible <U>::value &&
            std::is_nothrow_copy_constructible <U>::value) == true), void>::type
            copy_assign(vector<T> const &rhs) {

                /* if the vector type, T is_nothrow_copy_constructible and
                is_trivially_destructible then reuse memory if available else
                do sperate memory allocation. */

                /* check for self comparisson */
                if(this == &rhs)
                    return;

                if(size__ <= rhs.len__) {
                    /* clear the previous elements */
                    destroy_items<T>();
                    /* resuse the destination memory */
                    len__ = 0;
                    for(std::size_t i = 0; i < rhs.len__; ++i)
                        push_back_copy__(rhs[i]);
                }
                else {
                    /* If there isnt enough memory then allocate again */
                    vector<T> temp_copy(rhs);
                    temp_copy.swap(*this);
                }

            }

            template <typename U>
            typename std::enable_if <((std::is_trivially_destructible <U>::value &&
            std::is_nothrow_copy_constructible <U>::value) == false), void>::type
            copy_assign(vector<T> const &rhs) {
                
                /* if the vector type, T cannot be nothrow_copy_constructible 
                then do sperate memory allocation, the usual way. */
                vector<T> temp_copy(rhs);
                temp_copy.swap(*this);

            }

            template <typename U>
            typename std::enable_if <(std::is_nothrow_move_constructible <U>::value == true), void>::type
            copy_items(vector<T> &dest) {
                
                /* if the vector type, T is nothrow_move_constructible 
                then move the elements to the buffer */
                std::for_each(mem_buff__, mem_buff__ + len__, [&dest](T &item){dest.push_back_move__(std::move(item));});

            }

            template <typename U>
            typename std::enable_if <(std::is_nothrow_move_constructible <U>::value == false), void>::type
            copy_items(vector<T> &dest) {

                /* if the vector type, T is not nothrow_move_constructible 
                then copy the elements to the buffer */
                std::for_each(mem_buff__, mem_buff__ + len__, [&dest](T const &item){dest.push_back_copy__(item);});
                
            }

            /* Initialisation of memory using copy constructor of T. */
            void push_back_copy__(T const &val) {

                /* Initialise T object in location (mem_buff__ + len__) 
                using placement new and copy constructor of T. */
                new (mem_buff__ + len__) T(val);
                ++len__;

            }

            /* Initialisation of memory using move constructor of T. */
            void push_back_move__(T &&val) {

                /* Initialise T object in location (mem_buff__ + len__) 
                using placement new and move constructor of T. */
                new (mem_buff__ + len__) T(std::move(val));
                ++len__;

            }

            void resize__() {

                /* Calculate new vector size and do allocation and 
                copying of old data back to resized vector. */
                std::size_t new_size = std::max(static_cast<std::size_t>(2), size__ * 2);
                reserve_n_copy__(new_size);

            }

            void reserve_n_copy__(std::size_t new_sz) {

                /* Create temporary vector with new size. */
                vector<T> temp_vect(new_sz);

                /* Copy previous items to the temporary vector 
                and swap with *this. */
                copy_items<T>(temp_vect);
                temp_vect.swap(*this);

            }

            /* Functor for destroying the memory buffer */
            struct t_buff_destructor {

                void operator()(T *buff)    const {
                    ::operator delete(buff);
                }

            };
    };
}

#endif /* __VECT_IMPL__HEADER__ */