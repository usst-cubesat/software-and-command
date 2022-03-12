/**
 * @file main.c
 */

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <hal/Timing/WatchDogTimer.h>
#include <hal/Timing/Time.h>
#include <hal/Drivers/LED.h>

#include <hal/boolean.h>
#include <hal/Utility/util.h>
#include <hal/version/version.h>

#include <at91/peripherals/cp15/cp15.h>
#include <at91/utility/exithandler.h>
#include <at91/commons.h>
#include <stdlib.h>

#include <RDebug.h>
#include <RFram.h>
#include <RI2c.h>
#include <RUart.h>

#include <RTransceiver.h>
#include <RCommon.h>

#include <RCommunicationTasks.h>
#include <RDosimeterCollectionTask.h>
#include <RImageCaptureTask.h>
#include <RAdcsCaptureTask.h>
#include <RTelemetryCollectionTask.h>
#include <RSatelliteWatchdogTask.h>


/***************************************************************************************************
                                   DEFINITIONS AND PRIVATE GLOBALS
***************************************************************************************************/

/** How often the internal OBC Watchdog is kicked (i.e. pet, i.e. reset) in ms. */
#define OBC_WDOG_KICK_PERIOD_MS	(15 / portTICK_RATE_MS)

/** Default stack size (in bytes) allotted to each FreeRTOS Task. */
#define DEFAULT_TASK_STACK_SIZE	(4096)

/** FreeRTOS Task Handles. */
static xTaskHandle missionInitTaskHandle;
static xTaskHandle communicationRxTaskHandle;
static xTaskHandle communicationTxTaskHandle;
static xTaskHandle dosimeterCollectionTaskHandle;
static xTaskHandle imageCaptureTaskHandle;
static xTaskHandle adcsCaptureTaskHandle;
static xTaskHandle telemetryCollectionTaskHandle;
static xTaskHandle satelliteWatchdogTaskHandle;

/** Communication Transmit Task Priority. Downlinks messages when necessary; very high priority task. */
static const int communicationTxTaskPriority = configMAX_PRIORITIES - 1;
/** Communication Receive Task Priority. Constantly listening for messages; high priority task. */
static const int communicationRxTaskPriority = configMAX_PRIORITIES - 2;

/** Dosimeter Collection Task Priority. Periodically collects payload data; medium priority task. */
static const int dosimeterCollectionTaskPriority = configMAX_PRIORITIES - 3;
/** Image Capture Task Priority. Periodically collects image data; medium priority task. */
static const int imageCaptureTaskPriority = configMAX_PRIORITIES - 3;
/** ADCS Capture Task Priority. Periodically collects ADCS data; medium priority task. */
static const int adcsCaptureTaskPriority = configMAX_PRIORITIES - 3;

/** Telemetry Collection Task Priority. Periodically collects satellite telemetry; low priority task. */
static const int telemetryCollectionTaskPriority = configMAX_PRIORITIES - 4;
/** Satellite Watchdog Task Priority. Routinely pets (resets) satellite subsystem watchdogs; low priority task. */
static const int satelliteWatchdogTaskPriority = configMAX_PRIORITIES - 4;
/** Mission Init Task Priority. Does initializations that need to be ran post-scheduler; low priority task. */
static const int missionInitTaskPriority = configMAX_PRIORITIES - 4;


/***************************************************************************************************
                                       PRIVATE FUNCTION STUBS
***************************************************************************************************/

static void initBoard(void);
static int initDrivers(void);
static int initTime(void);
static int initSubsystems(void);

static int initObcWatchdog(void);
static int initMissionTasks(void);

void MissionInitTask(void* parameters);


/***************************************************************************************************
                                                MAIN
***************************************************************************************************/

/**
 * The main application entry point.
 * @return Never returns.
 */
