#include "Point.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>

Point::Point(const std::vector<double>& coords)
    : coords(coords) {
}

double Point::distanceTo(const Point& other) const {
  if (coords.size() != other.coords.size()) {
    throw std::runtime_error("Points must have the same number of dimensions.");
  }

  double sum = 0.0;
  for (size_t i = 0; i < coords.size(); ++i) {
    double diff = coords[i] - other.coords[i];
    sum += diff * diff;
  }
  return std::sqrt(sum);
}

std::string Point::toString() const {
  std::stringstream ss;
  ss << "(";
  for (size_t i = 0; i < coords.size(); ++i) {
    ss << coords[i];
    if (i < coords.size() - 1) {
      ss << ",";
    }
  }
  ss << ")";
  return ss.str();
}

const std::vector<double>& Point::getCoordinates() const {
  return coords;
}
