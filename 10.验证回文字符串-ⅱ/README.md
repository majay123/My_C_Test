680.验证回文字符串-ⅱ
===========================
题目内容介绍

****

| Author | 马陈东            |
| ------ | ------------------- |
| E-mail | machendong@nhope.cn |
|  From  | LEETCODE             |

****

给定一个非空字符串 s，最多删除一个字符。判断是否能成为回文字符串。
示例:
```c
输入: "aba"
输出: True

输入: "abca"
输出: True
解释: 你可以删除c字符。
```

***
主要解题思想：只可以删除一个字母，在for如遇见不相等的字母，（对正序的i-1，反序的不操作，调用_check_char()函数再次进行判断回文的返回值 || 对正序的不操作，反序的j-1，调用_check_char()函数再次进行判断回文的返回值） 进行判断 返回
