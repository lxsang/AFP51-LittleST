Class ROSTopic Object id calledid name type uri port
Methods ROSTopic 'all'
callerid
    ^ calledid asString

|
type
    ^type asString

|
name
    ^ name asString

|
uri
    ^ uri asString

|
port
    ^ port asInteger
|
publish:data
	^(<154 9 id data>)
]
