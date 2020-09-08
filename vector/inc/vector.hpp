
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
            }

            vector(vector const &rhs)
                :size__         {   rhs.size__  }
                ,len__          {   0           }
                ,mem_buff__     {   static_cast<T *>(::operator new(sizeof(T) * size__))    } {

                try {
                    for(std::size_t i = 0; i < rhs.len__; ++i)
                        push_back(rhs.mem_buff__[i]);
                }
                catch(...) {

                    std::unique_ptr<T, t_buff_destrutor>      dctor_obj(mem_buff__, t_buff_destrutor());
                    destroy_items<T>();
                    throw;

                }

            }


            vector(vector &&rhs)                    noexcept;
            vector& operator=(vector const &rhs);
            vector& operator=(vector const &&rhs)   noexcept;

            /* Main functions */
            void push_back(T const& val);
            void pop_back();

            T & operator[](std::size_t idx);
            T const & operator[](std::size_t idx);

        private:
            /* Internal members */
            std::size_t     size__;
            std::size_t     len__;
            T              *mem_buff__;

            /* Helper functions */
            void swap(vector &rhs)                  noexcept;

            template <typename U>
            typename std::enable_if <(std::is_trivially_destructible <U>::value == false), void>::type
            destroy_items() {

                for(std::size_t i = len__ - 1; i >= 0; --i)
                    mem_buff__[i].~T();
            
            }

            template <typename U>
            typename std::enable_if <(std::is_trivially_destructible <U>::value == true), void>::type
            destroy_items() {

                return;
            
            }

            /* Functor for destroying the memory buffer */
            struct t_buff_destructor {

                void operator()(T *buff)    const {
                    ::operator delete(buff);
                }

            }
            
    };

}

#endif /* __VECT_IMPL__HEADER__ */