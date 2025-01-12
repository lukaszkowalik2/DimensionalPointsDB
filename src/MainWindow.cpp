#include "MainWindow.hpp"

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>

#include <iostream>
#include <sstream>
#include <stdexcept>

MainWindow::MainWindow()
    : isInitialized(false), db(nullptr), mainGroup(nullptr) {
  window = new Fl_Window(800, 600, "DimensionalPointsDB");
  showInitialSetup();
  window->end();
}

void MainWindow::showInitialSetup() {
  setupGroup = new Fl_Group(0, 0, 800, 600);

  Fl_Box* titleBox = new Fl_Box(20, 20, 760, 40, "DimensionalPointsDB");
  titleBox->box(FL_UP_BOX);
  titleBox->labelfont(FL_BOLD);
  titleBox->labelsize(24);

  Fl_Box* dimLabel = new Fl_Box(200, 200, 400, 30, "Enter number of dimensions:");

  dimInput = new Fl_Int_Input(350, 250, 100, 30, "");
  dimInput->value("3");

  Fl_Button* startButton = new Fl_Button(350, 300, 100, 30, "Start");
  startButton->callback(start_cb, this);

  setupGroup->end();
}

void MainWindow::createMainInterface(int dimensions) {
  window->begin();

  if (setupGroup) {
    setupGroup->hide();
    window->remove(setupGroup);
  }

  mainGroup = new Fl_Group(0, 0, 800, 600);
  mainGroup->begin();

  window->label(("DimensionalPointsDB (" + std::to_string(dimensions) + "D)").c_str());

  Fl_Box* titleBox = new Fl_Box(20, 20, 760, 40, "DimensionalPointsDB");
  titleBox->box(FL_UP_BOX);
  titleBox->labelfont(FL_BOLD);
  titleBox->labelsize(24);

  pointInput = new Fl_Input(70, 80, 200, 25, "Point:");

  std::string defaultValue;
  for (size_t i = 0; i < db->getDimensions(); i++) {
    if (i > 0) defaultValue += ",";
    defaultValue += "0";
  }
  pointInput->value(("(" + defaultValue + ")").c_str());

  Fl_Button* addButton = new Fl_Button(280, 80, 70, 25, "Add");
  addButton->callback(add_cb, this);

  Fl_Button* findButton = new Fl_Button(360, 80, 100, 25, "Find Nearest");
  findButton->callback(find_cb, this);

  Fl_Button* changeDimsButton = new Fl_Button(470, 80, 140, 25, "Change Dimensions");
  changeDimsButton->callback(change_dims_cb, this);

  Fl_Button* editPointButton = new Fl_Button(620, 80, 70, 25, "Edit");
  editPointButton->callback(edit_point_cb, this);

  Fl_Button* deletePointButton = new Fl_Button(700, 80, 70, 25, "Delete");
  deletePointButton->callback(delete_point_cb, this);

  buffer = new Fl_Text_Buffer();
  display = new Fl_Text_Display(20, 120, 760, 460);
  display->buffer(buffer);
  display->textfont(FL_COURIER);

  mainGroup->end();
  window->end();
  window->redraw();
}

void MainWindow::start_cb(Fl_Widget*, void* v) {
  MainWindow* win = (MainWindow*)v;
  try {
    int dims = std::stoi(win->dimInput->value());
    if (dims < 1) {
      fl_alert("Dimensions must be positive!");
      return;
    }
    std::cout << "Number of dimensions: " << dims << std::endl;

    win->db = new PointDatabase(dims);

    win->createMainInterface(dims);
    win->isInitialized = true;
    win->updateDisplay();
    win->window->redraw();
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    fl_alert("Invalid dimension value!");
  }
}

void MainWindow::add_cb(Fl_Widget*, void* v) {
  MainWindow* win = (MainWindow*)v;
  if (!win->isInitialized) return;

  try {
    std::vector<double> coords = win->parsePointInput(win->pointInput->value());
    win->db->addPoint(Point(coords));

    win->updateDisplay();
    win->window->redraw();
    fl_message("Point added successfully!");
  } catch (const std::exception& e) {
    fl_alert("Error adding point: %s", e.what());
  }
}

