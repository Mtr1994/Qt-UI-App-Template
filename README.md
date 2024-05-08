# Qt-Ui-Template

![Platform](https://img.shields.io/badge/paltform-win10_x64-brightgreen)
![Qt Version](https://img.shields.io/badge/_Qt_-5.15.2-yellowgreen)
![Build](https://img.shields.io/badge/build-MSVC_2019_x64-blue)

#### 介绍
通用的 Qt Widget 程序模板

#### 软件架构
`Qt`

#### 快速获取

从 `Github` 获取模板程序需要多个步骤，耗费时间，使用脚本来获取会更方便，脚本内容如下：

```
#!/bin/bash
# create by mtr at 2021/05/13 09:59
# 

echo -e "init git repository \r\n"
git init

echo -e "add remote origin \r\n"
git remote add origin git@gitee.com:Mtr1994/Qt-Ui-Template.git

echo -e "get template code from master \r\n"
git pull origin master

echo -e "delete git repository ..."
rm -rf .git
rm -rf README.md
rm -rf LICENSE
rm -rf .gitignore
echo -e "finished"
```
