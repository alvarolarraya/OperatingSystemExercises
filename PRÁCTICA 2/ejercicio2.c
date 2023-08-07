
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void printCharArray(char array[], int arraylen) {
	for (int c = 0; c < arraylen; c++)
		printf("%c ", array[c]);
}

void removeElementFromCharArray(char array[], int arrayLen, int elem) {
	int offset = 0;
	for (int i = 0; i < arrayLen; i ++) {
		if (i == elem) {
			offset = 1;
		}
		array[i] = array[i + offset];
	}
}

void recursiveSubProcessCreator(char ids[], int idsLen) {
	int status;
	if (idsLen > 0) {
		pid_t pid = fork();

		if(pid == 0) {
			printf("Process %c-> PID: %d. PPID: %d.\n", ids[0], getpid(), getppid());
			removeElementFromCharArray(ids, idsLen, 0);
			recursiveSubProcessCreator(ids, idsLen - 1);
		}
		else {
			waitpid(pid, &status, 0);
			printf("Process %c (PID %d) Ended.\n", ids[0], pid);
		}
	}
}

int main() {
	char ids[4] = "ABCD";
	int idsLen = sizeof(ids) / sizeof(ids[0]);
	recursiveSubProcessCreator(ids, idsLen);

	return (0);
}
