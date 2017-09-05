/* fifo.h
* this file is part of Dynstack/RemoteControl for CORSIKA
*
* Copyright (C) <2016> <Dominik Baack>
*		All rights reserved.
*
* 	This software may be modified and distributed under the terms
* 	of the LGPL license. See the LICENSE file for details.
*/

#include "dynstack/stack/storage/fifo_stack.h"
#include "dynstack/stack/wrapper/in_modify_stack.h"

namespace test
{
    namespace dynstack
    {

        float modify(const float* const data)
        {
            return *data + 1.0f;
        }

        TEST(InModify, capacity)
        {
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp(12);

            ASSERT_EQ(tmp.capacity(), 12);
        }

        TEST(InModify, size)
        {
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp(12);

            ASSERT_EQ(tmp.size(), 0);

            tmp.push_back(3.0f);
            ASSERT_EQ(tmp.size(), 1);

            for(int i=0; i<11; i++)
            {
                tmp.push_back(3.0f);
                ASSERT_EQ(2 + i, tmp.size());
            }

            tmp.push_back(3.0f);
            ASSERT_EQ(tmp.size(), 12);
            tmp.push_back(3.0f);
            ASSERT_EQ(tmp.size(), 12);
        }

        TEST(InModify, pushBack_reference)
        {
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp(12);

            float a = 1.0f;
            const float b = 2.0f;

            tmp.push_back(a);
            tmp.push_back(b);
            tmp.push_back(3.0f);

            ASSERT_EQ(tmp.size(), 3);
            ASSERT_EQ(tmp.pop_back(), 2.0f);
            ASSERT_EQ(tmp.pop_back(), 3.0f);
            ASSERT_EQ(tmp.pop_back(), 4.0f);
        }

        TEST(InModify, pushBack_move)
        {
            /*::dynstack::storage::InModify_Stack< std::unique_ptr<float> > tmp(12);

            std::unique_ptr<float> a(new float(1.0f));
            std::unique_ptr<float> b(new float(2.0f));
            std::unique_ptr<float> c(new float(3.0f));

            tmp.push_back( std::move(a) );
            tmp.push_back( std::move(b) );
            tmp.push_back( std::move(c) );


            ASSERT_EQ(tmp.size(), 3);
            ASSERT_EQ(*tmp.pop_back(), 1.0f);
            ASSERT_EQ(*tmp.pop_back(), 2.0f);
            ASSERT_EQ(*tmp.pop_back(), 3.0f);*/
        }

        TEST(InModify, pushBack_uniquePtr)
        {
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp(12);

            std::unique_ptr<float[]> a(new float[4]);
            a[0] = 1.0f;
            a[1] = 2.0f;
            a[2] = 3.0f;
            a[3] = 4.0f;

            tmp.push_back(std::move(a), 4);

            ASSERT_EQ(tmp.size(), 4);
            ASSERT_EQ(tmp.pop_back(), 2.0f);
            ASSERT_EQ(tmp.pop_back(), 3.0f);
            ASSERT_EQ(tmp.pop_back(), 4.0f);
            ASSERT_EQ(tmp.pop_back(), 5.0f);
        }

        TEST(InModify, pushBack_ptr)
        {
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp(12);

            float a[] = {1.0f, 2.0f, 3.0f, 4.0f};

            tmp.push_back(a, 4);

            ASSERT_EQ(tmp.size(), 4);
            ASSERT_EQ(tmp.pop_back(), 2.0f);
            ASSERT_EQ(tmp.pop_back(), 3.0f);
            ASSERT_EQ(tmp.pop_back(), 4.0f);
            ASSERT_EQ(tmp.pop_back(), 5.0f);
        }

        TEST(InModify, back)
        {
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp(12);

            // Test normal behaviour
            tmp.push_back(3.0f);
            ASSERT_EQ(tmp.back(), 4.0f);

            // Order correct?
            tmp.push_back(4.0f);
            ASSERT_EQ(tmp.back(), 4.0f);

            tmp.pop();
            ASSERT_EQ(tmp.back(), 5.0f);

            // Test back on an empty Stack
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp2(12);
            ASSERT_EQ(tmp2.back(), float());   // Returnes default constructed value
        }

