#include <core/sys.h>
#include <stdio.h>
#include <unistd.h>

/*
  Make a helper function with a simple well defined semantics:
*/
static int spawn_proc(int in, int out, struct command* cmd)
{
    pid_t pid = fork();

    if (pid == 0) {
        if (in != 0) {
            dup2(in, 0);
            close(in);
        }

        if (out != 1) {
            dup2(out, 1);
            close(out);
        }

        return execvp(cmd->argv[0], (char* const*)cmd->argv);
    }

    return pid;
}

/*
  And here's the main fork routine:
*/
int fork_pipes(size_t n, struct command* cmd)
{
    int in, fd[2];
    size_t i;

    /* The first process should get its input from the original file descriptor
     * 0.  */
    in = 0;

    /* Note the loop bound, we spawn here all, but the last stage of the
     * pipeline.  */
    for (i = 0; i < n - 1; ++i) {

        pipe(fd);

        /* f[1] is the write end of the pipe, we carry `in` from the prev
         * iteration.  */
        spawn_proc(in, fd[1], cmd + i);

        /* No need for the write and of the pipe, the child will write here.  */
        close(fd[1]);

        /* Keep the read end of the pipe, the next child will read from there.
         */
        in = fd[0];
    }

    /* Last stage of the pipeline - set stdin be the read end of the previous
    pipe
    and output to the original file descriptor 1. */
    if (in != 0) {
        dup2(in, 0);
    }

    /* Execute the last stage with the current process. */
    return execvp(cmd[i].argv[0], (char* const*)cmd[i].argv);
}
