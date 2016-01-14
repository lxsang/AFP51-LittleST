Class RWTest APF51Mapper
Methods RWTest 'all'
signature 
 ^ self int16At:19
|
we
	self at:13 put16:1
|
at:idx 
    self at:15 put16:idx-1.
    ^self int16At:23
|
id 
 ^ self int16At:21
|
at: idx put:value
    self at:15 put16:idx-1.
	self at:17 put16:value.
	self we.
|
wflowConfig | rgs we|
   rgs <- Array new:1.
   we <- Array new:2.
   we at:1 put:12.
   we at:2 put:1.
   rgs at:1 put:we.
   self wflow at:1 put:16.
   self wflow at:2 put:rgs.
   self wflow at:3 put:16.
   self wflow at:4 put:14.
|
rflowConfig
   self rflow at:1 put:22.
   self rflow at:3 put:16.
   self rflow at:4 put:14.
]
