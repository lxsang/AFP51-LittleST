#ifndef RESTPRIV_H
#define RESTPRIV_H

// the plugin library
#include "plugin.h"
#include "../env.h"
#include "../memory.h"
#include "../names.h"

#define REST_JSON_WR 0
#define REST_REQUEST_HANDLER 1

object REST_priv(object*);
rest_json_write(object,object);
#endif