#pragma once

#include "user.h"

#include <string>

struct BMIResult {
  std::string category;
  std::string advice;
  std::string risk;
};

class BMIService {
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
