#include "user.h"

#include <iostream>

User::User()
    : id(0), name(""), gender(""), age(0), height(0.0), weight(0.0), bmi(0.0),
      category(""), advice(""), risk("") {}

User::User(int id_, const std::string &name_, const std::string &gender_,
           int age_, double height_, double weight_, double bmi_,
           const std::string &category_, const std::string &advice_,
           const std::string &risk_)
    : id(id_), name(name_), gender(gender_), age(age_), height(height_),
      weight(weight_), bmi(bmi_), category(category_), advice(advice_),
      risk(risk_) {}

int User::get_id() const { return id; }
std::string User::get_name() const { return name; }
std::string User::get_gender() const { return gender; }
int User::get_age() const { return age; }
double User::get_height() const { return height; }
double User::get_weight() const { return weight; }
double User::get_bmi() const { return bmi; }
std::string User::get_category() const { return category; }
std::string User::get_advice() const { return advice; }
std::string User::get_risk() const { return risk; }

void User::set_id(int id_) { id = id_; }
void User::set_name(const std::string &name_) { name = name_; }
void User::set_gender(const std::string &gender_) { gender = gender_; }
void User::set_age(int age_) { age = age_; }
void User::set_height(double height_) { height = height_; }
void User::set_weight(double weight_) { weight = weight_; }
void User::set_bmi(double bmi_) { bmi = bmi_; }
void User::set_category(const std::string &category_) { category = category_; }
void User::set_advice(const std::string &advice_) { advice = advice_; }
void User::set_risk(const std::string &risk_) { risk = risk_; }

std::string User::to_csv() const {
  std::ostringstream oss;
  oss << id << "|" << name << "|" << gender << "|" << age << "|" << height
      << "|" << weight << "|" << bmi << "|" << category << "|" << advice << "|"
      << risk;
  return oss.str();
}

User User::from_csv(const std::string &csvLine) {
  std::istringstream iss(csvLine);
  std::string token;

  int id = 0;
  std::string name, gender, category, advice, risk;
  int age = 0;
  double height = 0.0, weight = 0.0, bmi = 0.0;

  try {
    if (std::getline(iss, token, '|')) {
      id = std::stoi(token);
    }
    std::getline(iss, name, '|');
    std::getline(iss, gender, '|');
    if (std::getline(iss, token, '|')) {
      age = std::stoi(token);
    }
    if (std::getline(iss, token, '|')) {
      height = std::stod(token);
    }
    if (std::getline(iss, token, '|')) {
      weight = std::stod(token);
    }
    if (std::getline(iss, token, '|')) {
      bmi = std::stod(token);
    }
    std::getline(iss, category, '|');
    std::getline(iss, advice, '|');
    std::getline(iss, risk);
  } catch (const std::exception &e) {
    std::cerr << "Warning: skipping malformed CSV record: " << e.what() << "\n";
  }

  return User(id, name, gender, age, height, weight, bmi, category, advice,
              risk);
}