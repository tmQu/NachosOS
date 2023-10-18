#include "MyUtility.h"

int main() {
	int pingPID, pongPID;
	print("Ping-Pong test starting ... \n\n");
	pingPID = Exec("./test/ping", 1);
	pongPID = Exec("./test/pong", 0);

	while (1) {};
	return 0;
}

