#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 12345		/* 共有メモリにつけるキーワード */
				/* （名前みたいなもの？） */
#define SHM_SIZE 100		/* 確保する共有メモリのサイズ */

int main(int argc, char *argv[])
{
    int shm_id;
    char *shm;
    int i;

    /* 共有メモリの作成（所有者が書き込み読み込み可の状態で） */
    shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 00400 | 00200);
    if (shm_id < 0) {		/* 作成に失敗したら負の値を返す */
	fprintf(stderr, "shmget() error\n");
	perror(argv[0]);
	return 1;
    }
    fprintf(stderr,"%s:shm_id = %d\n", argv[0], shm_id);
    /* 共有メモリが普通のメモリのようにアクセスできるようにアドレスを */
    /* 割り当てる（？） */
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
    /* アドレスへの割り当てを解除する */
    shmdt(shm);
    /* 共有メモリを削除する */
    if (shmctl(shm_id, IPC_RMID, NULL) != 0) {
	perror(argv[0]);
	return 3;
    }
    return 0;
}
