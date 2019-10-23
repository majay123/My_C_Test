#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int** threeSum(int* nums, int numsSize, int* returnSize, int** returnColumnSizes){
    int count = 0;
    int tmp_nums[3] = {0};
    int** ret;

    printf("teststststts\n");
    ret = (int **)malloc(sizeof(int *) * numsSize);//分配指针数组

    ret[count] = (int *)malloc(sizeof(char) * 3);//分配每个指针所指向的数组


    for (int i = 0; i < numsSize; i++)
    {
        for (int j = 0; j < numsSize - 2; j++){
            if((nums[i] + nums[i + 1] + nums[j + 2]) == 0) {
                ret[count][0] = nums[i];
                ret[count][1] = nums[i+1];
                ret[count][2] = nums[j+2];
                // (*returnColumnSizes)[*returnSize] = 3;
                printf("%d %d %d\n", nums[i], nums[i+1], nums[j+2]);
                count++;
                ret[count] = (int *)malloc(sizeof(int) * 3);
                
                printf("count = %d\n", count);
                //returnColumnSizes[count] = tmp_nums;
                i++;
                printf("i = %d\n", i);
            }
        } 
    }
    returnSize = &count;
    return ret;
}

int results_end = 0;
int array[] = {0};
int results[] = {0};
int tmp = 0;
int temp = 0;

int target1 = 0;

void comb(int deep, int n, int m )
{
    int i = 0;
    int sum = 0;
    
    if(deep > n)    //越界递归结束
        return ; 
    if(results_end == m){
        sum = 0;
        for(int i = 0; i < m; i++){
            sum += results[i];
        }
        temp = target1 - sum;
        if(abs(tmp) > abs(temp))
            tmp = temp;
        return;
    }
    results[results_end++] = array[deep];
    comb(deep + 1, n, m);
    results_end--;
    comb(deep + 1, n, m);
}

int threeSumClosest(int* nums, int numsSize, int target){
    int i = 0;
    
    target1 = target;
    tmp = (target - (nums[0] + nums[1] + nums[2]));
    memcpy(array, nums, numsSize);
    comb(0, numsSize, 3);
    return target - tmp;
}
int main (void)
{
    int nums[8] = {1,2,4,8,16,32,64,128};
    int numsSize = 8;
    int* returnSize;
    int** returnColumnSizes;

    // printf("msgggg\n");
    // threeSum(nums, numsSize, returnSize, returnColumnSizes);
    int s = threeSumClosest(nums, 8, 82);
    printf("s = %d\n", s);
}