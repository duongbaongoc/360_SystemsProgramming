/* James Jessen
 * 10918967   
 *
 * CptS 360
 */

#include <stdio.h>

#include <global.h>
#include <input.h>
#include <print.h>
#include <cmd.h>

#include <init.c>

#include "FileUnitTest.c"


int main(int argc, char* argv[])
{
    char* device = "disk/mydisk"; //default

    if (argc > 2)
    {
        printf("Usage:\n");
        printf("         myext2   DEVICE\n");
        printf("  e.g.   myext2   mydisk\n\n");
        exit(1);
    }

    if(argc > 1)
        device = argv[1];

    initialize_fs(); 
    mount_root(device);
    printf("root refCount = %d\n", root->refCount);

    printf("creating P0 as running process\n");
    running = &ProcessTable[0];
    running->status = READY;
    running->cwd = root; root->refCount++;
    running->uid = SUPER_USER;
    printf("root refCount = %d\n", root->refCount);

    // Create Process1 with uid = 1 for non-super user

    FileUnitTest();

    while(true)
    {
        char*  input    = NULL;
        char** cmd_argv = NULL;
        int    cmd_argc = 0;
        int  (*cmd_fptr)(int, char**) = NULL; 

        char* dir = find_name(running->cwd);

        do // Prompt user for input
        { 
            printf("[%d %s]$ ", running->uid, dir); 
        }     
        while(!(input = get_input()));   // Get user input
        free(dir);

        cmd_argv = parse(input, " ");    // Parse input into cmd argv[]
        free(input);

        while(cmd_argv[++cmd_argc]){}    // Determine cmd argc

        cmd_fptr = get_cmd(cmd_argv[0]); // Get the command's function pointer
        cmd_fptr(cmd_argc, cmd_argv);    // Execute the command with parameters
        free_array(cmd_argv);
    }

    return 0;
}
