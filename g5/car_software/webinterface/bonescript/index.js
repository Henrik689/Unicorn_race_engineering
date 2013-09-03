// Copyright (C) 2011 - Texas Instruments, Jason Kridner 
//
// 
var fs = require('fs');
var child_process = require('child_process');
var http = require('http');
var url = require('url');
var path = require('path');
var events = require('events');

var myrequire = function(packageName, onfail) {
    var y = {};
    try {
        y = require(packageName);
        y.exists = true;
    } catch(ex) {
        y.exists = false;
        //console.log("'" + packageName + "' not loaded");
        //console.log("If desired, try installing it with:");
        //console.log("  curl http://npmjs.org/install.sh | bash");
        //console.log("  npm install " + packageName);
        onfail();
    }
    return(y);
};

var socket = myrequire('socket.io', function() {
    console.log("Dynamic web features not enabled");
});

var binary = myrequire('binary', function() { });
var inotify = myrequire('inotify', function() { });

var fibers = myrequire('fibers', function() {
    //console.log("Delay operations loops will consume CPU cycles");
    //console.log("Invoke using 'node-fibers' if node version < 0.5.2");
});

// Wait for some time
if(fibers.exists) {
    delay = exports.delay = function(milliseconds)
    {
        var fiber = Fiber.current;
        setTimeout(function() {
            fiber.run();
        }, milliseconds);
        yield();
    };
} else {
    delay = exports.delay = function(milliseconds)
    {
        var startTime = new Date().getTime();
        while(new Date().getTime() < startTime + milliseconds) {
        }
    };
}

// This is where everything is meant to happen
if(fibers.exists) {
    run = exports.run = function()
    {
        Fiber(function() {
            var fiber = Fiber.current;
            setup();
            if(typeof loop === "function") {
	            while(true) {
                    loop();
                    setTimeout(function() {
                        fiber.run();
                    }, 0);
		            yield();
		        }
            }
        }).run();
    };
} else {
    run = exports.run = function()
    {
        setup();
        if(typeof loop === "function") {
            process.nextTick(function repeat() {
                loop();
                process.nextTick(repeat);
            });
        }
    };
}

// This is a helper function for web servers
var loadFile = function(uri, subdir, res, type) {
    var filename = path.join(subdir, uri);
    fs.exists(
        filename,
        function(exists) {
            if(!exists) {
                res.writeHead(404, {"Content-Type": "text/plain"});
                res.write("Error 404: '" + uri + "' Not Found\n");
                res.end();
                return;
            }
            if(type == "binary") {
                fs.readFile(
                    filename,
                    "binary",
                    function(err, file) {
                        if(err) {
                            res.writeHead(500, {"Content-Type": "text/plain"});
                            res.write(err + "\n");
                            res.end();
                            return;
                        }
                        res.writeHead(200);
                        res.write(file, "binary");
                        res.end();
                    }
                );
            } else {
                fs.readFile(
                    filename,
                    encoding='utf8',
                    function(err, file) {
                        if(err) {
                            res.writeHead(500, {"Content-Type": "text/plain"});
                            res.write(err + "\n");
                            res.end();
                            return;
                        }
                        res.writeHead(200, {"Content-Type": type});
                        res.write("" + file);
                        res.end();
                    }
                );
            }
        }
    );
};

exports.Server = function(port, subdir, onconnect) {
    subdir = path.join(process.cwd(), subdir);
    var handler = function(req, res) {
        var uri = url.parse(req.url).pathname;
        if(uri == '/') {
            loadFile('index.html', subdir, res, "text/html");
        } else {
            if(uri.match(/\.js$/i)) {
                loadFile(uri, subdir, res, "application/javascript");
            } else if(uri.match(/\.css$/i)) {
                loadFile(uri, subdir, res, "text/css");
            } else if(uri.match(/\.htm(.)$/i)) {
                loadFile(uri, subdir, res, "text/html");
            } else if(uri.match(/\.(jpg|png|ico)$/i)) {
                loadFile(uri, subdir, res, "binary");
            } else {
                loadFile(uri, subdir, res, "text/plain");
            }
        }
    };
    this.server6 = http.createServer();
    this.server6.addListener('request', handler);
    this.server = http.createServer();
    this.server.addListener('request', handler);
    if(socket.exists && (typeof onconnect == 'function')) {
        //var io6 = socket.listen(this.server6);
        //io6.sockets.on('connection', onconnect);
        var io = socket.listen(this.server);
        io.sockets.on('connection', onconnect);
		io.set('log level',1); // Stop logging to terminal
    }
    this.begin = function() {
        //this.server6.listen(port, '::0');
        this.server.listen(port);
        
    };
};
