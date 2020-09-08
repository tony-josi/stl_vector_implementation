
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

/* namespace: Reinventing the wheel (rtw) vector */
namespace rtw_vect {

    template <typename T>
    class vector {

        public:
            /* Constructors & Assignment operators */
            vector(std::size_t sz = 5);
            vector(vector const &rhs);
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
    };

}

#endif /* __VECT_IMPL__HEADER__ */