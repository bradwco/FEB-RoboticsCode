#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h" // IWYU pragma: keep
#include "pros/motors.hpp"

//PP Files
// Path Generation Link: https://path.jerryio.com/
ASSET(SCORING_txt);
ASSET(GOAL_RUSH_txt);

//PROS motors
extern pros::MotorGroup left_motor_group;
extern pros::MotorGroup right_motor_group;
extern pros::Motor intakeMotor;
extern pros::Motor chainMotor;
extern pros::adi::DigitalOut solenoidClamp;

//PROS odom
extern pros::Imu imu;
extern pros::Rotation vertical_encoderL;
extern pros::Rotation vertical_encoderR;
extern pros::Rotation horizontal_encoder;

//LL motors
extern lemlib::Drivetrain drivetrain;

//LL odom
extern lemlib::TrackingWheel horizontal_tracking_wheel;
extern lemlib::TrackingWheel vertical_tracking_wheelL;
extern lemlib::TrackingWheel vertical_tracking_wheelR;
extern lemlib::OdomSensors sensors;

//LL PID Settings & Chassis
extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;
extern lemlib::Chassis chassis;

//Controller
extern pros::Controller controller;

//CONSTANTS/VARS
extern const double xOffset;
extern const double yOffset;
extern const double tileSize;
extern const double intakeVoltage;
extern const double chainVoltageFWD;
extern const double chainVoltageREV;
extern const double overHeatTemp;
extern bool overheatWarningActive;
extern pros::Task cycleScreen;
extern pros::Task checkOverheat;
extern bool invertDriveState;