#ifndef ROSPRIV_H
#define ROSPRIV_H

#include "stros.h"
#include "pointerpriv.h"
#include "../env.h"
#include "../memory.h"
#include "../names.h"

#define ROS_PRIV_INIT 		0
#define ROS_PRIV_NAME 		1
#define ROS_PRIV_MASTER 	2
#define ROS_PRIV_SUB		3
#define ROS_PRIV_PUB_INIT 	4
#define ROS_PRIV_DEPLOY		5
#define ROS_PRIV_STOP		6
#define ROS_PRIV_OK			7
#define ROS_PRIV_TOGG		8
#define ROS_PRIV_PUB		9

#define ROS_CLS_NODE	"ROSNode"
#define ROS_CLS_TOPIC	"ROSTopic" //5 vars

object rospriv(object*);
object rospriv_init();
object rospriv_setname(object);
object rospriv_master(object,object);
object rospriv_sub(object,object,object);
object rospriv_deploy();
void rospriv_subscribe_callback(void*, topic_t*);
object rospriv_create_pub(object, object);
object rospriv_node_pub(object,object);
extern pthread_mutex_t exec_mux;
#endif