#ifndef POINT_HPP
#define POINT_HPP

#include <string>
#include <vector>

class Point {
 public:
  explicit Point(const std::vector<double>& coords);

  double distanceTo(const Point& other) const;

  std::string toString() const;

  const std::vector<double>& getCoordinates() const;

 private:
  std::vector<double> coords;
};

#endif
