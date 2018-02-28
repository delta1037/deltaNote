# GeniusNote
给自己立个flag，做一个便签，支持电脑和手机，电脑和手机都可以添加任务到列表中，支持不同手机登录的同步功能

# 服务端
  支持手机和电脑添加任务
  向电脑与手机同步便签内容
  支持用户邮箱注册，(注册成功为用户新建数据库，向数据库中添加表，记录等)
# 手机
  app
  添加任务
  更新本地任务
# 电脑
  修改字体颜色，背景透明度，背景颜色
  添加任务
  更新本地任务
  
  
# server
  socket
  redis
  创建用户
  添加，删除记录
 
# client
  socket
  列表，本地数据保存
  添加，删除记录
  
  
# app

## 功能：
- 用户的创建，登录功能
- 添加，删除代办事项
- 代办事项分类（自由为事项添加标签，比如添加 作业，生活，。。。，并以此将待办事项分类）
- 可以将所有事项一起查看，也可以按照标签来分别查看
- 待办任务提醒（闹钟）
- 刷新列表与网络同步（比如在电脑端更改了任务内容，需要对手机本地进行更新）

# 客户端功能具体化
- 同步功能：每个待办事项用创建时间作为标示符，分别建立各端的状态码，0表示已经同步，1表示未同步，2表示已删除，在用户点击同步按钮时，客户端向服务器发送列表数据，服务器根据状态码进行更新，并给客户端反馈
- 数据传输：利用json文件传输，实现信息的打包发送
