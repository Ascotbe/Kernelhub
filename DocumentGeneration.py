# !/usr/bin/env python
# -*- coding: utf-8 -*-
import os 
import re
Path="/Users/ascotbe/code/Kernelhub"
window_success_list=[]
window_failure_list=[]
def WindowsConfig():
    #中文配置的开头
    chinese_begin="""let config = {\n    title: 'Kernelhub',\n    home: 'Info.md',\n    repo: 'Ascotbe/Kernelhub',\n    nav: [\n        {\n            title: '简介', path: '/'\n        },\n        {\n            title: '目录导航',type: 'dropdown', items: [\n                {\n                    title: "有利用脚本(测试成功)",path: '/TestSuccess'\n                },"""
    #所有配置的结尾
    config_end="""\n            ]\n        },\n    ],\n    tocVisibleDepth: 10,\n    plugins: []\n};"""
    #下表是黑名单，垃圾文件
    reject_list=['.DS_Store', '.git','.idea', '.gitignore','LICENSE', 'Patch', 'README.CN.md', 'README.md', 'TestFailure', 'docs', 'DocumentGeneration.py','StencilPlate']
    #英文配置的开头
    english_begin="""let config = {\n    title: 'Kernelhub',\n    home: 'Info.md',\n    repo: 'Ascotbe/Kernelhub',\n    nav: [\n        {\n            title: 'Introduction', path: '/'\n        },\n        {\n            title: 'DirectoryNavigation',type: 'dropdown', items: [\n                {\n                    title: "exploit script (successful test)",path: '/TestSuccess'\n                },"""
    #处理成功的编号
    for i in os.listdir(Path+"/Windows"):
        if i not in reject_list:
            window_success_list.append(i)
    #循环成功的编号和开头配置进行拼接
    for x in sorted(window_success_list):
        chinese_begin+="""\n                {\n                        path: '/CN/"""+x+"""', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/"""+x+"""/README.md'\n                },"""
        english_begin+="""\n                {\n                        path: '/EN/"""+x+"""', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/"""+x+"""/README_EN.md'\n                },"""
    #插入中间内容，未测试成功内容
    chinese_begin+="""\n                {\n                        title: '有利用脚本(测试未成功)', path: '/TestFailure'\n                },"""
    english_begin+="""\n                {\n                        title: 'exploit script (test failure)', path: '/TestFailure'\n                },"""

    for i in os.listdir(Path+"/Windows/TestFailure"):
        if i not in reject_list:
            window_failure_list.append(i)
    #写入windows测试失败漏洞
    for xx in sorted(window_failure_list):
        chinese_begin+="""\n                {\n                        path: '/CN/"""+xx+"""', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/"""+xx+"""/README.md'\n                },"""
        english_begin+="""\n                {\n                        path: '/EN/"""+xx+"""', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/"""+xx+"""/README_EN.md'\n                },"""

    chinese_config_file=open(Path+"/docs/Windows/Docs/config.js","w+")
    chinese_config_file.write(chinese_begin+config_end)
    chinese_config_file.close()
    english_config_file=open(Path+"/docs/Windows/EnglishDocs/config.js","w+")
    english_config_file.write(english_begin+config_end)
    english_config_file.close()


def WindowsMarkdown():
    common_vulnerabilities_and_exposures_list=[]
    chinese_success_list=[]
    chinese_failure_list=[]
    english_success_list=[]
    english_failure_list=[]
    #打开Windows目录下面的文件读取内容
    file=open(Path+"/Windows/README.md","r").read()
    date = re.findall(r'(?<=> Numbered list)([\w\W]+)(?=### Required environment)', file)[0].split("\n")
    for i in date:
        if i != "":
            common_vulnerabilities_and_exposures_list.append(i)
    for x in common_vulnerabilities_and_exposures_list[2:]:
        date = re.findall(r'(?<=\[)([\w\W]+)(?=\])', x)[0]
        if date in window_success_list:
            chinese_success_list.append(re.sub(r'(?<=\()([\w\W]+)(?=\))', "http://kernelhub.ascotbe.com/Windows/Docs/#/CN/"+date, x))
            english_success_list.append(re.sub(r'(?<=\()([\w\W]+)(?=\))', "http://kernelhub.ascotbe.com/Windows/EnglishDocs/#/EN/"+date, x))
        if date in window_failure_list:
            chinese_failure_list.append(re.sub(r'(?<=\()([\w\W]+)(?=\))', "http://kernelhub.ascotbe.com/Windows/Docs/#/CN/"+date, x))
            english_failure_list.append(re.sub(r'(?<=\()([\w\W]+)(?=\))', "http://kernelhub.ascotbe.com/Windows/EnglishDocs/#/EN/"+date, x))
    chinese_success_date=common_vulnerabilities_and_exposures_list[0]+"\n"+common_vulnerabilities_and_exposures_list[1]+"\n"
    chinese_failure_date=common_vulnerabilities_and_exposures_list[0]+"\n"+common_vulnerabilities_and_exposures_list[1]+"\n"
    english_success_date=common_vulnerabilities_and_exposures_list[0]+"\n"+common_vulnerabilities_and_exposures_list[1]+"\n"
    english_failure_date=common_vulnerabilities_and_exposures_list[0]+"\n"+common_vulnerabilities_and_exposures_list[1]+"\n"
    for x1 in chinese_success_list:
        chinese_success_date+=x1+"\n"
    for x2 in chinese_failure_list:
        chinese_failure_date+=x2+"\n"
    for x3 in english_success_list:
        english_success_date+=x3+"\n"
    for x4 in english_failure_list:
        english_failure_date+=x4+"\n"

    chinese_success_file=open(Path+"/docs/Windows/Docs/TestSuccess.md","w+")
    chinese_success_file.write(chinese_success_date)
    chinese_success_file.close()
    chinese_failure_file=open(Path+"/docs/Windows/Docs/TestFailure.md","w+")
    chinese_failure_file.write(chinese_failure_date)
    chinese_failure_file.close()
    english_success_file=open(Path+"/docs/Windows/EnglishDocs/TestSuccess.md","w+")
    english_success_file.write(english_success_date)
    english_success_file.close()
    english_failure_file=open(Path+"/docs/Windows/EnglishDocs/TestFailure.md","w+")
    english_failure_file.write(english_failure_date)
    english_failure_file.close()
    



def HomeMarkdown():
    EN_File_R = open(Path + "/Windows/README.md", "r").read()
    CN_File_R = open(Path + "/Windows/README.CN.md", "r").read()
    Header="\n\n| SecurityBulletin |   |   |   |\n| ---------------- |---------------- | ---------------- | ---------------- |\n"
    CVE=""
    x=0
    for i in sorted(window_failure_list):
        x+=1
        CVE += "| " + str(i)
        if x>4:
            Header+=CVE+"|\n"
            CVE=""
            x=0
    if x!=0:
        Header+=str(CVE)+"   |\n"
    EN_Date = re.sub(r'(?<=and welcome to submit PR)([\w\W]+)(?=### Disclaimer)', Header+"\n\n",EN_File_R)
    CN_Date = re.sub(r'(?<=欢迎提交PR)([\w\W]+)(?=### 免责声明)', Header+"\n\n",CN_File_R)
    EN_File_W = open(Path + "/Windows/README.md", "w+")
    CN_File_W = open(Path + "/Windows/README.CN.md", "w+")
    EN_File_W.write(EN_Date)
    CN_File_W.write(CN_Date)



if __name__ == '__main__':
    WindowsConfig()
    WindowsMarkdown()
    # HomeMarkdown()