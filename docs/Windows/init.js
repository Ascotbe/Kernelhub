var rootUrl = 'https://www.ascotbe.com/kernelhub';

var versions = [
	{ title: '中文', path: '/Docs/' },
	{ title: 'English', path: '/EnglishDocs/' },
]

var latestVersionIndex = 0;
var currentVersionIndex = -1;

versions[latestVersionIndex].title += '';

versions.forEach(function (v, idx) {
    if (v.path) {
        var pathname = window.location.pathname;
        if (pathname.lastIndexOf(v.path) == pathname.length - v.path.length /* pathname.endsWith(v.path) */) {
            currentVersionIndex = idx;
            rootUrl = window.location.origin + pathname.substr(0, pathname.length - v.path.length);
        }
    }
});

versions.forEach(function (v) {
    if (v.path) {
        v.path = rootUrl + v.path;
    }
});

config.nav.push({
    title: 'Language: ' + versions[currentVersionIndex].title, type: 'dropdown', items: versions
});



docute.init(config);

// Google Analytics
window.ga = window.ga || function () { (ga.q = ga.q || []).push(arguments) }; ga.l = +new Date;
ga('create', 'UA-115509121-1', 'auto');
ga('send', 'pageview');
docute.router.afterEach(function (to) {
    ga('set', 'page', to.fullPath);
    ga('send', 'pageview');
});
