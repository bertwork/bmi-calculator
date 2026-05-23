#include "bmi_service.h"

double BMIService::calculateBMI(double weight, double height) {
  if (height <= 0.0 || weight <= 0.0) {
    return 0.0;
  }
  return weight / (height * height);
}

BMIResult BMIService::classifyBMI(double bmi) {
  BMIResult result;

  if (bmi < UNDERWEIGHT_THRESHOLD) {
    result.category = "Underweight";
    result.advice = "Increase caloric intake with nutritious foods.";
    result.risk = "Malnutrition, osteoporosis, weakened immunity.";
  } else if (bmi < NORMAL_WEIGHT_THRESHOLD) {
    result.category = "Normal weight";
    result.advice =
        "Maintain current habits with balanced diet and exercise.";
    result.risk = "Low risk - keep it up!";
  } else if (bmi < OVERWEIGHT_THRESHOLD) {
    result.category = "Overweight";
    result.advice = "Reduce refined sugars and increase physical activity.";
    result.risk = "Elevated risk of diabetes and heart disease.";
  } else if (bmi < OBESE_CLASS_I_THRESHOLD) {
    result.category = "Obese Class I";
    result.advice = "Consult a healthcare provider for a weight-loss plan.";
    result.risk = "High risk of hypertension, diabetes, sleep apnea.";
  } else if (bmi < OBESE_CLASS_II_THRESHOLD) {
    result.category = "Obese Class II";
    result.advice = "Medical supervision strongly recommended.";
    result.risk = "Very high cardiovascular and metabolic risk.";
  } else {
    result.category = "Obese Class III";
    result.advice = "Seek immediate medical guidance.";
    result.risk = "Severe risk - life-threatening complications possible.";
  }

  return result;
}

double BMIService::convertMass(double mass, bool isPoundToKg) {
  if (isPoundToKg) {
    return mass * POUND_TO_KILOGRAM;
  }
  return mass / POUND_TO_KILOGRAM;
}

double BMIService::convertHeightToCm(double feet) {
  return feet * FEET_TO_METER * CM_TO_METERS;
}

void BMIService::applyToUser(User &user) {
  const double heightMeters = user.get_height() / CM_TO_METERS;
  const double bmi = calculateBMI(user.get_weight(), heightMeters);
  const BMIResult result = classifyBMI(bmi);

  user.set_bmi(bmi);
  user.set_category(result.category);
  user.set_advice(result.advice);
  user.set_risk(result.risk);
}
