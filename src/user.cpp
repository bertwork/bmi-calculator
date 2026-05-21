#include "user.h"

User::User()
    : id(0), name(""), gender(""), age(0), height(0.0), weight(0.0), bmi(0.0),
      category(""), advice(""), risk("") {}

User::User(int id, const std::string &name, const std::string &gender, int age,
           double height, double weight, double bmi,
           const std::string &category, const std::string &advice,
           const std::string &risk)
    : id(id), name(name), gender(gender), age(age), height(height),
      weight(weight), bmi(bmi), category(category), advice(advice), risk(risk) {
}

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

void User::set_id(int id) { this->id = id; }
void User::set_name(const std::string &name) { this->name = name; }
void User::set_gender(const std::string &gender) { this->gender = gender; }
void User::set_age(int age) { this->age = age; }
void User::set_height(double height) { this->height = height; }
void User::set_weight(double weight) { this->weight = weight; }
void User::set_bmi(double bmi) { this->bmi = bmi; }
void User::set_category(const std::string &category) {
  this->category = category;
}
void User::set_advice(const std::string &advice) { this->advice = advice; }
void User::set_risk(const std::string &risk) { this->risk = risk; }

std::string User::to_csv() const {
  std::ostringstream oss;
  oss << id << "," << name << "," << gender << "," << age << "," << height
      << "," << weight << "," << bmi << "," << category << "," << advice << ","
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

  if (std::getline(iss, token, ',')) {
    id = std::stoi(token);
  }
  std::getline(iss, name, ',');
  std::getline(iss, gender, ',');
  if (std::getline(iss, token, ',')) {
    age = std::stoi(token);
  }
  if (std::getline(iss, token, ',')) {
    height = std::stod(token);
  }
  if (std::getline(iss, token, ',')) {
    weight = std::stod(token);
  }
  if (std::getline(iss, token, ',')) {
    bmi = std::stod(token);
  }
  std::getline(iss, category, ',');
  std::getline(iss, advice, ',');
  std::getline(iss, risk);

  return User(id, name, gender, age, height, weight, bmi, category, advice,
              risk);
}