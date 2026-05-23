#pragma once

#include "user.h"

#include <string>

struct BMIResult {
  std::string category;
  std::string advice;
  std::string risk;
};

class BMIService {
private:
  static constexpr double UNDERWEIGHT_THRESHOLD = 18.5;
  static constexpr double NORMAL_WEIGHT_THRESHOLD = 25.0;
  static constexpr double OVERWEIGHT_THRESHOLD = 30.0;
  static constexpr double OBESE_CLASS_I_THRESHOLD = 35.0;
  static constexpr double OBESE_CLASS_II_THRESHOLD = 40.0;

public:
  static constexpr double POUND_TO_KILOGRAM = 0.453592;
  static constexpr double FEET_TO_METER = 0.3048;
  static constexpr double CM_TO_METERS = 100.0;

  static double calculateBMI(double weightKg, double heightMeters);
  static BMIResult classifyBMI(double bmi);
  static double convertMass(double mass, bool isPoundToKg = true);
  static double convertHeightToCm(double feet);
  static void applyToUser(User &user);
};
