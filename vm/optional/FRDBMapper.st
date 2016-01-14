Class FRDBMapper APF51Mapper
Methods FRDBMapper 'all'
signature 
 ^ self int16At:21
|
clkcount
    ^self int32At:25
|
start 
    self at:15 put16:1
|
id 
 ^ self int16At:23
|
res:value 
    self at:13 put16:value 
|
din:value
	self at:17 put32:value
]
