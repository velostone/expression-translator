#include "gtest.h"
#include "stack_queue.h"

TEST(Queue, can_create_queue)
{
    ASSERT_NO_THROW(Queue<int> q);
}

TEST(Queue, can_check_for_full)
{
    Queue<int> q;
    EXPECT_FALSE(q.isFull());
}

TEST(Queue, can_check_for_empty)
{
    Queue<int> q;
    EXPECT_TRUE(q.isEmpty());
}

TEST(Queue, can_get_size)
{
    Queue<int> q;
    q.push(8); q.push(8); q.push(8); q.push(8);
    EXPECT_EQ(4, q.size());
}

TEST(Queue, can_get_capacity)
{
    Queue<int> q;
    EXPECT_EQ(2, q.getCapacity());
}

TEST(Queue, can_push_element)
{
    Queue<int> q;
    q.push(8); q.push(10);
    EXPECT_EQ(10, q.getBack());
}

TEST(Queue, can_pop_element)
{
    Queue<int> q;
    q.push(8); q.push(10);
    q.pop();
    EXPECT_EQ(1, q.size());
}

TEST(Queue, throws_when_pop_empty_queue)
{
    Queue<int> q;
    ASSERT_ANY_THROW(q.pop());
}

TEST(Queue, can_get_front)
{
    Queue<int> q;
    q.push(10); q.push(8);
    EXPECT_EQ(10, q.getFront());
}

TEST(Queue, can_get_back)
{
    Queue<int> q;
    q.push(10); q.push(8);
    EXPECT_EQ(8, q.getBack());
}

TEST(Queue, can_fill_correctly)
{
    Queue<int> q;
    q.push(8); q.push(10); // full
    q.pop(); // освобождаем front
    q.push(14); // back == 0
    EXPECT_EQ(0, q.getBackIndex());
}