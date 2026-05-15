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