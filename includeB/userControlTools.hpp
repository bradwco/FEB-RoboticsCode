#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h" // IWYU pragma: keep

//Global States/Classes
enum class IntakeState {FORWARD, REVERSE, STOPPED};
extern IntakeState intakeState;
enum class ScreenState {DRIVE_TEMPS, MECHS_TEMPS, DRIVE_CONNECTS, MECHS_CONNECTS, NUM_STATES}; //NUM_STATES auto detects states to be dynamic
extern ScreenState screenState;

//Mechanism Functions
extern void updateIntake();
extern void displayDriveTemps();
extern void displayMechsTemps();
extern void displayDriveConnects();
extern void displayMechsConnects();
extern void updateScreen(void); 
extern void tempMonitor(void);
