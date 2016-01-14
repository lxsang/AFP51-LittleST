Class ODMapper APF51Mapper
Methods ODMapper 'all'
id
    ^ self int16At:21



|

signature
    ^ self int16At:19



|

x 
    ^ (self sumx/ self sumn) ceiling

|

resolution:res
    ^ self at:15 put16:res


|

sumx
    ^ self intAt:25 wsize:self wsize


|

sumn
    ^ self intAt:33 wsize:self wsize



|

y
    ^ (self sumy/ self sumn) ceiling

|

sumy
    ^ self intAt:29 wsize:self wsize



]
