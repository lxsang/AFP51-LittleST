Methods String 'all'
	back
		self size = 0 ifTrue:[^self].
		^ self copyFrom:1 to: (self size - 1)
]
Methods Integer 'all'
wait
	<153 1 self>
|
miliseconds
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