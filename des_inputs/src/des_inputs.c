/**************************************************
 * File: des_inputs.c
 * Author: Aliab Eman (041000420) and Savas Erturk (040919022)
 **************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <unistd.h>
#include "../../des_display/src/des.h"

void printInfo() {
	printf("Enter the event type (ls = left scan\n rs = right scan\n wsl = weight scale left door\n wsr = weight scale right door\n lo = left open\n"
		"ro = right open\n lc = left closed\n rc = right closed\n gru = guard right unlock\n grl = guard right lock\n"
		"gll = guard left lock\n glu = guard left unlock\n exit = exit program)\n");
}

int main(int argc, char* argv[]) {

	// PHASE : 1 ==============================
	int TRUE = 1;
	int MAX_NUMBER_SECONDS = 5;
	pid_t controllerPid;
	int coid;
	Person person;
	controller_response_struct controller_response;

	int MAX_ARGC = 2;

	// If argc is not 2 arguments in cmd, print usage message and exit failure.
	if (argc != MAX_ARGC) {
		printf("%s\n", errorMessages[IN_ERR_USG]);
		exit(EXIT_FAILURE);
	}

	controllerPid = atoi(argv[1]); // Converts string to int.

	// Call ConnectAttach() to attach to controller's channel
	coid = ConnectAttach(ND_LOCAL_NODE, controllerPid, 1, _NTO_SIDE_CHANNEL, 0);
	// On Failure: print error message and EXIT_FAILURE
	if (coid == -1) {
		fprintf(stderr, "ERROR: Could not connect to the des_controller. Did you specify the right PID?\n");
		exit(EXIT_FAILURE);
	}

	person.state = ST_START;

	if (MsgSend(coid, &person, sizeof(person), &controller_response,
			sizeof(controller_response)) == -1) {
		printf("%s\n", errorMessages[IN_ERR_SND]);
		exit(EXIT_FAILURE);
	}
	// PHASE : 2 =========================
	while (TRUE) {
		char input[5];
		printInfo();
		scanf("%s", input);

		if (strncmp(input, inMessage[IN_LS], strlen(inMessage[IN_LS])) == 0) {
			person.state = ST_LS;
			printf("Enter the Person's ID:\n");
			scanf("%d", &person.id);
		} else if (strncmp(input, inMessage[IN_RS], strlen(inMessage[IN_RS]))
				== 0) {
			person.state = ST_RS;
			printf("Enter ID:\n");
			scanf("%d", &person.id);
		} else if (strncmp(input, inMessage[IN_WSL], strlen(inMessage[IN_WSL]))
				== 0) {
			person.state = ST_WS1;
			printf("Please Enter your Weight\n");
			scanf("%d", &person.weight);
		} else if (strncmp(input, inMessage[IN_WSR], strlen(inMessage[IN_WSR]))
				== 0) {
			person.state = ST_WS2;
			printf("Please Enter your Weight\n");
			scanf("%d", &person.weight);
		} else if (strncmp(input, inMessage[IN_LO], strlen(inMessage[IN_LO]))
				== 0)
			person.state = ST_LO;
		else if (strncmp(input, inMessage[IN_RO], strlen(inMessage[IN_RO]))
				== 0)
			person.state = ST_RO;
		else if (strncmp(input, inMessage[IN_LC], strlen(inMessage[IN_LC]))
				== 0)
			person.state = ST_LC;
		else if (strncmp(input, inMessage[IN_RC], strlen(inMessage[IN_RC]))
				== 0)
			person.state = ST_RC;
		else if (strncmp(input, inMessage[IN_GRL], strlen(inMessage[IN_GRL]))
				== 0)
			person.state = ST_GRL;
		else if (strncmp(input, inMessage[IN_GRU], strlen(inMessage[IN_GRU]))
				== 0)
			person.state = ST_GRU;
		else if (strncmp(input, inMessage[IN_GLL], strlen(inMessage[IN_GLL]))
				== 0)
			person.state = ST_GLL;
		else if (strncmp(input, inMessage[IN_GLU], strlen(inMessage[IN_GLU]))
				== 0)
			person.state = ST_GLU;
		else if (strncmp(input, inMessage[IN_EXIT], strlen(inMessage[IN_EXIT]))
				== 0)
			person.state = ST_EXIT;

		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s\n", errorMessages[IN_ERR_SND]);
			exit(EXIT_FAILURE);
		}

		if (sizeof(controller_response) == 0) {
			printf("%s\n", errorMessages[IN_ERR_RSP]);
			exit(EXIT_FAILURE);
		}

		if (controller_response.statusCode != EOK) {
			printf("%s %s\n", errorMessages[ERR_SRVR_MSG],
					controller_response.errMsg);
		}

		if (person.state == ST_EXIT)
			break;
	}
	sleep(MAX_NUMBER_SECONDS);
	ConnectDetach(coid);
	return EXIT_SUCCESS;

}
