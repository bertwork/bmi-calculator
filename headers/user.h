#pragma once

#include <string>

class User {
private:
  int id;
  std::string name;
  std::string gender;
  int age;
  double height;
  double weight;
  double bmi;
  std::string category;
  std::string advice;
  std::string risk;

public:
  User();
  User(int id, const std::string &name, const std::string &gender, int age,
       double height, double weight, double bmi, const std::string &category,
       const std::string &advice, const std::string &risk);
};
