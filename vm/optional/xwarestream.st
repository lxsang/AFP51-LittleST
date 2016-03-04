Class TestSynObj Date
Methods TestSynObj 'all'
yes
	
	^ self printString
|
stream:aBlock 
	
	[ true ] whileTrue: [ 
		aBlock value:self data.
		50 milliseconds wait
	 ]
|
add:a with:b
	
	^ a+b
]
