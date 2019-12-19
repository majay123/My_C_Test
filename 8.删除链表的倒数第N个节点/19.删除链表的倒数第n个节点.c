/*
 * @lc app=leetcode.cn id=19 lang=c
 *
 * [19] 删除链表的倒数第N个节点
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */


struct ListNode* removeNthFromEnd(struct ListNode* head, int n){
    struct ListNode* start = head;
    struct ListNode* end = head;
    
    while(n--!=0)
        end = end->next;

    if(!end)
        return head->next;
    while(end->next){
        start = start->next;
        end = end->next;
    }
    start->next = start->next->next;
    return head;

}


// @lc code=end

