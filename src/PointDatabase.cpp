#include "PointDatabase.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

PointDatabase::PointDatabase(const std::string& fname, size_t dims)
    : filename(fname), dimensions(dims) {
  loadFromFile();
}

void PointDatabase::addPoint(const Point& p) {
  points.push_back(p);
  saveToFile();
}

Point PointDatabase::findNearest(const Point& target) const {
  if (points.empty())
    throw std::runtime_error("Database is empty");

  size_t nearest = 0;
  double minDist = points[0].distanceTo(target);

  for (size_t i = 1; i < points.size(); ++i) {
    double dist = points[i].distanceTo(target);
    if (dist < minDist) {
      minDist = dist;
      nearest = i;
    }
  }

  return points[nearest];
}

std::string PointDatabase::getAllPoints() const {
  std::stringstream ss;
  for (const auto& p : points) {
    ss << p.toString() << "\n";
  }
  return ss.str();
}

size_t PointDatabase::getDimensions() const {
  return dimensions;
}

void PointDatabase::loadFromFile() {
  std::ifstream file(filename);
  if (!file) return;

  points.clear();
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::vector<double> coords;
    double coord;

    while (ss >> coord) {
      coords.push_back(coord);
    }

    if (coords.size() == dimensions) {
      points.emplace_back(coords);
    }
  }
}

void PointDatabase::saveToFile() const {
  std::ofstream file(filename);
  if (!file)
    throw std::runtime_error("Cannot open file for writing: " + filename);

  for (const auto& point : points) {
    const auto& coords = point.getCoordinates();
    for (size_t i = 0; i < coords.size(); ++i) {
      if (i > 0) file << " ";
      file << coords[i];
    }
    file << "\n";
  }
}
