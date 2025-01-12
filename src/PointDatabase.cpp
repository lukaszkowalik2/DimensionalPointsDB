#include "PointDatabase.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

// Definicja statycznej zmiennej (nazwa katalogu na dane)
const std::string PointDatabase::DATA_DIR = "datapoints";

PointDatabase::PointDatabase(size_t dims)
    : dimensions(dims) {
  // Tworzymy katalog, jeśli nie istnieje
  ensureDirectoryExists();

  // Upewniamy się, że plik istnieje (lub go tworzymy)
  std::string filepath = getFilePath();
  std::ofstream file(filepath, std::ios::app);
  if (!file) {
    throw std::runtime_error("Cannot create file: " + filepath);
  }
  file.close();

  // Wczytujemy istniejące punkty z pliku
  loadFromFile();
}

void PointDatabase::ensureDirectoryExists() const {
  // Tworzy katalog, jeśli nie istnieje (C++17 <filesystem>)
  std::filesystem::create_directory(DATA_DIR);
}

std::string PointDatabase::getFilePath() const {
  // Nazwa pliku zależna od liczby wymiarów, np. datapoints/points_3d.txt
  return DATA_DIR + "/points_" + std::to_string(dimensions) + "d.txt";
}

void PointDatabase::addPoint(const Point& p) {
  points.push_back(p);
  saveToFile();  // zapis do pliku po dodaniu
}

Point PointDatabase::findNearest(const Point& target) const {
  if (points.empty()) {
    throw std::runtime_error("Database is empty");
  }

  size_t nearestIndex = 0;
  double minDist = points[0].distanceTo(target);

  // Szukamy punktu o najmniejszej odległości Euklidesowej
  for (size_t i = 1; i < points.size(); ++i) {
    double dist = points[i].distanceTo(target);
    if (dist < minDist) {
      minDist = dist;
      nearestIndex = i;
    }
  }
  return points[nearestIndex];
}

std::string PointDatabase::getAllPoints() const {
  std::stringstream ss;
  for (size_t i = 0; i < points.size(); ++i) {
    ss << (i + 1) << ". " << points[i].toString() << "\n";
  }
  return ss.str();
}

size_t PointDatabase::getDimensions() const {
  return dimensions;
}

void PointDatabase::loadFromFile() {
  std::ifstream file(getFilePath());
  if (!file) {
    std::cout << "Could not open file: " << getFilePath() << std::endl;
    return;
  }

  std::cout << "Loading points from file: " << getFilePath() << std::endl;
  points.clear();

  std::string line;
  while (std::getline(file, line)) {
    // Zakładamy, że każda linia zawiera wszystkie współrzędne rozdzielone spacją
    try {
      std::stringstream ss(line);
      std::vector<double> point_coords;
      double coord;

      while (ss >> coord) {
        point_coords.push_back(coord);
      }

      if (point_coords.size() == dimensions) {
        points.emplace_back(point_coords);
        std::cout << "Added point: "
                  << Point(point_coords).toString() << std::endl;
      } else {
        std::cout << "Wrong number of dimensions in point: "
                  << point_coords.size() << " (expected " << dimensions
                  << ")" << std::endl;
      }
    } catch (const std::exception& e) {
      std::cout << "Error parsing line: " << line
                << " - " << e.what() << std::endl;
      continue;
    }
  }
}

void PointDatabase::saveToFile() const {
  std::ofstream file(getFilePath());
  if (!file) {
    throw std::runtime_error("Cannot open file for writing: " + getFilePath());
  }

  for (const auto& point : points) {
    const auto& coords = point.getCoordinates();
    for (size_t i = 0; i < coords.size(); ++i) {
      if (i > 0) {
        file << " ";
      }
      file << coords[i];
    }
    file << "\n";
  }
}

void PointDatabase::editPoint(size_t index, const Point& newPoint) {
  if (index >= points.size()) {
    throw std::out_of_range("Point index out of range");
  }
  points[index] = newPoint;
  saveToFile();
}

void PointDatabase::deletePoint(size_t index) {
  if (index >= points.size()) {
    throw std::out_of_range("Point index out of range");
  }
  points.erase(points.begin() + index);
  saveToFile();
}

size_t PointDatabase::getPointCount() const {
  return points.size();
}

Point PointDatabase::getPoint(size_t index) const {
  if (index >= points.size()) {
    throw std::out_of_range("Point index out of range");
  }
  return points[index];
}
