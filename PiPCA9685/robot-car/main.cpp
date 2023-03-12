#include <ncurses.h> // Include the ncurses library
#include <unistd.h>
#include <PiPCA9685/PCA9685.h>
#include <cmath>
#include <iostream>

double set_wheel_steering_angle(double d) {
  const double h = 5.4;
  const double k = 8.7;

  double rad_result = atan((d - h)/k);

  return rad_result * 180.0 / M_PI;
}

int set_servo_angle(const double angle) {
  const int min_angle = 0;
  const int max_angle = 270;
  const int min_off_time = 750;
  const int max_off_time = 3270;

  // Calculate the off time value corresponding to the desired angle
  return ((angle - min_angle) * (max_off_time - min_off_time) / (max_angle - min_angle)) + min_off_time;
}

double calculate_d(double angle, double h, double k) {
  return tan(angle) * h + k;
}

double calculate_angle1(double d, double h, double k) {
  return atan((d - h)/k) * 180.0 / M_PI;
}

double calculate_angle2(double d, double h, double k) {
  return atan((d + h)/k) * 180.0 / M_PI;
}

void set_servos(PiPCA9685::PCA9685 &pca, double wheel1, double wheel2, double wheel3, double wheel4) {
  pca.set_pwm(0, 0, set_servo_angle(wheel1));
  pca.set_pwm(1, 0, set_servo_angle(wheel2));
  pca.set_pwm(2, 0, set_servo_angle(wheel3));
  pca.set_pwm(3, 0, set_servo_angle(wheel4));
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

  double innerWheelAngle = 45;

  while (true) {
    // Read user input from the keyboard
    int ch = getch();
    switch (ch) {
      case KEY_LEFT:
        // Move the slider left
        if (innerWheelAngle > -90) {
          innerWheelAngle -= 5;
        }
        break;
      case KEY_RIGHT:
        // Move the slider right
        if (innerWheelAngle < 90) {
          innerWheelAngle += 5;
        }
        break;
      default:
        break;
    }

    // Calculate the steering angle based on the slider position
    //double angle = set_steering_angle(slider_pos);
    double result = tan(abs(innerWheelAngle) * M_PI / 180.0) * k + h;

    // Calculate the wheel angles based on the steering angle
    double result1 = calculate_angle1(result, h, k);
    double result2 = calculate_angle2(result, h, k);

    if(innerWheelAngle > 0) {
      set_servos(pca, result1, 270-(180 - result2), 180-result2, 270-result1);
    }
    else {
      set_servos(pca, 180-result2, 270-result1, result1, 270-(180-result2));
    }
  }

  // Clean up ncurses
  endwin();

  return 0;
}