int main(void) {

	// initialize internal OBC board settings
	initBoard();

	// initialize the Hardware Abstraction Library (HAL) drivers
	initDrivers();

	// initialize external components and the Satellite Subsystem Interface (SSI)
	initSubsystems();

	// initialize the internal OBC watchdog, and start a task that automatically pets it
	initObcWatchdog();

#ifdef TEST

	// TODO: run tests

#else	/* TEST */

	// TODO: Antenna Diagnostic & Deployment (if necessary)

	// TODO: Satellite Diagnostic Check (if applicable - may be done later instead)

	// initialize the Mission Initialization Task
	int error = xTaskCreate(MissionInitTask,
							(const signed char*)"Mission Initialization Task",
							DEFAULT_TASK_STACK_SIZE,
							NULL,
							missionInitTaskPriority,
							&missionInitTaskHandle);

	if (error != pdPASS)
		debugPrint("main(): failed to create MissionInitTask.\n");

#endif	/* TEST */

	// start the FreeRTOS Scheduler - NEVER GETS PAST THIS LINE
	vTaskStartScheduler();

	// this function should never get here, nevertheless, please make sure that this last call doesn't get optimized away
	exit(0);
}


/***************************************************************************************************
                                         PRIVATE FUNCTIONS
***************************************************************************************************/

/**
 * Initialize low-level MCU/OBC configuration settings.
 */
static void initBoard(void) {

	// Enable the Instruction cache of the ARM9 core. Keep the MMU and Data Cache disabled.
	CP15_Enable_I_Cache();

}


/**
 * Initialize the low-level peripheral drivers used on the OBC.
 */
static int initDrivers(void) {

	int error = SUCCESS;

	// initialize the Debug UART port for debugging to a PC
	error = uartInit(UART_DEBUG_BUS);
	if (error != SUCCESS)
		debugPrint("initDriver(): failed to initialize Debug UART.\n");

	// initialize the Auxillary Camera UART port for communication with Camera
	error = uartInit(UART_CAMERA_BUS);
	if (error != SUCCESS)
		debugPrint("initDriver(): failed to initialize Camera UART.\n");

	// initialize the FRAM memory module for safe persistent storage
	error = framInit();
	if (error != SUCCESS)
		debugPrint("initDriver(): failed to initialize FRAM.\n");

	// initialize the I2C bus for general inter-component communication
	error = i2cInit();
	if (error != SUCCESS)
		debugPrint("initDriver(): failed to initialize I2C.\n");

	return error;
}


/**
 * Initialize the RTC and RTT, setting the default time in the process.
 */
static int initTime(void) {

	int error = SUCCESS;

	// create Time struct with default times (estimated Launch date)
	Time time = { 0 };
	time.year = 22;		// 2022
	time.month = 8;		// August
	time.date = 1;		// the 1st
	time.day = 2;		// Monday
	time.hours = 12;	// 12:00:00
	time.minutes = 0;	// 12:00:00
	time.seconds = 0;	// 12:00:00

	// the time (in seconds) between RTC and RTT synchronizations
	const unsigned int syncInterval = 120;

	// initilize the RTC and RTT and set the default time
	error = Time_start(&time, syncInterval);
	if (error != SUCCESS) {
		// TODO: report to system manager
		debugPrint("initTime(): failed to initialize RTC and RTT.\n");
	}

	return error;
}


/**
 * Initialize external subsystem modules and the Satellite Subsystem Interface library.
 */
static int initSubsystems(void) {

	int error = SUCCESS;

	// initialize the Transceiver module
	error = transceiverInit();
	if (error != SUCCESS)
		debugPrint("initSubsystems(): failed to initialize Transceiver subsystem.\n");

	return error;
}


/**
 * Initialize a low-priority task that automatically resets the OBC's internal Watchdog.
 *
 * On all builds except for Release, this will also enable an LED that blink every time that the
 * Watchdog timer is reset, allowing for a simple heartbeat to show proper OBC operation.
 *
 * @note This also initializes the Watchdog API.
 */
static int initObcWatchdog(void) {

	int error = SUCCESS;

#ifdef RELEASE
	error = WDT_startWatchdogKickTask(OBC_WDOG_KICK_PERIOD_MS, FALSE);
#else
	error = WDT_startWatchdogKickTask(OBC_WDOG_KICK_PERIOD_MS, TRUE);
#endif

	if (error != SUCCESS) {
		// TODO: report to system manager
		debugPrint("initObcWatchdog(): failed to start background OBC WDOG task.\n");
	}

	return error;
}


