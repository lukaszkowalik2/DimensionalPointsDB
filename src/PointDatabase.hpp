#pragma once
#include <string>
#include <vector>

#include "Point.hpp"

class PointDatabase {
 public:
  PointDatabase(const std::string& filename, size_t dimensions);
  void addPoint(const Point& p);
  Point findNearest(const Point& target) const;
  std::string getAllPoints() const;
  size_t getDimensions() const;

 private:
  void loadFromFile();
  void saveToFile() const;

  std::vector<Point> points;
  std::string filename;
  size_t dimensions;
};