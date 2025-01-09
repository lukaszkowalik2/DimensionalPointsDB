#pragma once
#include <string>
#include <vector>

class Point {
 public:
  Point(const std::vector<double>& coords);

  double distanceTo(const Point& other) const;
  std::string toString() const;
  const std::vector<double>& getCoordinates() const;

 private:
  std::vector<double> coordinates;
};