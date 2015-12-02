Class ImageManager Object

Methods ImageManager 'all'
	allSubClassesOf: aClass |str  clses|
		clses <- aClass subClasses.
		str <- '"',aClass asString,'"'.
		clses size  = 0 ifTrue:[^str].
		^'{',str,':[', (clses inject:'' into:[:x :y|
				x,(self allSubClassesOf:y),',']) back,']}'
		
|
	allSystemClasses
		^ self allSubClassesOf:Object
|
	allMethodsOf:aClass |str|
		^ '[', ((aClass methods keys inject:'' into:[:x :y|
			x,'"',y,'"',',']) back),']'
|		
	addMethodTo:class  |m|
		m <- Method new; text:sysTmp.
		(m compileWithClass: class) ifTrue:[
			class methods at:m name put:m.
			^'{"result":1}'
		] ifFalse:[
			^'{"result":0}'
		]
|
	editMethod:name of:class  |m|
		m <- class methodNamed:name.
		m text:sysTmp.
		(m compileWithClass: class) ifTrue:[
			^'{"result":1}'
		] ifFalse:[
			^'{"result":0}'
		]
|
	sourceOf:aMethod in:aClass
		^ (aClass methods at:(aMethod asSymbol)) text
|
	variablesOf:aClass
		aClass variables isNil ifTrue:[^nil].
		^ '[', ((aClass variables inject:'' into:[:x :y|
			x,'"',y,'"',',']) back),']'
|
	genAllKeywordsOf:list to:f |attr var|
		attr <- Set new.
	    var <- Set new.
		list do:[:each|
	        f print:(',{"value": "',each printString,'", "score": 1000, "meta": "class"}').
	        each methods keys do:[:el| attr add:el asString].
	        each variables notNil ifTrue:[
	        	each variables do:[:el| var add:el]
	        ]].
	    attr do:[:each|
	    	f print:(',{"value": "',each,'", "score": 1000, "meta": "method"}')].
	    var do:[:each| 
	    	f print:(',{"value": "',each,'", "score": 1000, "meta": "variables"}')].
|
	imageKeywords:file |list  f|
	    list <- List new.
	    list add:Object.
	    f <- File new; name: file;mode: 'w';open.
	    f print:('[{"value": "Object", "score": 1000, "meta": "class"}').
	    self dumpAllSubClassesOf:Object to:list.
	    self genAllKeywordsOf:list to:f.
	    f print:']'.
	    f close.
|
	dumpAllSubClassesOf:aClass to:list
	    aClass subClasses do:[:e| 
	        list add:e.
	        self dumpAllSubClassesOf:e to:list
	    ]
|

	exportClass:class to:aFile |tmp f i|
	    f <- File new;name:aFile;mode:'w';open.
	    tmp <- 'Class ', (class name asString),' ', 
	                (class superClass name asString).
	    class variables isNil ifFalse:[
	        class variables do:[:e| tmp <- (tmp,' ', e)]].
	    f print:tmp.
	    tmp <- 'Methods ', class name asString,' ''all'''.
	    f print:tmp.
	    i <- 0.
	    class methods binaryDo:[:k :v| 
	        i = 0 ifFalse:[f print:'|'].
	        i <- i+1.
	        f print: v text
	    ].
	    f print:']'.
	    f close.

]