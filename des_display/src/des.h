/*
 * des.h
 * Referencing dis-mvah:
 *
 *  An example H file that you can use as a model for defining enums for the inputs, outputs, and state names.
 *  This approach will make your code more readable :)
  * Author: Aliab Eman (041000420) and Savas Erturk (040919022)
 */

#ifndef DES_H_
#define DES_H_

#define NUM_STATES 14 //14 states in our Finite State Machine
#define NUM_INPUTS 13 //13 inputs
#define NUM_OUTPUTS 14

#define RUNNING  1

#define LEFT 0
#define RIGHT 1
#define DEFAULT 2


typedef struct {
	int id;
	int weight;
	int state;
} Person;

struct ctrl_response {
	Person person;
	int statusCode;
	char errMsg[128];
}typedef controller_response_struct;

typedef void *(*FState)();

typedef enum {
	ST_START = 0,
	ST_LS = 1,
	ST_GLU = 2,
	ST_LO = 3,
	ST_WS1 = 4,
	ST_LC = 5,
	ST_GLL = 6,
	ST_RS = 7,
	ST_GRU = 8,
	ST_RO = 9,
	ST_WS2 = 10,
	ST_RC = 11,
	ST_GRL = 12,
	ST_EXIT = 13
} State;

typedef enum {
	IN_LS = 0,
	IN_GLU = 1,
	IN_LO = 2,
	IN_WSL = 3,
	IN_LC = 4,
	IN_GLL = 5,
	IN_RS = 6,
	IN_GRU = 7,
	IN_RO = 8,
	IN_WSR = 9,
	IN_RC = 10,
	IN_GRL = 11,
	IN_EXIT = 12
} Input;

const char *inMessage[NUM_INPUTS] = {
		"ls",
		"glu",
		"lo",
		"wsl",
		"lc",
		"gll",
		"rs",
		"gru",
		"ro",
		"wsr",
		"rc",
		"grl",
		"exit"
};

typedef enum {
	OUTPUT_START = 0,
	OUTPUT_LS = 1,
	OUTPUT_GLU = 2,
	OUTPUT_LO = 3,
	OUTPUT_WS1 = 4,
	OUTPUT_LC = 5,
	OUTPUT_GLL = 6,
	OUTPUT_RS = 7,
	OUTPUT_GRU = 8,
	OUTPUT_RO = 9,
	OUTPUT_WS2 = 10,
	OUTPUT_RC = 11,
	OUTPUT_GRL = 12,
	OUTPUT_EXIT = 13
} Output;


const char *outMessage[NUM_OUTPUTS] = {
		"The controller is running as PID:", //0
		"Person: Scanning Left Door. ID = ", //1
		"Guard: Left door unlocked...", //2
		"Person: Opened left door",  //3
		"Person: Weight upon ENTRY = ", //4
		"Left door closed (automatically) ", //5
		"Guard: Left door locked...", //6
		"Person: Scanning Right Door... ID = ", //7
		"Guard: Right door unlocked...", //8
		"Person: Opened right door...", //9
		"Person: Weight upon EXIT = ", //10
		"Right door closed (automatically).", //11
		"Guard: Right door locked...", //12
		"Exiting Door Entry System..." //13
};


typedef enum {
	IN_ERR_CONN = 0,
	IN_ERR_SND = 1,
	IN_ERR_RSP = 2,
	IN_ERR_USG = 3,
	ERR_SRVR_MSG = 4,
	DP_ERR_CHANNEL_CREATE = 5,
	DP_ERR_RCV = 6,
	DP_ERR_RPLY = 7,
	CTRL_ERR_USG = 8,
	CTRL_ERR_CHANNEL_CREATE = 9,
	CTRL_ERR_CONN = 10,
	CTRL_ERR_RCV = 11,
	CTRL_ERR_SND = 12
} Error;

#define NUM_ERROR 13
const char *errorMessages[NUM_ERROR] = {
		"ERROR: Could not Connect to Controller ./des-input ", /* 0 */
		"ERROR: Sending Message ./des-input", /* 1 */
		"ERROR: NULL Response from server ./des-input", /*2*/
		"Missing PID to controller",/*3*/
		"GENERAL-SERVER-ERROR: ",/*4*/
		"ERROR: Unable to create Channel ./des-display",/*5*/
		"ERROR: Message not Received ./des-display",/*6*/
		"ERROR: Replying to Message ./des-display",/*7*/
		"Missing PID to display",
		"ERROR: Unable to Create Channel ./des-controller",/*9*/
		"ERROR: Could not connect to des-display ./des-controller",/*10*/
		"ERROR: Message not received ./des-controller",/*11*/
		"ERROR: Could not send Message ./des-controller STATE: ", /*12*/
};

#endif /* DES_H_ */