/**
 * Initialize all of the FreeRTOS tasks used during typical mission operation.
 */
static int initMissionTasks(void) {

	int error = pdPASS;

	// initialize the Communication Receive Task
	error = xTaskCreate(CommunicationRxTask,
						(const signed char*)"Communication Receive Task",
						DEFAULT_TASK_STACK_SIZE,
						NULL,
						communicationRxTaskPriority,
						&communicationRxTaskHandle);

	if (error != pdPASS)
		debugPrint("initMissionTasks(): failed to create CommunicationRxTask.\n");

	// initialize the Communication Transmit Task
	error = xTaskCreate(CommunicationTxTask,
						(const signed char*)"Communication Transmit Task",
						DEFAULT_TASK_STACK_SIZE,
						NULL,
						communicationTxTaskPriority,
						&communicationTxTaskHandle);

	if (error != pdPASS)
		debugPrint("initMissionTasks(): failed to create CommunicationTxTask.\n");

	// initialize the Dosimeter Collection Task
	error = xTaskCreate(DosimeterCollectionTask,
						(const signed char*)"Dosimeter Collection Task",
						DEFAULT_TASK_STACK_SIZE,
						NULL,
						dosimeterCollectionTaskPriority,
						&dosimeterCollectionTaskHandle);

	if (error != pdPASS)
		debugPrint("initMissionTasks(): failed to create DosimeterCollectionTask.\n");

	// initialize the Image Capture Task
	error = xTaskCreate(ImageCaptureTask,
						(const signed char*)"Image Capture Task",
						DEFAULT_TASK_STACK_SIZE,
						NULL,
						imageCaptureTaskPriority,
						&imageCaptureTaskHandle);

	if (error != pdPASS)
		debugPrint("initMissionTasks(): failed to create ImageCaptureTask.\n");

	// initialize the Image Capture Task
	error = xTaskCreate(AdcsCaptureTask,
						(const signed char*)"ADCS Capture Task",
						DEFAULT_TASK_STACK_SIZE,
						NULL,
						adcsCaptureTaskPriority,
						&adcsCaptureTaskHandle);

	if (error != pdPASS)
		debugPrint("initMissionTasks(): failed to create AdcsCaptureTask.\n");

	// initialize the Telemetry Collection Task
	error = xTaskCreate(TelemetryCollectionTask,
						(const signed char*)"Telemetry Collection Task",
						DEFAULT_TASK_STACK_SIZE,
						NULL,
						telemetryCollectionTaskPriority,
						&telemetryCollectionTaskHandle);

	if (error != pdPASS)
		debugPrint("initMissionTasks(): failed to create TelemetryCollectionTask.\n");

	// initialize the Satellite Watchdog Task
	error = xTaskCreate(SatelliteWatchdogTask,
						(const signed char*)"Satellite Watchdog Task",
						DEFAULT_TASK_STACK_SIZE,
						NULL,
						satelliteWatchdogTaskPriority,
						&satelliteWatchdogTaskHandle);

	if (error != pdPASS)
		debugPrint("initMissionTasks(): failed to create SatelliteWatchdogTask.\n");

	if (error == pdPASS)
		return SUCCESS;

	return error;
}


/***************************************************************************************************
                                           FREERTOS TASKS
***************************************************************************************************/

/**
 * Initialize FreeRTOS Tasks and other mission related modules.
 *
 * This functionality was placed into a FreeRTOS Task as some of the functionality and
 * initializations require the FreeRTOS Scheduler
 *
 * @param parameters
 */
void MissionInitTask(void* parameters) {

	// ignore the input parameter
	(void)parameters;

	// initialize the RTC and RTT to the default time
	initTime();

	// initialize the FreeRTOS Tasks used for typical mission operation
	initMissionTasks();

	// let this task delete itself
	vTaskDelete(NULL);
}
