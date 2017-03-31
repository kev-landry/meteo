
var http = require('http');
var url = require('url');
var querystring = require('querystring');

var server = http.createServer(function(req, res) {
    var params = querystring.parse(url.parse(req.url).query);
    res.writeHead(200, {"Content-Type": "text/plain"});
    console.log('value');
    if ('value' in params) {
        res.write('Vous vous appelez ' + params['value'] + ' ' + params['value2']);
    }
    else {
        res.write('Pas de réponse dans les params');
    }
    res.end();
});
server.listen(8080);