void MainWindow::find_cb(Fl_Widget*, void* v) {
  MainWindow* win = (MainWindow*)v;
  if (!win->isInitialized) return;

  try {
    std::vector<double> coords = win->parsePointInput(win->pointInput->value());
    Point target(coords);

    Point nearest = win->db->findNearest(target);
    fl_message("Nearest point: %s", nearest.toString().c_str());
  } catch (const std::exception& e) {
    fl_alert("Error finding nearest point: %s", e.what());
  }
}

void MainWindow::change_dims_cb(Fl_Widget*, void* v) {
  MainWindow* win = (MainWindow*)v;
  if (!win->isInitialized) return;

  const char* value = fl_input("Enter new number of dimensions:", "3");
  if (!value) return;

  try {
    int dims = std::stoi(value);
    if (dims < 1) {
      fl_alert("Dimensions must be positive!");
      return;
    }

    delete win->db;
    win->db = new PointDatabase(dims);
    win->createMainInterface(dims);
    win->isInitialized = true;
    win->updateDisplay();
    win->window->redraw();
  } catch (const std::exception& e) {
    fl_alert("Invalid dimension value!");
  }
}

void MainWindow::edit_point_cb(Fl_Widget*, void* v) {
  MainWindow* win = (MainWindow*)v;
  if (!win->isInitialized) return;

  const char* index_str = fl_input("Enter point index to edit (starting from 1):", "1");
  if (!index_str) return;

  try {
    int index = std::stoi(index_str) - 1;
    if (index < 0 || index >= win->db->getPointCount()) {
      fl_alert("Invalid point index!");
      return;
    }

    Point point = win->db->getPoint(index);
    const char* new_value = fl_input("Enter new point coordinates:", point.toString().c_str());
    if (!new_value) return;

    std::vector<double> coords = win->parsePointInput(new_value);
    if (coords.size() != win->db->getDimensions()) {
      fl_alert("Wrong number of dimensions!");
      return;
    }

    win->db->editPoint(index, Point(coords));
    win->updateDisplay();
    win->window->redraw();
    fl_message("Point updated successfully!");
  } catch (const std::exception& e) {
    fl_alert("Error editing point: %s", e.what());
  }
}

void MainWindow::delete_point_cb(Fl_Widget*, void* v) {
  MainWindow* win = (MainWindow*)v;
  if (!win->isInitialized) return;

  const char* index_str = fl_input("Enter point index to delete (starting from 1):", "1");
  if (!index_str) return;

  try {
    int index = std::stoi(index_str) - 1;
    win->db->deletePoint(index);
    win->updateDisplay();
    win->window->redraw();
    fl_message("Point deleted successfully!");
  } catch (const std::exception& e) {
    fl_alert("Error deleting point: %s", e.what());
  }
}

void MainWindow::updateDisplay() {
  if (db && buffer) {
    buffer->text(db->getAllPoints().c_str());
  }
}

std::vector<double> MainWindow::parsePointInput(const std::string& input) const {
  std::vector<double> coords;

  if (input.empty() || input[0] != '(' || input[input.size() - 1] != ')') {
    throw std::runtime_error("Input must be in format (x,y,z)");
  }

  std::string cleaned = input.substr(1, input.size() - 2);
  std::stringstream ss(cleaned);
  std::string coord;

  while (std::getline(ss, coord, ',')) {
    try {
      coords.push_back(std::stod(coord));
    } catch (const std::exception&) {
      throw std::runtime_error("Invalid number format");
    }
  }

  if (coords.size() != db->getDimensions()) {
    throw std::runtime_error(
        "Expected " + std::to_string(db->getDimensions()) +
        " coordinates, but got " + std::to_string(coords.size()));
  }
  return coords;
}

void MainWindow::show() {
  window->show();
}
