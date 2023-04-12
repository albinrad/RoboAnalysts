#include <iostream>
#include <cmath>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <Eigen/Dense>
#include <chrono>

#define double alpha = const double pi / 4
#define double beta = 0

#define PI = const double pi

using namespace Eigen;

//Determine Jacobian based on alpha and beta
Matrix3d CalculateInvertedJacobian(double alpha, double beta)
  {
    Matrix3d J;
    J << 2 * sin(alpha_ + beta_), 2 * sin(alpha_ - beta_), 0,
         -2 * cos(alpha_ + beta_), 2 * cos(alpha_ - beta_), 0,
         0, 0, 4;
    Matrix3d J_inv = J.inverse();
    
    // Check if the Jacobian matrix is invertible
    if (J_inv.determinant() < 1e-6)
    {
      std::cerr << "Error: Jacobian matrix is not invertible." << std::endl;
      return Matrix3d.inverse()::Zero();
    }

    else
    {
        return J_inv;
    }
  }

  Vector3d calculate_derivative (Vector3d& position_current)
  {
    //adjust time delta based on input sampling rate
    double dt = 0.001;
    Vector3d q_initial = Vector3d::Zero();

    FiniteDifference fd(FiniteDifference::forward, dt);

    return fd(position_current);

  }


  //Compute Required Control Torque Vector
  Vector3d CalculateTorqueControl (double KP, double KD, const Matrix3d& inverted_jacobian, const Vector3d& position_desired, Vector3d& position_current, Vector3d& position_derivative) 
  {
    Vector3d torque = inverted_jacobian * (KP * (position_desired - position_current) - KD * position_derivative);
    return torque;
  }

//Determine BLDC Torque based on kinematic symmetry
Vector4d BLDC_Torque (Vector3d torque)
{
    double front_right = torque (0);
    double back_left = - torque (0);
    double front_left = torque (1);
    double back_right = - torque(1);

    //servo torque applied equally to all wheels
    double servo = torque (2);

    Vector4d applied_torque;
    applied_torque << front_right, back_left, front_left, back_right;
    return applied_torque;
}

//control loop that will be called by main 
int PDControl()
{
    //KP and KD need to be tuned as 3x3 gain matrices in simulation or with the real implementation
    double KP = 1;
    double KD = 0.5;

    //desired base orientation
    Vector3d position_desired;
    position_desired << PI / 4, PI / 4, 0;

    //connect this to IMU inputs; could also pass as IMU as arguments to PDControl
    Vector3d position_current; 
    position_current = get_IMU_attitude();

    Vector3d position_derivative;
    position_derivative = calculate_derivative(position_current);

    Matrix3d inverted_jacobian = CalculateInvertedJacobian(alpha, beta);

    //compute required torques to achieve desired attitude
    Vector3d torque_outputs;
    torque_outputs = CalculateTorqueControl (KP, KD, inverted_jacobian, position_desired, position_current, position_derivative);

    //translate torque vector into specific torques for BLDC and servos
    Vector4d applied_torques;
    applied_torques = BLDC_Torque(torque_outputs);

}


