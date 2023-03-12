#include <ncurses.h> // Include the ncurses library
#include <unistd.h>
#include <PiPCA9685/PCA9685.h>
#include <cmath>
#include <iostream>

// Function to set the steering angle based on the slider position
double set_steering_angle(int slider_pos) {
  // Calculate the steering angle based on the slider position
  const double min_angle = -45.0;
  const double max_angle = 45.0;
  double angle = min_angle + (max_angle - min_angle) * (double)slider_pos / 100.0;
  return angle;
}

int main() {
  
  PiPCA9685::PCA9685 pca{};
  setvbuf(stdout, NULL, _IONBF, 0);

  const double h = 5.4;
  const double k = 8.7;

  double d = 7;

  pca.set_pwm_freq(310.0);

  // Initialize ncurses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  // Print the slider control
  printw("Use arrow keys to adjust the steering angle:\n");
  printw("[                                                  ]\n");

  // Set the initial slider position to the middle
  int slider_pos = 50;
  int slider_width = 50;
  int slider_start_x = 1;
  int slider_end_x = slider_start_x + slider_width;
  int slider_y = 1;
  mvprintw(slider_y, slider_start_x + slider_pos, "*");
  refresh();

  while (true) {
    // Read user input from the keyboard
    int ch = getch();
    switch (ch) {
      case KEY_LEFT:
        // Move the slider left
        if (slider_pos > 0) {
          mvprintw(slider_y, slider_start_x + slider_pos, " ");
          slider_pos--;
          mvprintw(slider_y, slider_start_x + slider_pos, "*");
          refresh();
        }
        break;
      case KEY_RIGHT:
        // Move the slider right
        if (slider_pos < slider_width) {
          mvprintw(slider_y, slider_start_x + slider_pos, " ");
          slider_pos++;
          mvprintw(slider_y, slider_start_x + slider_pos, "*");
          refresh();
        }
        break;
      default:
        break;
    }

    // Calculate the steering angle based on the slider position
    double angle = set_steering_angle(slider_pos);

    // Calculate the wheel angles based on the steering angle
    double result1 = calculate_angle1(angle, h, k);
    double result2 = calculate_angle2(angle, h, k);
    set_servos(pca, result1, 270-(180 - result1), 180-result2, 270-result1);

    // Sleep for a short period of time to avoid updating the servos too quickly
    usleep(100000);
  }

  // Clean up ncurses
  endwin();

  return 0;
}