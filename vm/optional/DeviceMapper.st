Class DeviceMapper Object path devno size data
Methods DeviceMapper 'all'

devno
    ^devno

|
size:aNumber
    size <- aNumber

|
at:idx
    self data isNil ifTrue:[^nil].
    ^ self data basicAt:idx

|
at:idx put32:value
    self data isNil ifTrue:[^self].
    ^ (<152 6 devno idx value>)

|
data
    ^data

|
size
    ^size

|
bytesDo:aBlock
    self data isNil ifFalse:[
        self data do:aBlock
    ]

|
close
    <152 1 devno data>

|
int32At:idx
    ^(<152 4 devno idx>)

|
path:aFile
    path <- aFile

|
path
    ^path

|
open
    "connect to the device"
    self path isNil ifTrue:[
        Smalltalk error:'No dev file specified'.
        ^false
    ].
    devno <- (<152 0 path size>).
    devno isNil ifTrue:[
        self close.
        Smalltalk error:'Cannot map the device', (self path).
        ^false
    ].
    "map to array"
    data <- (<152 2 devno size>).
    data isNil ifTrue:[
        self close.
        Smalltalk error:'Cannot create ByteArray object'.
        ^false
    ].
    ^true

|
int16At:idx
    ^(<152 3 devno idx>)

|
at:idx put:value
    self data isNil ifTrue:[^self].
    ^ self data basicAt:idx put:value

|
at:idx put16:value
    self data isNil ifTrue:[^self].
    ^ (<152 5 devno idx value>)
|
intAt:idx wsize:size
	^(<152 7 devno idx size>)
]
