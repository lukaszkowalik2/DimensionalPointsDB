#include "MainWindow.hpp"

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>

#include <sstream>

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

  Fl_Box* titleBox = new Fl_Box(20, 20, 760, 40,
                                (std::string("DimensionalPointsDB (") + std::to_string(dimensions) + "D)").c_str());
  titleBox->box(FL_UP_BOX);
  titleBox->labelfont(FL_BOLD);
  titleBox->labelsize(24);

  for (int i = 0; i < dimensions; ++i) {
    std::string label = "X" + std::to_string(i + 1) + ":";
    Fl_Input* input = new Fl_Input(20 + i * 80, 80, 70, 25, label.c_str());
    input->value("0.0");
    inputs.push_back(input);
  }

  Fl_Button* addButton = new Fl_Button(20 + dimensions * 80, 80, 70, 25, "Add");
  addButton->callback(add_cb, this);

  Fl_Button* findButton = new Fl_Button(100 + dimensions * 80, 80, 100, 25, "Find Nearest");
  findButton->callback(find_cb, this);

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
    win->db = new PointDatabase("points.txt", dims);
    win->createMainInterface(dims);
    win->isInitialized = true;
  } catch (const std::exception& e) {
    fl_alert("Invalid dimension value!");
  }
}

void MainWindow::add_cb(Fl_Widget*, void* v) {
  MainWindow* win = (MainWindow*)v;
  if (!win->isInitialized) return;

  try {
    std::vector<double> coords;
    for (auto input : win->inputs) {
      coords.push_back(std::stod(input->value()));
    }
    win->db->addPoint(Point(coords));
    win->updateDisplay();
    fl_message("Point added successfully!");
  } catch (const std::exception& e) {
    fl_alert("Error adding point: %s", e.what());
  }
}

void MainWindow::find_cb(Fl_Widget*, void* v) {
  MainWindow* win = (MainWindow*)v;
  if (!win->isInitialized) return;

  try {
    std::vector<double> coords;
    for (auto input : win->inputs) {
      coords.push_back(std::stod(input->value()));
    }
    Point target(coords);
    Point nearest = win->db->findNearest(target);
    fl_message("Nearest point: %s", nearest.toString().c_str());
  } catch (const std::exception& e) {
    fl_alert("Error finding nearest point: %s", e.what());
  }
}

void MainWindow::updateDisplay() {
  if (db && buffer) {
    buffer->text(db->getAllPoints().c_str());
  }
}

void MainWindow::show() {
  window->show();
}