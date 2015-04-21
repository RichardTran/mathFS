#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>


// Array indices match functions[]<=>docNames[]<=>documents[] for easy pseudo key->value 
static const char* functions[]={
	"/",    	//0
	"/add", 	//1
	"/sub", 	//2
	"/mult",	//3
	"/div", 	//4
	"/factor",	//5
	"/exp", 	//6
	"/fib",  	//7
	};
static const char* docNames[]={
	"",    		//0
	"/add/doc", 	//1
	"/sub/doc", 	//2
	"/mult/doc",	//3
	"/div/doc", 	//4
	"/factor/doc",	//5
	"/exp/doc", 	//6
	"/fib/doc",  	//7
	};
static const char* documents[]={
	"", 						//0 empty since we won't be making a doc for root.
	"Adds two numbers.\n", 				//1
	"Subtracts two numbers.\n", 			//2
	"Multiplies two numbers.\n",			//3
	"Divides two numbers.\n",			//4
	"Computes the prime factors of a number\n",	//5
	"Raises a number to a given exponent\n",	//6
	"Computes the first n fibonacci numbers\n"	//7
	};
static int test_getattr(const char *path, struct stat *stbuf)
{
	printf("getAttr Here\n");
	int i =0;	
        int res = 0;
        memset(stbuf, 0, sizeof(struct stat));
	for(i=0;i<8;i++){
		printf("functions: %s\n",functions[i]);
		//path = mountpoint "/"	
		if (strcmp(path, functions[i]) == 0) 
		{ 	
			printf("Make Directory Here\n");
			stbuf->st_mode = S_IFDIR | 0755; //0755 = Admin can do all. Rest can read+exec
			stbuf->st_nlink = 3;
			return res;
        	} 
		else if(strcmp(path, docNames[i])==0){
			//0444 = Read only
			//Call math functions through here. Example: /add/3/3
			printf("Making a file, not directory.\n");
			stbuf->st_mode = S_IFREG | 0444;
			stbuf->st_nlink = 1;
			stbuf->st_size = strlen(documents[i]);
			return res;	
		}
	}
	printf("No entry~~~\n"); //basically turns a filename to be read = No such file or directory
        res = -ENOENT;
        return res;
}

// readdir is meant only to display all function directories and their docs to user
// 
// filler(buf, fileName, NULL, 0) are just for output for ls. Even we got rid of their filler, files still exist
// We add +1 to functions[i] to rid the first character, which in many cases is the '/' char.
static int test_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
        (void) offset;
	printf("readDir Here\n");
        (void) fi;
	int i =0;
	for(i=1;i<8;i++){
		if (strcmp(path,functions[i])==0){
			filler(buf, ".", NULL, 0); // if path == "/"
			filler(buf, "..", NULL, 0);
			filler(buf, "doc", NULL, 0);
			return 0; //important. without this, a directory wont have it
		}
	}
	if(strcmp(path,"/")==0){
        	filler(buf, ".", NULL, 0); // if path == "/"
        	filler(buf, "..", NULL, 0);
		int i;
		for(i=1;i<8;i++){
        		filler(buf, functions[i] + 1, NULL, 0);
		}
	}
        else{
                return -ENOENT;
	}
        return 0;
}
static int test_open(const char *path, struct fuse_file_info *fi)
{
	printf("test_Open\n");
	int i=0;
	int found = 0;
	for(i=1;i<8;i++){ //meant for docs
		if (strcmp(path,docNames[i])==0){
                	found = 1;
			break;
		}
	}
	if(found ==0){
		return -ENOENT;
	}
        if ((fi->flags & 3) != O_RDONLY)
                return -EACCES;
        return 0;
}
static int test_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
	printf("test_Read\n");
        size_t len;
        (void) fi;
	int i = 0;
	int found = 0;
	for(i=1;i<8;i++){ //meant for docs
		if (strcmp(path,docNames[i])==0){
                	found = 1;
			break;
		}
	}
	if(found ==0){
		return -ENOENT;
	}
	
        len = strlen(documents[i]);
        if (offset < len) {
                if (offset + size > len)
                        size = len - offset;
                memcpy(buf, documents[i] + offset, size); //storing the string into buf.
        } else
                size = 0;
        return size;
}
static struct fuse_operations test_oper = {
        .getattr        = test_getattr,
        .readdir        = test_readdir,
        .open           = test_open,
        .read           = test_read,
};
int main(int argc, char *argv[])
{
        return fuse_main(argc, argv, &test_oper, NULL);
}
