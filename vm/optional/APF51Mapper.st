Class APF51Mapper DeviceMapper amap
Methods APF51Mapper 'all'
new
	super new.
    self path:'/dev/uio0'.
    self size:65536.
    self open.
|

pendingIRQ
    ^ self int16At:11



|


wsize
    ^16


|


dsize
    ^16


|

mask:v 
    ^ self at:3 put16:v



|


asize
    ^16


|

ack:v
    ^ self at:1 put16:v



|

mask
    ^self int16At:9



|
id
    ^ 0


|
waitForIRQ:aBlock
    [self pendingIRQ == 0] whileTrue:[].
    aBlock value.
    self ack:self id.


]
