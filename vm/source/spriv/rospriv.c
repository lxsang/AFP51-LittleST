#include "rospriv.h"

object rospriv(object* args)
{
	int type = args[0]>>1;
	switch(type)
	{
		case ROS_PRIV_INIT:
			return rospriv_init();
		case ROS_PRIV_NAME:
			return rospriv_setname(args[1]);
		case ROS_PRIV_MASTER:
			return rospriv_master(args[1], args[2]);
		case ROS_PRIV_SUB:
			return rospriv_sub(args[1], args[2], args[3]);
		case ROS_PRIV_PUB_INIT:
			return rospriv_create_pub(args[1],args[2]);
		case ROS_PRIV_PUB:
			return rospriv_node_pub(args[1],args[2]);
		case ROS_PRIV_DEPLOY:
			return rospriv_deploy();
		case ROS_PRIV_STOP:
			stros_toggle(1);
			return nilobj;
		case ROS_PRIV_OK:
			return stros_ok()?trueobj:falseobj;
		case ROS_PRIV_TOGG:
			stros_toggle(args[1]== trueobj?1:0);
			return nilobj;
		default: return nilobj;
	}
}
object rospriv_init()
{
	if(!xml_prc_server.node)
	{
		xml_prc_server.node = malloc(sizeof(ros_node_t));
		xml_prc_server.node->id = "sm_node";
		xml_prc_server.node->master_uri = DEFAULT_MASTER_URI;
		xml_prc_server.node->master_port = DEFAULT_MASTER_PORT;
		xml_prc_server.node->subscribers = dict();
		xml_prc_server.node->publishers = dict();
	}
	// create new wrapper object
	object newObj = allocObject(3);
	setClass(newObj, globalSymbol(ROS_CLS_NODE));
	basicAtPut(newObj,1, newPointer(&xml_prc_server.node->id));
	basicAtPut(newObj,2, newPointer(&xml_prc_server.node->master_uri));
	basicAtPut(newObj,3, newPointer(&xml_prc_server.node->master_port));
	return newObj;
}
object rospriv_create_pub(object name, object type)
{
	publisher* pub;
	object newObj = nilobj;
	if(xml_prc_server.node)
	{
		pub =  create_publisher(xml_prc_server.node, charPtr(name), charPtr(type));
		newObj = allocObject(6);
		setClass(newObj, globalSymbol(ROS_CLS_TOPIC));
		basicAtPut(newObj,1,newPointer(pub));
		basicAtPut(newObj,2,newPointer(&pub->callerid));
		basicAtPut(newObj,3,newPointer(&pub->topic));
		basicAtPut(newObj,4,newPointer(&pub->type));
		basicAtPut(newObj,5,newPointer(&pub->uri));
		basicAtPut(newObj,6,newPointer(&pub->port));
	}
	return newObj;
}
object rospriv_node_pub(object id,object data)
{
	// must verify data type here
	publisher* pub = (publisher*)sysMemPtr(id);
	if(!pub) return falseobj;
	void* rawdata;
	int thash = hash(pub->type,HASHSIZE);
	switch(thash)
	{
		case  TYPE_STRING : 
			rawdata = (void*) strdup(charPtr(data));
			break;
		// there will be more here
		default: 
			rawdata = (void*) sysMemPtr(data);
			break;
	}
	publish(pub,rawdata);
	return trueobj;
}
object rospriv_setname(object name)
{
	if(name == nilobj) return falseobj;
	xml_prc_server.node->id = strdup(charPtr(name));
	return trueobj;
}
object rospriv_master(object uri,object port)
{
	if(uri != nilobj)
		xml_prc_server.node->master_uri = strdup(charPtr(uri));
	if(port != nilobj)
		*(&xml_prc_server.node->master_port) = port>>1;
	return nilobj;
}
object rospriv_sub(object tp,object type,object callback)
{ 
	if(!xml_prc_server.node) return falseobj;
	char* topic = strdup(charPtr(tp));
	subscriber* sub = malloc(sizeof(subscriber));
	sub->callerid = xml_prc_server.node->id;
	sub->topic = topic;
	sub->type = strdup(charPtr(type));
	sub->status = TOPIC_DOWN;
	sub->uri = NULL;
	sub->port = 0;
	sub->data = (void*)callback;
	incr(callback);
	sub->handler = rospriv_subscribe_callback;
	dput(xml_prc_server.node->subscribers,topic,(void*) sub);
	return trueobj;
}
object rospriv_deploy()
{
	if(xml_prc_server.node)
	{
		stros_node_deploy(xml_prc_server.node);
		return trueobj;
	}
	return falseobj;
}
void dump_block(object b)
{
	printf("Context %d\n", basicAt(b,1));
	printf("args %d\n", basicAt(b,2));
	printf("argloc %d\n", basicAt(b,3));
	printf("byte ptr %d\n", basicAt(b,3));
}
/*TODO fix this bug*/
void rospriv_subscribe_callback(void* data, topic_t* sub)
{
	if(!sub || !sub->data)
	{
		printf("Callback not found\n");
		return;
	}
	object block = ((object)sub->data);
	if(block == nilobj) 
	{
		printf("The block is nil\n");
		return;
	}
    object process = allocObject(3);
    object stack = allocObject(50);
	setClass(stack, globalSymbol("Array"));
	// find the sheduler
    setClass(process,globalSymbol("Process"));
    basicAtPut(process, 1, stack);
    basicAtPut(process, 2, newInteger(10));
    basicAtPut(process, 3, newInteger(2));

    /* put argument on stack */
    basicAtPut(stack, 1, nilobj);				/* argument */
    /* now make a linkage area in stack */
    basicAtPut(stack, 2, nilobj);				/* previous link */
    basicAtPut(stack, 3, basicAt(block, 1));	/* context object (nil = stack) */
    basicAtPut(stack, 4, newInteger(1));		/* return point */
    basicAtPut(stack, 5, nilobj);				/* method */
    basicAtPut(stack, 6, basicAt(block,4));		/* byte offset */
	object scheduler = globalSymbol("scheduler");
    if(scheduler != nilobj)
	{
		//pthread_mutex_lock (&exec_mux);
		// create new Link Object
		object lk = newLink(nilobj,process);
		// get the set object from scheduler
		object set = basicAt(scheduler,2);
		object lks = basicAt(set,1);
		// insert the process to the processList
		basicAtPut(lk, 3, lks);
		basicAtPut(set,1, lk);
		//basicAtPut(scheduler,3,process);
		// wait until the process exit
		//pthread_mutex_unlock (&exec_mux);
		lks = basicAt(set,1);
		dump_block(block);
		while(lks != nilobj && basicAt(lks,3) != nilobj)
		{
			lks = basicAt(set,1);
		}
		//printf("%s\n", );
	}
	 printf("reach here\n");
}