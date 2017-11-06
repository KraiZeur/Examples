// sudo apt-get install libcap-dev
#include <sys/capability.h>
#include <stdio.h>

/*

typedef struct __user_cap_header_struct {
              __u32 version;
              int pid;
           } *cap_user_header_t;

           typedef struct __user_cap_data_struct {
              __u32 effective;
              __u32 permitted;
              __u32 inheritable;
           } *cap_user_data_t;

*/

int main(void)
{
    struct __user_cap_header_struct cap_header_data;
    struct __user_cap_data_struct cap_data_data;
    
    cap_user_header_t cap_header = &cap_header_data;
    cap_user_data_t cap_data = &cap_data_data;

    int ret = capget(cap_header, cap_data);
    if (ret<0)
    {
        perror("Failed capget");
    }

    printf("Cap data 0x%x, 0x%x, 0x%x\n", cap_data->effective,
           cap_data->permitted, cap_data->inheritable);


    //capset(cap_user_header_t hdrp, const cap_user_data_t datap);

    return 0;
}