// Copyright 2020 Your Name <your_email>

#include <gtest/gtest.h>
#include <SharedPtr.hpp>

struct Person {
  int age;
  bool gender;
  float salary;
};

TEST(Constructors, constructors) {
  SharedPtr<int> p0;
  EXPECT_EQ(p0.get(), nullptr);

  SharedPtr<int> p1(new int{5});
  SharedPtr<int> p2(p1);
  EXPECT_EQ(*p2, *p1);

  *p1 = 7;
  EXPECT_EQ(*p2, 7);

  {
    SharedPtr<int> p3 = std::move(p1);
    EXPECT_EQ(p1.get(), nullptr);
    EXPECT_EQ(*p3, *p2);
    EXPECT_EQ(p3.use_count(), 2);
    EXPECT_EQ(p2.use_count(), 2);
  }
  EXPECT_EQ(p2.use_count(), 1);
}

TEST(Functions, functions){
  SharedPtr<Person> person1(new Person{30, true, 1000.0});
  SharedPtr<Person> person1_other_ref(person1);

  EXPECT_TRUE(person1_other_ref->gender);
  EXPECT_EQ(person1_other_ref->age, 30);

  person1->age++;
  EXPECT_EQ(person1_other_ref->age, 31);

  person1_other_ref->salary += 100;
  EXPECT_EQ(person1->salary, 1100.0);

  SharedPtr<Person> person1_other_ref2;
  person1_other_ref2 = person1;

  EXPECT_EQ(person1_other_ref2.use_count(), 3);
  EXPECT_EQ(person1_other_ref2->age, 31);

  person1_other_ref2->gender = false;
  EXPECT_FALSE(person1->gender);

  person1_other_ref.reset();
  EXPECT_FALSE(person1_other_ref);
  EXPECT_EQ(person1_other_ref2.use_count(), 2);

  auto p2 = new Person{10, true, 0.0};
  person1_other_ref2.reset(p2);

  EXPECT_EQ(person1.use_count(), 1);
  EXPECT_TRUE(person1_other_ref2);
  EXPECT_EQ(person1_other_ref2->age, 10);

  person1.swap(person1_other_ref2);
  EXPECT_EQ(person1->age, 10);
  EXPECT_EQ(person1_other_ref2->age, 31);
}
