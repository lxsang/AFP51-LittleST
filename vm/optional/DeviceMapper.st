Class DeviceMapper Object path devno size data devfile rflow wflow
Methods DeviceMapper 'all'

int32At:idx
    ^(<152 4 devno idx>)


|
new 
    wflow <- Array new:4.
	rflow <- Array new:4.
    self wflowConfig.
	self rflowConfig.
|

wflow:arr
	wflow <- arr

|
rflow:arr
	rflow <- arr
|
rflow
	^ rflow
|
at:idx
    self data isNil ifTrue:[^nil].
    ^ self data basicAt:idx


|

data
    ^data
|
driver
	^ 'obtrkn'
|
loadGateware: path
	^ <152 12 path (self driver)>
|
wflowConfig
    ^self
|
rflowConfig
	^ self
|
size
    ^size


|

bytesDo:aBlock
    self data isNil ifFalse:[
        self data do:aBlock
    ]


|

at:idx put16:value
    self data isNil ifTrue:[^self].
    ^ (<152 5 devno idx value>)

|

at:idx put:value wsize:size
	^(<152 9 devno idx value size>)

|

intAt:idx wsize:size
	^(<152 7 devno idx size 4>)

|

at:idx put32:value
    self data isNil ifTrue:[^self].
    ^ (<152 6 devno idx value>)


|

close
    <152 1 devno data devfile>


|

size:aNumber
    size <- aNumber


|

int16At:idx
    ^(<152 3 devno idx>)


|

path
    ^path
|
writeArray:anObj
	^ (<152 10 devno (self wflow) anObj>)
|
readArrayOfInt16:object
	^(<152 11 devno (self rflow) object>)
|
open |arr|
    "connect to the device"
    self path isNil ifTrue:[
        Smalltalk error:'No dev file specified'.
        ^false
    ].
    arr <- (<152 0 path size>).
    arr isNil ifTrue:[
        self close.
        Smalltalk error:'Cannot map the device', (self path).
        ^false
    ].
	devfile <- arr at:1.
	devno <- arr at:2.
    "map to array"
    data <- (<152 2 devno size>).
    data isNil ifTrue:[
        self close.
        Smalltalk error:'Cannot create ByteArray object'.
        ^false
    ].
    ^true


|

waitForIRQ:aBlock |irq|
	irq <- (<152 8>).
	irq ifTrue:[aBlock value].

|

wflow
	^wflow

|

at:idx put:value
    self data isNil ifTrue:[^self].
    ^ self data basicAt:idx put:value


|

devno
    ^devno


|

path:aFile
    path <- aFile


]
