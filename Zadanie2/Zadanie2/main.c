#include "server.h"
#include "client.h"

int main(int argc, char** argv)
{
	if (strcmp(argv[1], "server") == 0) {
		server();
	}
	else if (strcmp(argv[1], "client") == 0) {
		client();
	}
	else {
		printf("Usage: server/client argument\n");
		return 2;
	}
	return 0;
}
