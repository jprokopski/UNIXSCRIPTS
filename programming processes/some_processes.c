#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
 
int main() {
        char buf[4], polecenie[4];
        while (1) {
                int status;
                printf("Podaj polecenie do wykonania [d,s,x,e,q]:\n");
                fgets(buf, sizeof(buf), stdin);
                sscanf(buf, "%s", polecenie);
                if (polecenie[0] == 'd'|| polecenie[0] == 's'|| polecenie[0] == 'x'|| polecenie[0] == 'e' || polecenie[0] == 'q') {
                        if (polecenie[0] != 'q') {
                                if (fork()==0) {
                                        printf("Tu potomek pid=%d\n", getpid());
                                        switch (polecenie[0]) {
                                                case 'd':
                                                        printf("URUCHOMIONO PROGRAM DATE\n");
                                                        // system("date");
                                                        execlp("date", "date", NULL);
                                                        break;
                                                case 's':
                                                        printf("URUCHOMIONO INTERPRETER SH\n");
                                                        // system("sh");
                                                        execlp("sh", "sh", NULL);
                                                        break;
                                                case 'x':
                                                        printf("URUCHOMIONO PROGRAM OKIENKOWY XCLOCK\n");
                                                        // system("xclock -update 1");
                                                        execlp("xclock", "xclock", "-update", "1", NULL);
                                                        break;
                                                case 'e':
                                                        printf("URUCHOMIONO PROGRAM OKIENKOWY ZENITY\n");
                                                        system("zenity=`zenity --file-selection`; xedit $zenity");
                                                        break;
                                        }
                                        exit(0);
                        }
                }
                while (waitpid((pid_t)-1,(int *)0,WNOHANG)>0){};
                if ((buf[0] != 'e') && (buf[0] != 'x')) {
                        waitpid(0,&status,0);
                }
                printf("Tu rodzic po utworzeniu potomka.\n");
                if(polecenie[0] == 'q') {
                        return 0;
                }
        }
    }
}