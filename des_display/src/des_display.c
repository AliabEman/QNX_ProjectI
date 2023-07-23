/**************************************************
 * File: des_controller.c
 * Author: Aliab Eman (041000420) and Savas Erturk (040919022)
 **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include "des.h"

int main(void) {

	Person person;
	controller_response_struct response;

	int chid;
	int rcvid;
	int TRUE = 1;

	// PHASE I
	chid = ChannelCreate(0);
	if (chid == -1) {
		// On Failure: print error message and EXIT_FAILURE
		printf("%s\n", errorMessages[DP_ERR_CHANNEL_CREATE]);
		exit(EXIT_FAILURE);
	}

	printf("The display is running as PID: %d\n", getpid());

	// PHASE II
	while (TRUE) {

		if ((rcvid = MsgReceive(chid, &person, sizeof(person), NULL)) == -1) {
			printf("%s\n", errorMessages[DP_ERR_RCV]);
			exit(EXIT_FAILURE);
		}

		switch (person.state) {
		case ST_START:
			printf("%s %d \n", outMessage[OUTPUT_START], person.id);
			break;
		case ST_LS:
			printf("%s %d \n", outMessage[OUTPUT_LS], person.id);
			break;
		case ST_GLU:
			printf("%s\n", outMessage[OUTPUT_GLU]);
			break;
		case ST_LO:
			printf("%s\n", outMessage[OUTPUT_LO]);
			break;
		case ST_WS1:
			printf("%s %d \n", outMessage[OUTPUT_WS1], person.weight);
			break;
		case ST_RS:
			printf("%s %d \n", outMessage[OUTPUT_RS], person.id);
			break;
		case ST_WS2:
			printf("%s %d \n", outMessage[OUTPUT_WS2], person.weight);
			break;
		case ST_RO:
			printf("%s\n", outMessage[OUTPUT_RO]);
			break;
		case ST_LC:
			printf("%s\n", outMessage[OUTPUT_LC]);
			break;
		case ST_RC:
			printf("%s\n", outMessage[OUTPUT_RC]);
			break;
		case ST_GRL:
			printf("%s\n", outMessage[OUTPUT_GRL]);
			break;
		case ST_GRU:
			printf("%s\n", outMessage[OUTPUT_GRU]);
			break;
		case ST_GLL:
			printf("%s\n", outMessage[OUTPUT_GLL]);
			break;
		case ST_EXIT:
			printf("%s\n", outMessage[OUTPUT_EXIT]);
			break;
		default:
			printf("Invalid entry\n");
			break;
		}

		if (person.state != ST_EXIT) {
			if (MsgReply(rcvid, EOK, &response, sizeof(response)) == -1) {
				printf("%s\n", errorMessages[DP_ERR_RPLY]);
			}
		}

//		if (response.statusCode != EOK) {
//			printf("%s %s\n", errorMessages[ERR_SRVR_MSG], response.errMsg);
//		}

		if (person.state == ST_EXIT)
			break;
	}
	ChannelDestroy(chid);
	return EXIT_SUCCESS;
}
