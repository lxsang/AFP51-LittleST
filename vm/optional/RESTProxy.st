Class RESTProxy Object client request
Class VMProxy RESTProxy
"Defined methods for REST proxy"
Methods RESTProxy 'all'
client
    ^client
|
request
    ^ request
|
response:obj
    ^(<155 0 (self client) obj>)
|
handle
    ^self response:'This class need to be subclassed'
]
"Defined methods for VMProxy"
Methods VMProxy 'all'
handle |m args|
    m <- self request at:#uri ifAbsent: ['test'].
    m print.
    m<- self class methods at:m asSymbol.
    args <- Array new:1.
    args at:1 put:self.
    self response:(m  executeWith: args)
|
test
    'some thing here' print
|
default
    ^(imgMeta allSystemClasses)
]