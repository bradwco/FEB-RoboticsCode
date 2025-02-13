#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h" // IWYU pragma: keep
#include "../includeB/robot-config.hpp"

pros::MotorGroup left_motor_group({-1,2,-3,4}, pros::MotorGearset::blue);
pros::MotorGroup right_motor_group({5,-6,7,-8}, pros::MotorGearset::blue);
pros::Motor intakeMotor(-11, pros::MotorGearset::blue);
pros::Motor chainMotor(-12, pros::MotorGearset::blue);
pros::adi::DigitalOut solenoidClamp('A');

pros::Imu imu(10);
pros::Rotation vertical_encoderL(-20);
pros::Rotation vertical_encoderR(18);
pros::Rotation horizontal_encoder(-19);

lemlib::Drivetrain drivetrain(&left_motor_group,
    &right_motor_group, 
    12.375, 
    lemlib::Omniwheel::NEW_325, 
    600, 
    2
);

lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_2, 0);
lemlib::TrackingWheel vertical_tracking_wheelL(&vertical_encoderL, lemlib::Omniwheel::NEW_2, -2.546);
lemlib::TrackingWheel vertical_tracking_wheelR(&vertical_encoderR, lemlib::Omniwheel::NEW_2, 2.546);

lemlib::OdomSensors sensors(
    &vertical_tracking_wheelR, 
    nullptr, //Put in left one in future (if fixed)
    &horizontal_tracking_wheel, 
    nullptr, 
    &imu 
);

lemlib::ControllerSettings lateral_controller(10,
                                              0, 
                                              5, 
                                              3, // anti windup
                                              1, // in
                                              100, // ms
                                              3, // in
                                              500, // ms
                                              20 // max accel
);

lemlib::ControllerSettings angular_controller(2,
                                              0,
                                              12,
                                              0, // anti windup
                                              1, // in
                                              100, // ms
                                              3, // in
                                              500, // ms
                                              0 // max accel
);

lemlib::Chassis chassis(drivetrain, 
    lateral_controller, 
    angular_controller, 
    sensors
);
pros::Controller controller(pros::E_CONTROLLER_MASTER);

const double xOffset = 7.25;
const double yOffset = 15.0/2;
const double tileSize = 24;
const double intakeVoltage = 217;
const double chainVoltageFWD = 217;
const double chainVoltageREV = -217;
const double overHeatTemp = 130; //Farhenheit
bool overheatWarningActive = false; //used to disable the updateScreen
bool invertDriveState = false;