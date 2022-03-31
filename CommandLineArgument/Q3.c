/*
    Author : Jap Purohit
    Purpose of the Program : Write a program that when executed creates two child processes using fork() system call where
                            one child process prints date and the other displays the contents of the folder that contains the
                            program. Further, all three processes print their PID and identify themselves as parent, child 1
                            and child 2 prior to printing anything else.
    Enrollement Number : AU1940109
*/
#include<stdio.h>
#include<time.h>
#include <dirent.h>
#include <unistd.h>

int main(){
    pid_t c1_pid, c2_pid;

    (c1_pid = fork()) && (c2_pid = fork()); // Creates two children
    if (c1_pid == 0) {
        /* Child 1 code goes here */
        printf("Child Procees-1 PID : %d\t\t Parent Process ID : %d\n",getpid(),getppid());
    
        time_t tm;
        time(&tm);
        printf("Current Date/Time = %s", ctime(&tm));
    } else if (c2_pid == 0) {
        /* Child 2 code goes here */
        
        printf("Child Procees-2 PID : %d\t\t Parent Process ID : %d\n",getpid(),getppid());
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                printf("%s\n", dir->d_name);
            }
            closedir(d);
        }
    } else {
            /* Parent code goes here */
        printf("Parent Process PID : %d\t\t Parent Process ID : %d\n",getpid(),getppid());
    }
    return 0;
}
