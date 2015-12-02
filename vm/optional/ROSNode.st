Class ROSNode Object name muri mport
Methods ROSNode 'all'
new 
 ^(<154 0>)
|
master:uri at:port
    <154 2 uri port>
|
masterPort
    ^ mport asInteger
|
masterUri
    ^ muri asString
|
name 
    ^ name asString
|
subscribeTo:aTopic type:aType do:aBlock |proc|
	^(<154 3 aTopic aType aBlock>)
|
deploy
	^ (<154 5>)
|
rosOk
	^ (<154 7>)
|
spin
	[self rosOk] whileTrue:[]
|
rosToggle:aBool
	<154 8 aBool>
|
name:aStr
    <154 1 aStr>
|
createPublisher:name type:type
	^(<154 4 name type>)
]
