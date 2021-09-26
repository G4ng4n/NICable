#include "utils.h"

int main(int argc, char* argv[]){
	int index;
	switch (argc) {
	case 2:
		if (strcmp(argv[1], "list") == 0) {
			enum_NIC();
			return 0;
		}
		break;
	case 3:
		index = atoi(argv[2]);
		if (index == 0 || errno == ERANGE) {
			printf("Invalid index: %s\n", argv[1]);
			break;
		}
		if (strcmp(argv[1], "disable") == 0) {
			if (set_NIC(index, NIC_CONTROL_DISABLE)) {
				printf("NIC #%d successfully disabled!\n", index);
				return 0;
			}

			return 1;
		}
		else if (strcmp(argv[1], "enable") == 0) {
			if (set_NIC(index, NIC_CONTROL_ENABLE)) {
				printf("NIC #%d successfully enabled!\n",index);
				return 0;
			}
			return 1;
		}
		else {
			printf("Invalid argument: %s\n",argv[1]);
		}
	}

	printf("----- Usage -----\n");
	printf("List interfaces: %s list\n", argv[0]);
	printf("Disable interface #: %s disable #\n", argv[0]);
	printf("Enable interface #: %s enable #\n", argv[0]);
	return 0;
}
