/*
 * =====================================================================================
 *
 *       Filename:  _util.c
 *
 *    Description:  没有归类的工具函数都放到在这儿了。
 *
 *        Version:  1.0
 *        Created:  2012年02月24日 11时23分01秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  聂汉子 (niehanzi), niehanzi@qiyi.com
 *        Company:  奇艺世纪
 *
 * =====================================================================================
 */
#include "_other.h"

char *file_suffix(char *file_name){
    int   str_len = 0;
    char *ptr = NULL;

    if(NULL == file_name){
        return NULL;
    }
    
    str_len = strlen(file_name); 
    if(0 == str_len){
        return NULL;
    }

    ptr = file_name + str_len - 1;
    if('.' == *ptr){
        return NULL;
    }

    while(file_name <= ptr){
        if('.' == *ptr){
            return ptr + 1;
        }

        ptr--;
    }
    return NULL;
}

off_t file_get_length(const char *file_name)
{
    struct stat stat_buf;

    if (stat(file_name, &stat_buf) == 0){
        return stat_buf.st_size;
    }

    return -1;
}

int create_random(int max){
    struct timeval cur_time;
    gettimeofday(&cur_time, NULL);
    //srand((int)time(0));
    srand((int)cur_time.tv_usec);
    return random() % max;
}

static void dir_list_cb(char *file_name){
    printf("%s\n", file_name);
}

int list_dir(char *path, void (*callback)(char *file_name)){
    DIR           *dir   = NULL;     
    struct dirent *entry = NULL;
    char          file_path[128];

    dir = opendir(path);
    if(NULL == dir){
        return 0;
    }

    while(NULL != (entry = readdir(dir))){
        if(0 == strcmp(entry->d_name, ".") ||
                0 == strcmp(entry->d_name, "..")){
            continue;
        }
        bzero(file_path, sizeof(file_path));
        snprintf(file_path, 127, "%s%s",
                path, entry->d_name);
        if(callback){
            callback(file_path);
        }
        else{
            dir_list_cb(file_path);
        }
    }

    closedir(dir);
    dir = NULL;
    entry = NULL;
    return 1;
}

int create_dir(char *path_name){
    int    i,len;
    char   str[1024] = {0};

    strncpy(str, path_name, sizeof(str) - 1);
    len = strlen(str);

    for(i = 0; i < len; i++){
        if(str[i] == '/' && i > 0){
            str[i] = '\0';
            if(access(str, F_OK) != 0){
                if(-1 == mkdir(str, 0777)){
                    return 0;
                }
            }
            str[i] = '/';
        }
    }
    if(len > 0 && access(str, F_OK) != 0){
        if(-1 == mkdir(str, 0777)){
            return 0;
        }
    }
    return 1;
}

int file_exist(char *file_name)
{
    if (access(file_name, R_OK) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int create_file(char *file_name, size_t size)
{
    FILE *fp = NULL; 

    /*  如果文件不存在，创建他 */
    fp = fopen(file_name, "w"); 
    if(NULL == fp){
        printf("fopen %s failed.\n", file_name);
        return 0;
    }   
    fclose(fp);

    if (truncate(file_name, size) < 0) {
        printf("truncate %s failed, %s.\n", file_name, strerror(errno));
        return 0;
    }
    return 1;
}

int create_file_if_not_existed(char *file_name)
{
    FILE *fp = NULL; 

    /*  如果文件存在，成功返回 */
    if(access(file_name, R_OK) == 0){ 
        return 1;
    }   

    /*  如果文件不存在，创建他 */
    fp = fopen(file_name, "w"); 
    if(NULL == fp){
        printf("fopen %s failed.\n", file_name);
        return 0;
    }   

    fclose(fp);
    return 1;
}

int remove_file(char *file_name)
{
    if(access(file_name, R_OK) < 0){
        return 1; 
    }

    if(unlink(file_name) < 0){
        printf("unlink %s failed, %s.", file_name, strerror(errno));
        return 0;
    }

    return 1;
}

#ifdef TEST 
int main(int argc, char **argv){
    char buffer[1024] = {0};
    time_str(buffer, sizeof(buffer));
    printf("%s\n", buffer);

    time_str_by_time(buffer, sizeof(buffer), 1335950049);
    printf("%s\n", buffer);

    time_str_by_time(buffer, sizeof(buffer), 1335832031);
    printf("%s\n", buffer);

    time_str_by_time(buffer, sizeof(buffer), 1335832032);
    printf("%s\n", buffer);
    return 1;
}
#endif
