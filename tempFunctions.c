#include "tempFunctions.h"

float CelToFarh(float C) {
    return C * 9.0/5.0 + 32.0;
}

float CelToKel(float C) {
    return C + 273.15;
}

float FarhToCel(float F) {
    return (F - 32.0) * 5.0/9.0;
}

float FarhToKel(float F) {
    return (F + 459.67) * 5.0/9.0;
}

float KelToCel(float K) {
    return K - 273.15;
}

float KelToFarh(float K) {
    return K * 9.0/5.0 - 459.67;
}