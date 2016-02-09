#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 12345
#define SHM_SIZE 0

int main(int argc, char *argv[])
{
    int shm_id;
    char *shm;
    int i;

    /* 共有メモリの獲得 */
    shm_id = shmget(SHM_KEY, SHM_SIZE, 00400 | 00200);
    if (shm_id < 0) {
	fprintf(stderr, "shmget() error\n");
	perror(argv[0]);
	return 1;
    }
    fprintf(stderr, "%s:shm_id = %d\n", argv[0], shm_id);
    shm = shmat(shm_id, NULL, 0);
    if (shm < 0) {
	fprintf(stderr, "shmmat() error\n");
	perror(argv[0]);
	return 2;
    }
    for (i = 0; i < 100; i++) {
	fputc(shm[i], stderr);
    }
    fputc('\n',stderr);
    *shm = 'B';
    return 0;
}
