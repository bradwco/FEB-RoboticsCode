#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h" // IWYU pragma: keep
#include "../includeB/robot-config.hpp"
#include "../includeB/userControlTools.hpp"

//Global States
IntakeState intakeState = IntakeState::STOPPED; //stop it by default
ScreenState screenState = ScreenState::DRIVE_TEMPS;

//Mechanism Functions
void updateIntake(){
    switch(intakeState)
    {
    case IntakeState::FORWARD:
        intakeMotor.move(intakeVoltage);
        break;
    case IntakeState::REVERSE:
        intakeMotor.move(-intakeVoltage);
        break;
    case IntakeState::STOPPED:
    default:
        intakeMotor.brake();
    }
}
void displayDriveTemps(){
    std::vector<double> tempsL = left_motor_group.get_temperature_all();
    std::vector<double> tempsR = right_motor_group.get_temperature_all();

    for (size_t i = 0; i < tempsL.size(); i++) {
        tempsL[i] = (tempsL[i] * 9 / 5) + 32;
    }
    
    for (size_t i = 0; i < tempsR.size(); i++) {
        tempsR[i] = (tempsR[i] * 9 / 5) + 32;
    }
    controller.print(1,3,"DRIVE TEMPS (F°)");
    controller.print(2, 1, "L: %.0f %.0f %.0f %.0f", tempsL[0], tempsL[1], tempsL[2], tempsL[3]);
    controller.print(3, 1, "R: %.0f %.0f %.0f %.0f", tempsR[0], tempsR[1], tempsR[2], tempsR[3]);
}
void displayMechsTemps(){
    double motorChainTemp = chainMotor.get_temperature();
    motorChainTemp = (motorChainTemp * 9 / 5) + 32;

    double motorIntakeTemp = intakeMotor.get_temperature();
    motorIntakeTemp = (motorIntakeTemp * 9 / 5) + 32;

    controller.print(1,3,"MECHS TEMPS (F°)");
    controller.print(2, 1, "Chain Temp: %.0f", motorChainTemp);
    controller.print(3, 1, "Intake Temp: %.0f", motorIntakeTemp);
}
void displayDriveConnects() {
    std::vector<int32_t> leftMotorStatus = left_motor_group.is_over_current_all();
    std::vector<int32_t> rightMotorStatus = right_motor_group.is_over_current_all();

    controller.print(1, 3, "DRIVE CONNECTS");

    if (leftMotorStatus.size() >= 4 && rightMotorStatus.size() >= 4) {
        controller.print(2, 1, "L: %d %d %d %d", leftMotorStatus[0], leftMotorStatus[1], leftMotorStatus[2], leftMotorStatus[3]);
        controller.print(3, 1, "R: %d %d %d %d", rightMotorStatus[0], rightMotorStatus[1], rightMotorStatus[2], rightMotorStatus[3]);
    } else {
        controller.print(2, 1, "Drive Connection Error!");
    }
}
void displayMechsConnects() {
    bool chainConnected = chainMotor.get_efficiency() > 0;
    bool intakeConnected = intakeMotor.get_efficiency() > 0;

    controller.print(1, 3, "MECHS CONNECTS");
    controller.print(2, 1, "Chain: %s", chainConnected ? "CONNECTED" : "DISCONNECTED");
    controller.print(3, 1, "Intake: %s", intakeConnected ? "CONNECTED" : "DISCONNECTED");
}
void updateScreen(void){
    while(true){
        if(overheatWarningActive){
            cycleScreen.remove();
        }
        pros::lcd::clear();

        switch(screenState)
        {
            case ScreenState::MECHS_TEMPS:
                displayMechsTemps();
                break;
            case ScreenState::DRIVE_CONNECTS:
                displayDriveConnects();
                break;
            case ScreenState::MECHS_CONNECTS:
                displayMechsConnects();
                break;
            case ScreenState::DRIVE_TEMPS:
            default:
                displayDriveTemps();
                break;
        }
        pros::delay(2000);
    }
}
void tempMonitor(void) {
    while (true) {
        std::vector<double> allTemps;
        bool overheatDetected = false;
        std::vector<std::string> warnings;  

        std::vector<double> leftTemps = left_motor_group.get_temperature_all();
        std::vector<double> rightTemps = right_motor_group.get_temperature_all();

        allTemps.insert(allTemps.end(), leftTemps.begin(), leftTemps.end());
        allTemps.insert(allTemps.end(), rightTemps.begin(), rightTemps.end());
        allTemps.push_back(intakeMotor.get_temperature());
        allTemps.push_back(chainMotor.get_temperature());

        for (size_t i = 0; i < allTemps.size(); i++) {
            allTemps[i] = (allTemps[i] * 9 / 5) + 32;
        }

        for (size_t i = 0; i < allTemps.size(); i++) {
            if (allTemps[i] >= overHeatTemp) {
                overheatDetected = true;

                if (i < 4) {
                    warnings.push_back("LEFT MOTOR(S) OVERHEATING");
                } else if (i < 8) {
                    warnings.push_back("RIGHT MOTOR(S) OVERHEATING");
                } else if (i == 8) {
                    warnings.push_back("INTAKE MOTOR OVERHEATING");
                } else if (i == 9) {
                    warnings.push_back("CHAIN MOTOR OVERHEATING");
                }
            }
        }

        overheatWarningActive = overheatDetected;
        if (overheatDetected) {
            controller.rumble("-.-.-");
            controller.clear();

            if (warnings.size() >= 3) {
                controller.print(2, 3, "3+ WARNINGS DETECTED");
            } else {
                if (warnings.size() > 0) controller.print(1, 3, warnings[0].c_str());
                if (warnings.size() > 1) controller.print(3, 3, warnings[1].c_str());
            }
        }

        pros::delay(5000);  
    }
}

