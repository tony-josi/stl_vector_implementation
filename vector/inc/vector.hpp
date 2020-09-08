
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

#define  __VECT_IMPL_DEBUG_OP 

#ifdef  __VECT_IMPL_DEBUG_OP 
    #include <iostream>
    #define     _rtw_DEBUG_OP(_str__)    std::cout<<_str__;
#else
    #define     _rtw_DEBUG_OP(_str__)
#endif

/* namespace: Reinventing the wheel (rtw) vector */
namespace rtw_vect {

    template <typename T>
    class vector {

        public:
            
            /* Constructors & Assignment operators */
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

            }

            vector& operator=(vector const &rhs) {
                
                /* SFINAE based overload for copying elements based on T */
                copy_assign<T>(rhs);
                return *this;
            
            }


            vector(vector &&rhs)                    noexcept;
            vector& operator=(vector const &&rhs)   noexcept;

            /* Main functions */
            void push_back(T const& val);
            void pop_back();

            T & operator[](std::size_t idx);
            T const & operator[](std::size_t idx)   const;

        private:
            /* Internal members */
            std::size_t     size__;
            std::size_t     len__;
            T              *mem_buff__;

            /* Helper functions */
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
                for(std::size_t i = len__ - 1; i >= 0; --i)
                    mem_buff__[i].~T();
            
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
                        push_back__(rhs[i]);
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

            void push_back__(T const& val);

            /* Functor for destroying the memory buffer */
            struct t_buff_destructor {

                void operator()(T *buff)    const {
                    ::operator delete(buff);
                }

            };
            
    };

}

#endif /* __VECT_IMPL__HEADER__ */