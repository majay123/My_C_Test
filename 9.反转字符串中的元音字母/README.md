345.反转字符串中的元音字母
===========================
题目内容介绍

****

| Author | 马陈东            |
| ------ | ------------------- |
| E-mail | machendong@nhope.cn |
|  From  | LEETCODE             |

****

编写一个函数，以字符串作为输入，反转该字符串中的元音字母。
```c
输入: "hello"
输出: "holle"

输入: "leetcode"
输出: "leotcede"
```

***
主要解题思想：先列出元音字母 const char array[10] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};，先判断头尾是否是元音字母，若两边都是元音字母，则进行调换


