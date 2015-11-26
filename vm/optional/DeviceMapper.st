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
    ^ (<151 6 devno idx value>)

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
    <151 1 devno data>

|
int32At:idx
    ^(<151 4 devno idx>)

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
    devno <- (<151 0 path size>).
    devno isNil ifTrue:[
        self close.
        Smalltalk error:'Cannot map the device', (self path).
        ^false
    ].
    "map to array"
    data <- (<151 2 devno size>).
    data isNil ifTrue:[
        self close.
        Smalltalk error:'Cannot create ByteArray object'.
        ^false
    ].
    ^true

|
int16At:idx
    ^(<151 3 devno idx>)

|
at:idx put:value
    self data isNil ifTrue:[^self].
    ^ self data basicAt:idx put:value

|
at:idx put16:value
    self data isNil ifTrue:[^self].
    ^ (<151 5 devno idx value>)

]
