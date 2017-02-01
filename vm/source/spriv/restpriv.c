#include "restpriv.h"
object REST_priv(object* args)
{
	int type = args[0]>>1;
	switch(type)
	{
		case REST_JSON_WR:
			return rest_json_write(args[1], args[2]);
		default: return nilobj;
	}
}
/*private function, loading string from string object*/
char* load_string(object objptr)
{
	char* data;
	struct objectStruct objs;
	objs = objectTable[objptr>>1];
	if(objs.size < 0)
	{
		char tmp[-objs.size];
		for(int j= 0;j<-objs.size;j++)
		{
			tmp[j] = byteAt(objptr,j+1);
		}
		data = __s("%s",tmp);
		return data;
	} else {
		//printf("class %d\n", objs.size );
		return "Error when loading resource";
	}
}

object rest_json_write(object client, object value)
{
	int cl = intValue(client);
	char *data = load_string(value);
	printf("data %s\n",data);
	json(cl);
	__t(cl,data);

}