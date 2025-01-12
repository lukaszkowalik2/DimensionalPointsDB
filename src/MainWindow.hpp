#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Window.H>

#include "PointDatabase.hpp"

class MainWindow {
 public:
  MainWindow();
  void show();

 private:
  Fl_Window* window;
  Fl_Group* setupGroup;
  Fl_Group* mainGroup;

  Fl_Int_Input* dimInput;
  Fl_Input* pointInput;
  Fl_Text_Buffer* buffer;
  Fl_Text_Display* display;

  bool isInitialized;
  PointDatabase* db;

  void showInitialSetup();
  void createMainInterface(int dimensions);
  void updateDisplay();

  static void start_cb(Fl_Widget*, void* v);
  static void add_cb(Fl_Widget*, void* v);
  static void find_cb(Fl_Widget*, void* v);
  static void change_dims_cb(Fl_Widget*, void* v);

  std::vector<double> parsePointInput(const std::string& input) const;
};

#endif
