### 语法参考1：
https://www.cnblogs.com/itheo/p/14272421.html

### 语法参考2：
https://developers.google.com/protocol-buffers/docs/proto3

### 用途
- 用于支持UI和gwservice之间实现信息高效编码解码

- 手动在当前目录下生成源代码，打开shell运行如下命令，注意，修改proto以后手动生成一次代码，测试一下语法是否正确：

```shell
protoc --c_out=. *.proto
```

