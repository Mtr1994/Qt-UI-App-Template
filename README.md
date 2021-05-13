# Qt-UI-App-Template
a template for all the application
## 介绍
  为了减少新建工程的固定工作量，提供一套 ui 程序的模板工程，附带默认的　QSS　文件、默认的图标、默认的初始化大小等，帮助开发者快速建立新的工程，避免前期重复的工作！
  
## 快速获取
从 `Github` 获取模板程序需要多个步骤，耗费时间，使用脚本来获取会更方便，脚本内容如下：

```
#!/bin/bash
# create by mtr at 2021/05/13 09:59
# 

echo -e "init git repository \r\n"
git init

echo -e "add remote origin \r\n"
git remote add origin git@github.com:Mtr1994/Qt-UI-App-Template.git

echo -e "get template code from master \r\n"
git pull origin master

echo -e "delete git repository ..."
rm -rf .git
rm -rf README.md
rm -rf LICENSE
rm -rf .gitignore
echo -e "finished"
```
