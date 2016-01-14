Class SCMapper APF51Mapper
Methods SCMapper 'all'
break 
    ^ self int16At:33
|
dbTarget:aSymbol 
    self at:17 put16:38

|
signature 
 ^ self int16At:29
|
clkcount
    ^self int32At:35
|
dbOpr:s
    s = #= ifTrue:[^ self at:19 put16:2].
    s = #< ifTrue:[^ self at:19 put16:0].
    s = #> ifTrue:[^ self at:19 put16:1].
    s = #~= ifTrue:[^ self at:19 put16:3]
|
start 
    self at:23 put16:1
|
id 
 ^ self int16At:31
|
resume
    self at:21 put16:3
|
din:value 
    self at:25 put32:value 
|
dout
    ^ self int32At:39
|
dbOpd1:value
   self at:13 put32:value



|
active
    self at:21 put16:1
]
