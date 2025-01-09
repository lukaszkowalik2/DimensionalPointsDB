#include "Point.hpp"

#include <cmath>
#include <sstream>

Point::Point(const std::vector<double>& coords)
    : coordinates(coords) {
}

double Point::distanceTo(const Point& other) const {
  double sum = 0.0;
  for (size_t i = 0; i < coordinates.size(); ++i) {
    double diff = coordinates[i] - other.coordinates[i];
    sum += diff * diff;
  }
  return std::sqrt(sum);
}

std::string Point::toString() const {
  std::stringstream ss;
  ss << "(";
  for (size_t i = 0; i < coordinates.size(); ++i) {
    if (i > 0) ss << ", ";
    ss << coordinates[i];
  }
  ss << ")";
  return ss.str();
}

const std::vector<double>& Point::getCoordinates() const {
  return coordinates;
}