        TEST(InModify, pop)
        {
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp(12);
            tmp.push_back(3.0f);

            // Test Normal behaviour
            ASSERT_EQ(tmp.size(), 1);
            tmp.pop();
            ASSERT_EQ(tmp.size(), 0);
            // Test already empty behaviour
            tmp.pop();
            ASSERT_EQ(tmp.size(), 0);

            // Test behaviour after overflow
            for(int i=0; i<12; i++)
            {
                tmp.push_back(3);
            }
            ASSERT_EQ(tmp.size(), 12);

            tmp.pop();
            ASSERT_EQ(tmp.size(), 11);

            // Test behaviour of a new initialized stack
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp2(12);
            tmp2.pop();

            ASSERT_EQ(tmp2.size(), 0);
        }

        TEST(InModify, popBack)
        {
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp(12);

            // Empty init behaviour
            ASSERT_EQ(tmp.size(), 0);
            ASSERT_EQ(tmp.pop_back(), 0.0f);
            ASSERT_EQ(tmp.size(), 0);

            // Normal behaviour
            tmp.push_back(3.0f);
            ASSERT_EQ(tmp.size(), 1);
            ASSERT_EQ(tmp.pop_back(), 4.0f);
            ASSERT_EQ(tmp.size(), 0);

            tmp.push_back(3.0f);
            tmp.push_back(4.0f);
            ASSERT_EQ(tmp.size(), 2);
            ASSERT_EQ(tmp.pop_back(), 4.0f);
            ASSERT_EQ(tmp.size(), 1);
            ASSERT_EQ(tmp.pop_back(), 5.0f);
            ASSERT_EQ(tmp.size(), 0);

            // Empty behaviour
            ASSERT_EQ(tmp.pop_back(), 0.0f);
            ASSERT_EQ(tmp.size(), 0);

            // Test behaviour after overflow
            for(int i=0; i<12; i++)
            {
                tmp.push_back(3.0f);
            }
            ASSERT_EQ(tmp.size(), 12);
            ASSERT_EQ(tmp.pop_back(), 4.0f);
            ASSERT_EQ(tmp.size(), 11);

        }

        TEST(InModify, pusbBack_UpperLimit)
        {
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp(12);

            for(unsigned int i=0; i<tmp.capacity(); i++)
            {
                tmp.push_back(i);
            }
            ASSERT_EQ(tmp.size(), tmp.capacity());

            // Default insert
            float a = 1.0f;
            const float b = 2.0f;

            tmp.push_back(a);
            tmp.push_back(b);
            tmp.push_back(3.0f);

            ASSERT_EQ(tmp.size(), tmp.capacity());

            // unique_ptr array insert
            std::unique_ptr<float[]> c(new float[4]);
            c[0] = 1.0f;
            c[1] = 2.0f;
            c[2] = 3.0f;
            c[3] = 4.0f;
            tmp.push_back(std::move(c), 4);

            ASSERT_EQ(tmp.size(), tmp.capacity());

            // Array insert
            float d[] = {1.0f, 2.0f, 3.0f, 4.0f};
            tmp.push_back(d, 4);

            ASSERT_EQ(tmp.size(), tmp.capacity());
        }


        TEST(InModify, clear)
        {
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp(12);

            // Empty test
            ASSERT_EQ(tmp.size(), 0);
            tmp.clear();
            ASSERT_EQ(tmp.size(), 0);

            // Fill Normal
            tmp.push_back( 1.0f );
            ASSERT_EQ(tmp.size(), 1);
            tmp.clear();
            ASSERT_EQ(tmp.size(), 0);

            for(unsigned int i=0; i<tmp.capacity(); i++)
            {
                tmp.push_back(i);
            }
            ASSERT_EQ(tmp.size(), 12);
            tmp.clear();
            ASSERT_EQ(tmp.size(), 0);

        }

        TEST(InModify, invalidRegion)
        {
            ::dynstack::wrapper::InModifyStack<::dynstack::storage::FIFO_Stack<float>, modify> tmp(12);

            tmp.push_back(2);
            ASSERT_EQ(tmp.size(), 1);

            auto backElement = tmp.back();
            float old = backElement;
            const bool ret = tmp.pop();

            tmp.push_back(1);

            ASSERT_EQ(old, backElement);
            ASSERT_EQ(ret, true);
        }

    }
}