#include <cmd.h>

int my_rm(int argc, char* argv[])
{
    result_t result = NONE;
    const int uid = running->uid;
    const int device = running->cwd->device;

    if(argc < 2)
    {
        fprintf(stderr, "rm: missing operand\n");
        return MISSING_OPERAND;
    }

    // rm each path given by user
    int i = 1;
    while(i < argc)
    {
        char* path  = argv[i];
        int ino     = getino(device, path);
        MINODE* mip = iget(device, ino);

        // Verify file exists
        if(!mip)
        {
            result = DOES_NOT_EXIST;
            fprintf(stderr, "rm: failed to remove '%s':"
                    " No such file or directory\n", path);
            goto clean_up;
        }
        // Verify user has permission to remove the file 
        else if(uid != SUPER_USER && uid != mip->inode.i_uid)
        {
            result = PERM_DENIED;
            fprintf(stderr, "rm: failed to remove '%s':"
                    " Permission denied\n", path);
            goto clean_up;
        }
        // Verify that it is not a directory
        else if(S_ISDIR(mip->inode.i_mode))
        {
            result = IS_DIR;
            fprintf(stderr, "rm: failed to remove '%s':"
                    " Is a directory\n", path);
            goto clean_up;
        }
        // Verify that it is not busy
        else if(mip->refCount > 1)
        {
            result = BUSY;
            fprintf(stderr, "rm: failed to remove file '%s':"
                    " File busy\n", path);
            goto clean_up;
        }

        // If removing multiple files, display
        if(argc > 2)
            printf("rm: removing file '%s'\n", path);

        INODE* ip = &mip->inode;

        // Deallocate its blocks
        for(int b = 0; b < NUM_DIRECT_BLOCKS && ip->i_block[b] != 0; b++)
            bfree(device, ip->i_block[b]);

        // Deallocate its inode
        ifree(device, ino);

        // Get parent
        char*  dirname = NULL;
        char* basename = NULL;
        parse_path(path, &dirname, &basename);

        int parent_ino = getino(device, dirname);
        free(dirname);
        free(basename);

        MINODE* parent_mip = iget(device, parent_ino); 
        INODE*   parent_ip = &parent_mip->inode;

        // Remove entry from parent directory
        rm_child(parent_mip, ino); 

        // Update parent's info
        parent_ip->i_atime = time(0L);
        parent_ip->i_mtime = time(0L);
        parent_mip->dirty = true;

        // Write parent changes to disk
        iput(parent_mip);

clean_up:
        // Write changes to deleted file to disk and clear refCount
        iput(mip); 

        if(result != NONE)
            return result;
        
        i++;
    }

    return SUCCESS;
}
