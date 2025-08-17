#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// void filter(int *input_pipe) {
//     close(input_pipe[1]); // 关闭输入管道的写端

//     int num;
//     if (read(input_pipe[0], &num, sizeof(num)) <= 0) {
//         close(input_pipe[0]);
//         exit(0);
//     }

//     printf("%d\n", num);

//     int output_pipe[2];
//     pipe(output_pipe);

//     if (fork() == 0) {
//         // 子进程递归处理
//         close(input_pipe[0]); // 关闭父管道的读端
//         filter(output_pipe);
//     } else {
//         // 父进程
//         close(output_pipe[0]); // 关闭新管道的读端

//         int next_num;
//         while (read(input_pipe[0], &next_num, sizeof(next_num)) > 0) {
//             if (next_num % num != 0) {
//                 write(output_pipe[1], &next_num, sizeof(next_num));
//             }
//         }

//         close(input_pipe[0]); // 关闭父管道的读端
//         close(output_pipe[1]); // 关闭新管道的写端
//         wait(0); // 等待子进程结束
//         exit(0);
//     }
// }

// int main(int argc, char **argv) {
//     int pipe_fd[2];
//     pipe(pipe_fd);

//     if (fork() == 0) {
//         // 子进程开始筛选
//         filter(pipe_fd);
//     } else {
//         // 主进程生成数字
//         close(pipe_fd[0]); // 关闭管道的读端

//         for (int i = 2; i <= 35; i++) {
//             write(pipe_fd[1], &i, sizeof(i));
//         }

//         close(pipe_fd[1]); // 关闭管道的写端
//         wait(0); // 等待子进程结束
//         exit(0);
//     }
//     return 0;
// }








int main(int argc, char **argv){
    int father[2];
    int son[2];
    pipe(son);
    int filter_num = 2;
    int lable=0;

    if(fork() != 0){
        // 主线程
        close(son[0]); // Close read end in parent
        // printf("pipe in %d out %d\n", son[0], son[1]);
        for(int i = 2; i <= 35; i++){
            write(son[1], &i, sizeof(i));
            // printf("main send %d\n", i);
        }
        close(son[1]); // Close write end after writing
        // 等待子进程结束
        wait(0);
        exit(0);
    } else {
        // 子线程
        father[0] = son[0];
        father[1] = son[1];
        printf("pid%d father pipe in %d out %d\n",getpid(), father[0], father[1]);
        sleep(3);
        pipe(son);
        // close(son[0]); // 不能在fork之前关闭，因为子进程需要用来读端
        close(father[1]); // Close write end in child
        printf("pid%d son pipe in %d out %d\n",getpid(), son[0], son[1]);
        
        int num;
        while(read(father[0], &num, sizeof(num)) > 0){
            if(lable == 0){
                filter_num = num;
                lable = 1;
                printf("%d\n", num);
                if(fork() == 0){
                    // 创建过滤器子进程
                    // printf("son proc %d create\n", getpid());
                    lable = 0;
                    close(father[0]); // Close read end in filter process
                    father[0] = son[0];
                    father[1] = son[1];
                    pipe(son);
                    // close(son[0]); //不能在fork之前关闭，因为它的子进程需要用这个描述符对应的管道读数据
                    close(father[1]); // 关闭父管道的写端
                    printf("pid%d father pipe in %d out %d\n",getpid(), father[0], father[1]);
                    sleep(3);
                    printf("pid%d son pipe in %d out %d\n",getpid(), son[0], son[1]);
                    
                    continue; // 继续读取父管道
                } else {
                    // close(son[0]); // Close read end in parent
                }
            }else{
                if(num % filter_num != 0){
                    write(son[1], &num, sizeof(num));
                    // int pid = getpid();
                    // printf("pid %d send %d\n", pid, num);
                }
            }
            // printf("%d\n", num);
        }
        close(son[1]); // Close write end after writing
        close(father[0]); // Close read end after reading
        wait(0); // 等待过滤器子进程结束
        exit(0);
        }
    exit(0);
}