let config = {
    title: 'Kernelhub',
    home: 'Info.md',
    repo: 'Ascotbe/Kernelhub',
    nav: [
        {
            title: 'Introduction', path: '/'
        },
        {
            title: 'DirectoryNavigation',type: 'dropdown', items: [
                {
                    title: "exploit script (successful test)",path: '/TestSuccess'
                },
                {
                        path: '/EN/.github', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/.github/README_EN.md'
                },
                {
                        path: '/EN/Linux', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/Linux/README_EN.md'
                },
                {
                        path: '/EN/Windows', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/Windows/README_EN.md'
                },
                {
                        title: 'exploit script (test failure)', path: '/TestFailure'
                },
                {
                        path: '/EN/CVE-2000-0979', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2000-0979/README_EN.md'
                },
                {
                        path: '/EN/CVE-2005-1983', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2005-1983/README_EN.md'
                },
                {
                        path: '/EN/CVE-2009-0079', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2009-0079/README_EN.md'
                },
                {
                        path: '/EN/CVE-2010-0020', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2010-0020/README_EN.md'
                },
                {
                        path: '/EN/CVE-2010-2554', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2010-2554/README_EN.md'
                },
                {
                        path: '/EN/CVE-2011-0045', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2011-0045/README_EN.md'
                },
                {
                        path: '/EN/CVE-2011-1237', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2011-1237/README_EN.md'
                },
                {
                        path: '/EN/CVE-2013-0008', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2013-0008/README_EN.md'
                },
                {
                        path: '/EN/CVE-2013-1300', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2013-1300/README_EN.md'
                },
                {
                        path: '/EN/CVE-2013-5065', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2013-5065/README_EN.md'
                },
                {
                        path: '/EN/CVE-2014-6321', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2014-6321/README_EN.md'
                },
                {
                        path: '/EN/CVE-2014-6324', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2014-6324/README_EN.md'
                },
                {
                        path: '/EN/CVE-2015-0002', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2015-0002/README_EN.md'
                },
                {
                        path: '/EN/CVE-2015-0062', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2015-0062/README_EN.md'
                },
                {
                        path: '/EN/CVE-2015-1725', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2015-1725/README_EN.md'
                },
                {
                        path: '/EN/CVE-2016-3309', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2016-3309/README_EN.md'
                },
                {
                        path: '/EN/CVE-2017-0005', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2017-0005/README_EN.md'
                },
                {
                        path: '/EN/CVE-2017-0100', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2017-0100/README_EN.md'
                },
                {
                        path: '/EN/CVE-2017-0263', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2017-0263/README_EN.md'
                },
                {
                        path: '/EN/CVE-2017-11783', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2017-11783/README_EN.md'
                },
                {
                        path: '/EN/CVE-2017-8465', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2017-8465/README_EN.md'
                },
                {
                        path: '/EN/CVE-2018-0824', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2018-0824/README_EN.md'
                },
                {
                        path: '/EN/CVE-2018-0886', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2018-0886/README_EN.md'
                },
                {
                        path: '/EN/CVE-2018-1038', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2018-1038/README_EN.md'
                },
                {
                        path: '/EN/CVE-2018-8440', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2018-8440/README_EN.md'
                },
                {
                        path: '/EN/CVE-2019-0708', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2019-0708/README_EN.md'
                },
                {
                        path: '/EN/CVE-2019-0859', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2019-0859/README_EN.md'
                },
                {
                        path: '/EN/CVE-2019-0863', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2019-0863/README_EN.md'
                },
                {
                        path: '/EN/CVE-2019-0986', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2019-0986/README_EN.md'
                },
                {
                        path: '/EN/CVE-2019-1040', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2019-1040/README_EN.md'
                },
                {
                        path: '/EN/CVE-2019-1215', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2019-1215/README_EN.md'
                },
                {
                        path: '/EN/CVE-2019-1253', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2019-1253/README_EN.md'
                },
                {
                        path: '/EN/CVE-2019-1322', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2019-1322/README_EN.md'
                },
                {
                        path: '/EN/CVE-2019-1422', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2019-1422/README_EN.md'
                },
                {
                        path: '/EN/CVE-2020-0624', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2020-0624/README_EN.md'
                },
                {
                        path: '/EN/CVE-2020-0814', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2020-0814/README_EN.md'
                },
                {
                        path: '/EN/CVE-2020-1350', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2020-1350/README_EN.md'
                },
                {
                        path: '/EN/CVE-2020-1362', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2020-1362/README_EN.md'
                },
                {
                        path: '/EN/CVE-2020-17057', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2020-17057/README_EN.md'
                },
                {
                        path: '/EN/CVE-2020-17087', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2020-17087/README_EN.md'
                },
                {
                        path: '/EN/CVE-2021-1709', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2021-1709/README_EN.md'
                },
                {
                        path: '/EN/CVE-2021-21551', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2021-21551/README_EN.md'
                },
                {
                        path: '/EN/CVE-2021-31166', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2021-31166/README_EN.md'
                },
                {
                        path: '/EN/CVE-2021-34527', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2021-34527/README_EN.md'
                },
                {
                        path: '/EN/CVE-2021-43224', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2021-43224/README_EN.md'
                },
                {
                        path: '/EN/CVE-2021-43883', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2021-43883/README_EN.md'
                },
                {
                        path: '/EN/CVE-2022-26937', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2022-26937/README_EN.md'
                },
                {
                        path: '/EN/CVE-2022-30206', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2022-30206/README_EN.md'
                },
                {
                        path: '/EN/CVE-2022-33679', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2022-33679/README_EN.md'
                },
                {
                        path: '/EN/CVE-2022-34718', source: 'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/Windows/TestFailure/CVE-2022-34718/README_EN.md'
                },
            ]
        },
    ],
    tocVisibleDepth: 10,
    plugins: []
};