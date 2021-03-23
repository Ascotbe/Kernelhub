/*
 * @Author: your name
 * @Date: 2020-05-15 17:48:17
 * @LastEditTime: 2021-03-23 14:03:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /Kernelhub/docs/EnglishDocumentation/config.js
 */
let config = {
    title: 'Kernelhub',
    home: 'Home.md',
    repo: 'Ascotbe/Kernelhub',
    nav: [
        {
            title: 'Introduction',  path: '/',source:'https://raw.githubusercontent.com/Ascotbe/Kernelhub/master/README.md',
        },
        {
            title: 'Directory navigation',type: 'dropdown', items: [
                {
                    title: "table of Contents",path: '/Home'
                },
            ]
        }

    ],
    tocVisibleDepth: 10,
    plugins: []
};
