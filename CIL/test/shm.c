#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 12345		/* ��ͭ����ˤĤ��륭����� */
				/* ��̾���ߤ����ʤ�Ρ��� */
#define SHM_SIZE 100		/* ���ݤ��붦ͭ����Υ����� */

int main(int argc, char *argv[])
{
    int shm_id;
    char *shm;
    int i;

    /* ��ͭ����κ����ʽ�ͭ�Ԥ��񤭹����ɤ߹��߲Ĥξ��֤ǡ� */
    shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 00400 | 00200);
    if (shm_id < 0) {		/* �����˼��Ԥ���������ͤ��֤� */
	fprintf(stderr, "shmget() error\n");
	perror(argv[0]);
	return 1;
    }
    fprintf(stderr,"%s:shm_id = %d\n", argv[0], shm_id);
    /* ��ͭ���꤬���̤Υ���Τ褦�˥��������Ǥ���褦�˥��ɥ쥹�� */
    /* ������Ƥ�ʡ��� */
    shm = shmat(shm_id, NULL, 0);
    if (shm < 0) {
	fprintf(stderr, "shmat() error\n");
	perror(argv[0]);
	return 2;
    }
    for (i = 0; i < SHM_SIZE; i++) {
	shm[i] = 'A' + i % 26;
    }
    if (fork() == 0)
	{
	    system("shm_tst", "r");
	    exit(0);
	}
    while (*shm == 'A')
	;
    /* ���ɥ쥹�ؤγ�����Ƥ������� */
    shmdt(shm);
    /* ��ͭ����������� */
    if (shmctl(shm_id, IPC_RMID, NULL) != 0) {
	perror(argv[0]);
	return 3;
    }
    return 0;
}
