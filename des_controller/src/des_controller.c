/**************************************************
 * File: des_controller.c
 * Author: Aliab Eman (041000420) and Savas Erturk (040919022)
 **************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <errno.h>
#include "../../des_display/src/des.h"

void *state_LS(); /*	 LEFT SCAN 			*/
void *state_LO(); /*  LEFT OPEN  		*/
void *state_LC(); /*  LEFT CLOSE 		*/
void *state_RS(); /*  RIGHT SCAN 		*/
void *state_RO(); /*  RIGHT OPEN 		*/
void *state_RC(); /*  RIGHT CLOSE 		*/
void *state_GRL(); /*  GUARD RIGHT LOCK 	*/
void *state_GRU(); /*  GUARD RIGHT UNLOCK */
void *state_GLL(); /*  GUARD LEFT LOCK 	*/
void *state_GLU(); /*  GUARD LEFT UNLOCK	*/
void *state_WS1(); /*  WEIGHT SCALE 		*/
void *state_WS2(); /*  WEIGHT SCALE 		*/
void *state_start();/*  Start 				*/
void *state_exit(); /*  EXIT 				*/

void initialize();

int coid;
int chid;
int rcvid;

controller_response_struct controller_response;
Person person;

FState f_state = state_start;
int door_state = DEFAULT;

int main(int argc, char* argv[]) {
	int MAX_ARGC = 2;
	pid_t displayId;
	int TRUE = 1;

	if (argc != MAX_ARGC) {
		printf("%s\n", errorMessages[CTRL_ERR_USG]);
		exit(EXIT_FAILURE);
	}

	displayId = atoi(argv[1]); // convert string to int

	// PHASE I
	chid = ChannelCreate(0);
	if (chid == -1) {
		// Upon Failure
		printf("%s\n", errorMessages[CTRL_ERR_CHANNEL_CREATE]);
		exit(EXIT_FAILURE);
	}

	coid = ConnectAttach(ND_LOCAL_NODE, displayId, 1, _NTO_SIDE_CHANNEL, 0);

	if (coid == -1) {
		// Upon Failure
		printf("%s\n", errorMessages[CTRL_ERR_CONN]);
		exit(EXIT_FAILURE);
	}

	printf("%s %d\n", outMessage[OUTPUT_START], getpid());
	printf("Waiting for Person...");
	initialize();

	while (TRUE) {
		// PHASE II
		rcvid = MsgReceive(chid, &person, sizeof(person), NULL);
		if (rcvid == -1) {
			printf("%s\n", errorMessages[CTRL_ERR_RCV]);
			exit(EXIT_FAILURE);
		}

		if (person.state == ST_EXIT) {
			f_state = (*state_exit)();
		} else {
			f_state = (FState) (*f_state)();
		}
		controller_response.statusCode = EOK;

		if (person.state != ST_EXIT)
			MsgReply(rcvid, EOK, &controller_response,
					sizeof(controller_response));

		if (person.state == ST_EXIT)
			break;

	}
	ConnectDetach(coid);
	ChannelDestroy(chid);
	return EXIT_SUCCESS;
}

//State 0
void *state_start() {
	if (person.state == ST_LS) {
		return state_LS; //State 1
	}
	return state_start; //State 0
}

//State 1
void *state_LS() {
	if (person.state == ST_GLU) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s GLU - st_ls()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		return state_GLU; //State 2
	}
	return state_LS; //State 1
}

//State 2
void *state_GLU() {
	if (person.state == ST_LO) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s LO - st_glu()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		return state_LO; //State 3
	}
	return state_GLU; //State 2
}

//State 4
void *state_WS1() {
	if (person.state == ST_LC) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s LC - st_ws()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		return state_LC; //State 5
	}
	return state_WS1; //State 4
}

//State 3
void *state_LO() {
	if (person.state == ST_WS1) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s WS -st_lo()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		return state_WS1; //State 4
	}
	return state_LO; //State 3
}




//State 5
void *state_LC() {
	if (person.state == ST_GLL) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s GLL - st_lc()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		return state_GLL; //State 6
	}
	return state_LC; //State 5
}

//State 6
void *state_GLL() {
	if (door_state == DEFAULT
			&& person.state == ST_RS) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s RS - st_gll()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		door_state = RIGHT;
		return state_RS; //State 7
	}
	return state_GLL; //State 6
}

//State 7
void *state_RS() {
	if (person.state == ST_GRU) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s GRU - st_rs()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		return state_GRU; //State 8
	}
	return state_RS; //State 7
}

//State 8
void *state_GRU() {
	if (person.state == ST_RO) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s RO - st_gru()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		return state_RO; //State 9
	}
	return state_GRU; //State 8
}

//State 10
void *state_WS2() {
	if (person.state == ST_RC) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s RC - st_ws()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		return state_RC; //State 11
	}
	return state_WS2; //State 10
}

//State 9
void *state_RO() {
	if (person.state == ST_WS2) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s WS - st_ro()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		return state_WS2; //State 10
	}
	return state_RO; //State 9
}




//State 11
void *state_RC() {
	if (person.state == ST_GRL) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s GRL - st_rc() \n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		return state_GRL; //State 12
	}
	return state_RC; //State 11
}

//State 12
void *state_GRL() {
	if (door_state == RIGHT && person.state == ST_GLU) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s GLU - st_grl()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		door_state = DEFAULT;
		return state_GLU;
	}
	if (door_state == DEFAULT && person.state == ST_RS) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s RS - st_grl()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		door_state = RIGHT;
		return state_RS;
	}
	if (door_state == DEFAULT && person.state == ST_LS) {
		if (MsgSend(coid, &person, sizeof(person), &controller_response,
				sizeof(controller_response)) == -1) {
			printf("%s LS - st_grl()\n", errorMessages[CTRL_ERR_SND]);
			exit(EXIT_FAILURE);
		}
		door_state = LEFT;
		return state_LS;
	}

	return state_GRL; //State 12
}



void *state_exit() { /*  EXIT */

	if (MsgSend(coid, &person, sizeof(person), &controller_response,
			sizeof(controller_response)) == -1) {
		printf("%s EXIT - st_exit()\n", errorMessages[CTRL_ERR_SND]);
		exit(EXIT_FAILURE);
	}
	return state_exit;
}

void initialize() {
	person.id = 0;
	person.weight = 0;
	person.state = ST_START;
}
