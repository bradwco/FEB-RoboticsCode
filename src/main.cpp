#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h"
#include "../includeB/robot-config.hpp"
#include "../includeB/userControlTools.hpp"

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

void initialize() {
	pros::lcd::initialize();
	chassis.calibrate();
	pros::Task cycleScreen(updateScreen);
	pros::Task checkOverheat(tempMonitor);
}

void disabled() {
/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
}

void competition_initialize() {
/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
}

void autonomous() {
    chassis.setPose(54.698,16.516, 90);
    chassis.follow(GOAL_RUSH_txt, 15, 2000, false);
    chassis.follow(SCORING_txt, 15, 2000);
}

void opcontrol() {
    while (true) {
		int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

		//Reg or Invert Driving
		if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
			invertDriveState = !invertDriveState;
		}
		(!invertDriveState)? chassis.tank(leftY, rightY): chassis.tank(-rightY,-leftY);

		//Screen States
		if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)){
			screenState = static_cast<ScreenState>((static_cast<int>(screenState) + 1) % static_cast<int>(ScreenState::NUM_STATES)); // cycle between all 4 states dynamically
		}

		//Mechanism Functions
		//INTAKE
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
			intakeState = (intakeState == IntakeState::FORWARD) ? IntakeState::STOPPED : IntakeState::FORWARD; //switch between fwd & stop
		}
		if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
			intakeState = (intakeState == IntakeState::REVERSE) ? IntakeState::STOPPED : IntakeState::REVERSE; //switch between reverse & stop
        }
		updateIntake();

		//CLAMP
		solenoidClamp.set_value(!controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1));

		//SCORING
		//Nested ternary operators (IDK IF WORKING)
		//Lambda function syntax: [capture local vars (parameters) -> return_type { function body };
		//() excute lambda
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) ? chainMotor.move(chainVoltageFWD) :
		controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2) ? chainMotor.move(chainVoltageREV) :
		[]()->std::int32_t {return chainMotor.brake();}(); 

		pros::delay(25);
	}
}