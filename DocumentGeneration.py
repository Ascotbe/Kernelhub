# !/usr/bin/env python
# -*- coding: utf-8 -*-
import os 
import re
Path="/Users/ascotbe/code/Kernelhub"
SuccessList=[]
FailureList=[]
def Config():
    CNBegin="""let config = {\n    title: 'Kernelhub',\n    home: 'Info.md',\n    repo: 'Ascotbe/Kernelhub',\n    nav: [\n        {\n            title: '简介', path: '/'\n        },\n        {\n            title: '目录导航',type: 'dropdown', items: [\n                {\n                    title: "有利用脚本(测试成功)",path: '/TestSuccess'\n                },"""
    End="""\n            ]\n        },\n    ],\n    tocVisibleDepth: 10,\n    plugins: []\n};"""
    RejectList=['.DS_Store', '.git','.idea', '.gitignore','LICENSE', 'Patch', 'README.CN.md', 'README.md', 'TestFailure', 'docs', 'DocumentGeneration.py']
    ENBegin="""let config = {\n    title: 'Kernelhub',\n    home: 'Info.md',\n    repo: 'Ascotbe/Kernelhub',\n    nav: [\n        {\n            title: 'Introduction', path: '/'\n        },\n        {\n            title: 'DirectoryNavigation',type: 'dropdown', items: [\n                {\n                    title: "exploit script (successful test)",path: '/TestSuccess'\n                },"""
    #处理成功的编号
    for i in os.listdir(Path):
        if i not in RejectList:
            SuccessList.append(i)
    for x in sorted(SuccessList):
        CNBegin+="""\n                {\n                        path: '/CN/"""+x+"""', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/"""+x+"""/README.md'\n                },"""
        ENBegin+="""\n                {\n                        path: '/EN/"""+x+"""', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/"""+x+"""/README_EN.md'\n                },"""
    #处理失败的编号
    CNBegin+="""\n                {\n                        title: '有利用脚本(测试未成功)', path: '/TestFailure'\n                },"""
    ENBegin+="""\n                {\n                        title: 'exploit script (test failure)', path: '/TestFailure'\n                },"""
    for i in os.listdir(Path+"/TestFailure"):
        if i not in RejectList:
            FailureList.append(i)
    for xx in sorted(FailureList):
        CNBegin+="""\n                {\n                        path: '/CN/"""+xx+"""', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/TestFailure/"""+xx+"""/README.md'\n                },"""
        ENBegin+="""\n                {\n                        path: '/EN/"""+xx+"""', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/TestFailure/"""+xx+"""/README_EN.md'\n                },"""
    CNBegin+=End
    ENBegin+=End
    cn_file=open(Path+"/docs/Docs/config.js","w+")
    cn_file.write(CNBegin)
    cn_file.close()
    en_file=open(Path+"/docs/EnglishDocs/config.js","w+")
    en_file.write(ENBegin)
    en_file.close()


def Markdown():
    CVEList=[]
    CNMarkdownSuccessList=[]
    CNMarkdownFailureList=[]
    ENMarkdownSuccessList=[]
    ENMarkdownFailureList=[]
    file=open(Path+"/README.md","r").read()
    date = re.findall(r'(?<=> Numbered list)([\w\W]+)(?=### Required environment)', file)[0].split("\n")
    for i in date:
        if i != "":
            CVEList.append(i)
    for x in CVEList[2:]:
        date = re.findall(r'(?<=\[)([\w\W]+)(?=\])', x)[0]
        if date in SuccessList:
            CNMarkdownSuccessList.append(re.sub(r'(?<=\()([\w\W]+)(?=\))', "http://kernelhub.ascotbe.com/Docs/#/CN/"+date, x))
            ENMarkdownSuccessList.append(re.sub(r'(?<=\()([\w\W]+)(?=\))', "http://kernelhub.ascotbe.com/EnglishDocs/#/EN/"+date, x))
        if date in FailureList:
            CNMarkdownFailureList.append(re.sub(r'(?<=\()([\w\W]+)(?=\))', "http://kernelhub.ascotbe.com/Docs/#/CN/"+date, x))
            ENMarkdownFailureList.append(re.sub(r'(?<=\()([\w\W]+)(?=\))', "http://kernelhub.ascotbe.com/EnglishDocs/#/EN/"+date, x))
    CNMarkdownSuccessDate=CVEList[0]+"\n"+CVEList[1]+"\n"
    CNMarkdownFailureDate=CVEList[0]+"\n"+CVEList[1]+"\n"
    ENMarkdownSuccessDate=CVEList[0]+"\n"+CVEList[1]+"\n"
    ENMarkdownFailureDate=CVEList[0]+"\n"+CVEList[1]+"\n"
    for x1 in CNMarkdownSuccessList:
        CNMarkdownSuccessDate+=x1+"\n"
    for x2 in CNMarkdownFailureList:
        CNMarkdownFailureDate+=x2+"\n"
    for x3 in ENMarkdownSuccessList:
        ENMarkdownSuccessDate+=x3+"\n"
    for x4 in ENMarkdownFailureList:
        ENMarkdownFailureDate+=x4+"\n"

    CNTestSuccess=open(Path+"/docs/Docs/TestSuccess.md","w+")
    CNTestSuccess.write(CNMarkdownSuccessDate)
    CNTestSuccess.close()
    CNTestFailure=open(Path+"/docs/Docs/TestFailure.md","w+")
    CNTestFailure.write(CNMarkdownFailureDate)
    CNTestFailure.close()
    ENTestSuccess=open(Path+"/docs/EnglishDocs/TestSuccess.md","w+")
    ENTestSuccess.write(ENMarkdownSuccessDate)
    ENTestSuccess.close()
    ENTestFailure=open(Path+"/docs/EnglishDocs/TestFailure.md","w+")
    ENTestFailure.write(ENMarkdownFailureDate)
    ENTestFailure.close()
    



def HomeMarkdown():
    EN_File_R = open(Path + "/README.md", "r").read()
    CN_File_R = open(Path + "/README.CN.md", "r").read()
    Header="\n\n| SecurityBulletin |   |   |   |\n| ---------------- |---------------- | ---------------- | ---------------- |\n"
    CVE=""
    x=0
    for i in sorted(FailureList):
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
    EN_File_W = open(Path + "/README.md", "w+")
    CN_File_W = open(Path + "/README.CN.md", "w+")
    EN_File_W.write(EN_Date)
    CN_File_W.write(CN_Date)



if __name__ == '__main__':
    Config()
    Markdown()
    HomeMarkdown()
