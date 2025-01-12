#ifndef POINTDATABASE_HPP
#define POINTDATABASE_HPP

#include <cstddef>  // size_t
#include <string>
#include <vector>

#include "Point.hpp"

class PointDatabase {
 public:
  explicit PointDatabase(size_t dims);
  void addPoint(const Point& p);
  Point findNearest(const Point& target) const;
  std::string getAllPoints() const;
  size_t getDimensions() const;

 private:
  void loadFromFile();
  void saveToFile() const;
  void ensureDirectoryExists() const;
  std::string getFilePath() const;
  static const std::string DATA_DIR;
  std::vector<Point> points;
  size_t dimensions;
};

#endif
