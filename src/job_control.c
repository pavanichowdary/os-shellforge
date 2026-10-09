#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <termios.h>

#include "job_control.h"


static pid_t shell_pgid;


/* =========================================================
   JOB CONTROL INITIALIZATION
   ========================================================= */

void job_control_init(void)
{
    /*
     * Shell should be interactive.
     */

    if (!isatty(STDIN_FILENO))
    {
        return;
    }


    shell_pgid = getpid();


    /*
     * Put shell in its own process group.
     */

    if (setpgid(
            shell_pgid,
            shell_pgid
        ) < 0)
    {
        /*
         * It may already be a process-group leader.
         */
    }


    /*
     * Give terminal to shell.
     */

    tcsetpgrp(
        STDIN_FILENO,
        shell_pgid
    );


    /*
     * Shell should not be stopped by
     * terminal job-control signals.
     */

    signal(SIGTTOU, SIG_IGN);

    signal(SIGTTIN, SIG_IGN);

    signal(SIGTSTP, SIG_IGN);
}


/* =========================================================
   GIVE TERMINAL TO JOB
   ========================================================= */

void give_terminal_to(pid_t pgid)
{
    tcsetpgrp(
        STDIN_FILENO,
        pgid
    );
}


/* =========================================================
   TAKE TERMINAL BACK
   ========================================================= */

void take_terminal_back(void)
{
    tcsetpgrp(
        STDIN_FILENO,
        shell_pgid
    );
}


/* =========================================================
   GET SHELL PROCESS GROUP
   ========================================================= */

pid_t get_shell_pgid(void)
{
    return shell_pgid;
}
