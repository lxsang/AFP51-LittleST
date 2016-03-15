Methods String 'all'
	back
		self size = 0 ifTrue:[^self].
		^ self copyFrom:1 to: (self size - 1)
|
	dataAsJSON
		^ '"',self,'"'
]
Methods Integer 'all'
wait
	<153 1 self>
|
dataAsJSON
	^ self asString
|
milliseconds
	^ self
|
seconds
	^ (self * 1000)
|
minutes
	^ (self*60) seconds
]
Methods Scheduler 'all'
	runOne
		" run as long as process list is non empty "
		[ processList size > 0] whileTrue: 
			[
				processList do: 
				[ :x | currentProcess <- x.
					    x execute  ]]
]
* serialize ojbect to JSON data
* this helpful for remote control
Methods Object 'all'
	asJSON
		^ ('{"class":"',(self class asString),'", "values":',(self dataAsJSON),'}')
|
	dataAsJSON |str i|
		self class instanceSize = 0 ifTrue:[^'""'].
		str <- '{'.
		i <- 1.
		self class variables do:[:e|
			str <- str,'"', (e asString),'":', (self basicAt:i) asJSON, ','.
			i <- i + 1
		].
		str <- (str back),'}'.
		^ str
]
Methods Array 'all'
	dataAsJSON |str|
		str <- '['.
		self do:[:e|str <- str, (e asJSON),','].
		str <- (str back),']'.
		^ str
]
Methods Boolean 'all'
	dataAsJSON
		^ self asString
]
Methods UndefinedObject 'all'
	webGlobal 
		stderr <- File new; name: 'stdout'; mode: 'w'; open. 
		stdout <- File new; name: 'stdout'; mode: 'w'; open. 
    	sysTmp <- ''. 
		scheduler <- Scheduler new.
		imgMeta <- ImageManager new.
		sysPortal <- nil.
		gwId <- Dictionary new.
		webProcess <- [scheduler runOne] newProcess.
|
	dataAsJSON
		^ 'nil'
]