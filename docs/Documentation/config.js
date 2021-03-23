/*

f = open("1.txt","r")
lines = f.readlines()
for line in lines:
    print ("{\n        path: \'/CN/"+line.strip("\n")+"\', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/"+line.strip("\n")+"/README.md\'\n},")
*/
let config = {
    title: 'Kernelhub',
    home: '',
    repo: 'Ascotbe/Kernelhub',
    nav: [
        {
            title: '简介', path: '/',source:'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/README.CN.md',
        },
        {
            title: '目录导航',type: 'dropdown', items: [
                {
                    title: "目录",path: '/Home'
                },
                {
                        path: '/CN/CVE-2003-0352', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2003-0352/README.md'
                },
                {
                        path: '/CN/CVE-2005-1983', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2005-1983/README.md'
                },
                {
                        path: '/CN/CVE-2006-3439', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2006-3439/README.md'
                },
                {
                        path: '/CN/CVE-2008-1084', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2008-1084/README.md'
                },
                {
                        path: '/CN/CVE-2008-3464', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2008-3464/README.md'
                },
                {
                        path: '/CN/CVE-2008-4037', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2008-4037/README.md'
                },
                {
                        path: '/CN/CVE-2008-4250', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2008-4250/README.md'
                },
                {
                        path: '/CN/CVE-2009-0079', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2009-0079/README.md'
                },
                {
                        path: '/CN/CVE-2009-2532', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2009-2532/README.md'
                },
                {
                        path: '/CN/CVE-2010-0020', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2010-0020/README.md'
                },
                {
                        path: '/CN/CVE-2010-0233', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2010-0233/README.md'
                },
                {
                        path: '/CN/CVE-2010-0270', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2010-0270/README.md'
                },
                {
                        path: '/CN/CVE-2010-1897', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2010-1897/README.md'
                },
                {
                        path: '/CN/CVE-2010-2554', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2010-2554/README.md'
                },
                {
                        path: '/CN/CVE-2010-3338', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2010-3338/README.md'
                },
                {
                        path: '/CN/CVE-2010-4398', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2010-4398/README.md'
                },
                {
                        path: '/CN/CVE-2011-0045', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2011-0045/README.md'
                },
                {
                        path: '/CN/CVE-2011-1249', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2011-1249/README.md'
                },
                {
                        path: '/CN/CVE-2011-1974', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2011-1974/README.md'
                },
                {
                        path: '/CN/CVE-2011-2005', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2011-2005/README.md'
                },
                {
                        path: '/CN/CVE-2012-0002', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2012-0002/README.md'
                },
                {
                        path: '/CN/CVE-2012-0217', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2012-0217/README.md'
                },
                {
                        path: '/CN/CVE-2013-0008', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2013-0008/README.md'
                },
                {
                        path: '/CN/CVE-2013-1332', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2013-1332/README.md'
                },
                {
                        path: '/CN/CVE-2013-1345', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2013-1345/README.md'
                },
                {
                        path: '/CN/CVE-2013-5065', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2013-5065/README.md'
                },
                {
                        path: '/CN/CVE-2014-1767', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2014-1767/README.md'
                },
                {
                        path: '/CN/CVE-2014-4076', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2014-4076/README.md'
                },
                {
                        path: '/CN/CVE-2014-4113', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2014-4113/README.md'
                },
                {
                        path: '/CN/CVE-2014-6321', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2014-6321/README.md'
                },
                {
                        path: '/CN/CVE-2014-6324', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2014-6324/README.md'
                },
                {
                        path: '/CN/CVE-2015-0002', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2015-0002/README.md'
                },
                {
                        path: '/CN/CVE-2015-0003', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2015-0003/README.md'
                },
                {
                        path: '/CN/CVE-2015-0057', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2015-0057/README.md'
                },
                {
                        path: '/CN/CVE-2015-0062', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2015-0062/README.md'
                },
                {
                        path: '/CN/CVE-2015-1701', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2015-1701/README.md'
                },
                {
                        path: '/CN/CVE-2015-1725', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2015-1725/README.md'
                },
                {
                        path: '/CN/CVE-2015-2370', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2015-2370/README.md'
                },
                {
                        path: '/CN/CVE-2015-2387', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2015-2387/README.md'
                },
                {
                        path: '/CN/CVE-2015-2546', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2015-2546/README.md'
                },
                {
                        path: '/CN/CVE-2016-0041', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2016-0041/README.md'
                },
                {
                        path: '/CN/CVE-2016-0051', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2016-0051/README.md'
                },
                {
                        path: '/CN/CVE-2016-0095', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2016-0095/README.md'
                },
                {
                        path: '/CN/CVE-2016-0099', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2016-0099/README.md'
                },
                {
                        path: '/CN/CVE-2016-3225', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2016-3225/README.md'
                },
                {
                        path: '/CN/CVE-2016-3309', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2016-3309/README.md'
                },
                {
                        path: '/CN/CVE-2016-3371', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2016-3371/README.md'
                },
                {
                        path: '/CN/CVE-2016-7255', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2016-7255/README.md'
                },
                {
                        path: '/CN/CVE-2017-0101', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2017-0101/README.md'
                },
                {
                        path: '/CN/CVE-2017-0143', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2017-0143/README.md'
                },
                {
                        path: '/CN/CVE-2017-0213', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2017-0213/README.md'
                },
                {
                        path: '/CN/CVE-2017-8464', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2017-8464/README.md'
                },
                {
                        path: '/CN/CVE-2018-0743', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2018-0743/README.md'
                },
                {
                        path: '/CN/CVE-2018-0833', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2018-0833/README.md'
                },
                {
                        path: '/CN/CVE-2018-1038', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2018-1038/README.md'
                },
                {
                        path: '/CN/CVE-2018-8120', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2018-8120/README.md'
                },
                {
                        path: '/CN/CVE-2018-8440', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2018-8440/README.md'
                },
                {
                        path: '/CN/CVE-2018-8453', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2018-8453/README.md'
                },
                {
                        path: '/CN/CVE-2018-8639', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2018-8639/README.md'
                },
                {
                        path: '/CN/CVE-2019-0803', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2019-0803/README.md'
                },
                {
                        path: '/CN/CVE-2019-0859', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2019-0859/README.md'
                },
                {
                        path: '/CN/CVE-2019-1388', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2019-1388/README.md'
                },
                {
                        path: '/CN/CVE-2019-1458', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2019-1458/README.md'
                },
                {
                        path: '/CN/CVE-2020-0787', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2020-0787/README.md'
                },
                {
                        path: '/CN/CVE-2020-0796', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2020-0796/README.md'
                },
                {
                        path: '/CN/CVE-2020-1472', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2020-1472/README.md'
                },
                {
                        path: '/CN/CVE-2020-16898', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2020-16898/README.md'
                },
                {
                        path: '/CN/CVE-2020-16938', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2020-16938/README.md'
                },
                {
                        path: '/CN/CVE-2020-17087', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2020-17087/README.md'
                },
                {
                        path: '/CN/CVE-2021-1709', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2021-1709/README.md'
                },
                {
                        path: '/CN/CVE-2021-1732', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/CVE-2021-1732/README.md'
                },
            ]
        },
    ],
    tocVisibleDepth: 10,
    plugins: []
};
