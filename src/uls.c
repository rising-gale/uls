#include "uls.h"
typedef struct flags {
    int l;
    int C;
    int x;
    int R;
    int r;
    int t;
    int u;
    int c;
    int S;
    int a;
    int A;
    int force;
    int m;
    int files;
    int G;
    int T;
    int g;
    int o;
    
} st_flags;

void mx_print_file(const char *file_name){
    mx_printstr(file_name);
    mx_printchar(' ');
}
void mx_print_dir(const char *dir, st_flags *flags){
    struct dirent *d;
    DIR *dh = opendir(dir);
	if (!dh)
	{
		if (errno = ENOENT)
		{
			//If the directory is not found
			mx_printerr("uls: cannot access '");
            write(2,dir,mx_strlen(dir));
            mx_printerr("': No such file or directory\n");
            exit(0);
		}
		else
		{
			//If the directory is not readable then throw error and exit
			perror("Unable to read directory");
		}
		exit(EXIT_FAILURE);
	}
	//While the next entry is not readable we will print directory files
	while ((d = readdir(dh)) != NULL)
	{
        
		//If hidden files are found we continue
		if (flags->a == 0 && d->d_name[0] == '.')
			continue;
        
		// mx_printstr(d->d_name);
        // mx_printstr("  ");
		if(flags->l){
            struct stat buffer;
            stat(d->d_name, &buffer);
            
        } 
	}
	// if(!op_l)
	mx_printchar('\n');
}
void mx_display_long_format(char * name){
    struct stat buf;
    char buff_time[32];
    struct passwd* psd;  //Receive the user name of the file owner from this structure
    struct group* grp;   //Get group name
    // if(lstat(name,&buf)==-1)
    // {
    //     my_err("stat",__LINE__);
    // }
    if(S_ISLNK(buf.st_mode))
        printf("l");
    else if(S_ISREG(buf.st_mode))
        printf("-");
    else if(S_ISDIR(buf.st_mode))
        printf("d");
    else if(S_ISCHR(buf.st_mode))
        printf("c");
    else if(S_ISBLK(buf.st_mode))
        printf("b");
    else if(S_ISFIFO(buf.st_mode))
        printf("f");
    else if(S_ISSOCK(buf.st_mode))
        printf("s");
    //Get Print File Owner Rights
    if(buf.st_mode&S_IRUSR)
        printf("r");
    else
        printf("-");
    if(buf.st_mode&S_IWUSR)
        printf("w");
    else
        printf("-");
    if(buf.st_mode&S_IXUSR)
        printf("x");
    else
        printf("-");

    //All group permissions
    if(buf.st_mode&S_IRGRP)
        printf("r");
    else
        printf("-");
    if(buf.st_mode&S_IWGRP)
        printf("w");
    else
        printf("-");
    if(buf.st_mode&S_IXGRP)
        printf("x");
    else
        printf("-");

    //Other people's rights
    if(buf.st_mode&S_IROTH)
        printf("r");
    else
        printf("-");
    if(buf.st_mode&S_IWOTH)
        printf("w");
    else
        printf("-");
    if(buf.st_mode&S_IXOTH)
        printf("x");
    else
        printf("-");

    printf("  ");
    //Get the user name and group name of the file owner based on uid and gid
    psd=getpwuid(buf.st_uid);
    grp=getgrgid(buf.st_gid);
    printf("%4d ",buf.st_nlink);  //Link number
    printf("%-8s ",psd->pw_name);
    printf("%-8s ",grp->gr_name);

    printf("%6d",buf.st_size);
    strcpy(buff_time,ctime(&buf.st_mtime));
    buff_time[strlen(buff_time)-1]='\0'; //Buffe_time has its own newline, so you need to remove the following newline character
    printf("  %s  ",buff_time);
    cprint(name,buf.st_mode);
    printf("\n");

}
void mx_set_flags_zero(st_flags *flags){
    flags->l = 0;
    flags->C = 0;
    flags->x = 0;
    flags->x = 0;
    flags->R = 0;
    flags->r = 0;
    flags->t = 0;
    flags->u = 0;
    flags->c = 0;
    flags->S = 0;
    flags->a = 0;
    flags->A = 0;
    flags->force = 0;
    flags->m = 0;
    flags->files = 0;
    flags->G = 0;
    flags->T = 0;
    flags->g = 0;
    flags->o = 0;
}
void mx_parseflags(const char *str, st_flags * flags){

    for (int i = 1; i < mx_strlen(str); i++)
    {
        switch (str[i])
        {
        case 'a':
            flags->a = 1;
            break;
        case 'l':
            flags->l = 1;
            break;        
        default:
            mx_printerr("uls: unknown option -- ");
            write(2, &str[i], 1);
            exit(0);
            break;
        }
    }
    
}

// void mx_printwithflags(const char *str, st_flags *flags){

// }

int main(int argc, const char *argv[])
{
    st_flags flags;
    mx_set_flags_zero(&flags);
	if (argc == 1)
	{
		mx_print_dir(".",&flags);
	}
	else 
	{
        char **files = (char **)malloc(sizeof(char *) * argc);
        char **directories = (char **)malloc(sizeof(char *) * argc);
        for (int i = 1; i < argc; i++)
        {
           if(argv[i][0]=='-'){
               mx_parseflags(argv[i], &flags);
           }
        }
        int iterator_for_files = 0;
        int iterator_for_dirs = 0;
        for (int i = 1; i < argc; i++)
        {
            struct stat buffer;
            if(argv[i][0]!='-'){
               stat(argv[i],&buffer);
               if(S_ISREG(buffer.st_mode)){
                //    printf("%s","File \n");
                //    printf("%s",argv[i]);
                char *s = mx_strdup(argv[i]);
                //    mx_strcpy(files[iterator_for_files],s);
                   files[iterator_for_files] = s;
                   iterator_for_files++;
               } else if(S_ISDIR(buffer.st_mode)){
                //    printf("%s","Dir\n");
                char *s = mx_strdup(argv[i]);
                //    mx_strcpy(directories[iterator_for_dirs],s);
                   directories[iterator_for_dirs] = s;
                   iterator_for_dirs++;
                //    mx_print_dir(argv[i], &flags);
               } else {
			        mx_printerr("uls: cannot access '");
                    write(2,argv[i],mx_strlen(argv[i]));
                    mx_printerr("': No such file or directory\n");
                    exit(0);
               }
                    
            }
        }
        // printf("%s",directories[1]);
        for (int i = 0; i < iterator_for_files; i++)
        {
            mx_print_file(files[i]);
        }
        mx_printchar('\n');
        mx_printchar('\n');
        for (int i = 0; i < iterator_for_dirs; i++)
        {
            mx_printstr(directories[i]);
            mx_printstr(":\n");
            mx_print_dir(directories[i],&flags);
            mx_printchar('\n');
        }
        
        
		// if (argv[1][0] == '-')
		// {
		// 	//Checking if option is passed
		// 	//Options supporting: a, l
		// 	int op_a = 0, op_l = 0;
		// 	char *p = (char*)(argv[1] + 1);
		// 	while(*p){
		// 		if(*p == 'a') op_a = 1;
		// 		else if(*p == 'l') op_l = 1;
		// 		else{
		// 			perror("Option not available");
		// 			exit(EXIT_FAILURE);
		// 		}
		// 		p++;
		// 	}
		// 	mx_print_dir(".",op_a,op_l);
		// } else {
        //     for (int i = 1; i < argc; i++)
        //     {
        //         mx_print_dir(argv[i],0,0);
        //     }
            
            
        // }
        
	}
	return 0;
}
