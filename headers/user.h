#pragma once

#include <sstream>
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

  int get_id() const;
  std::string get_name() const;
  std::string get_gender() const;
  int get_age() const;
  double get_height() const;
  double get_weight() const;
  double get_bmi() const;
  std::string get_category() const;
  std::string get_advice() const;
  std::string get_risk() const;

  void set_id(int id);
  void set_name(const std::string &name);
  void set_gender(const std::string &gender);
  void set_age(int age);
  void set_height(double height);
  void set_weight(double weight);
  void set_bmi(double bmi);
  void set_category(const std::string &category);
  void set_advice(const std::string &advice);
  void set_risk(const std::string &risk);

  std::string to_csv() const;
  static User from_csv(const std::string &csvLine);
};
