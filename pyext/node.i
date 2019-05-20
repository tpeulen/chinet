%module chinet

%include "std_vector.i";
%include "std_map.i";
%include "std_list.i";

%{
#include "../include/Node.h"
#include "../include/MongoObject.h"
#include "../include/Port.h"
%}

%shared_ptr(Node)
%shared_ptr(Port)

%template(MapStringPort) std::map<std::string, std::shared_ptr<Port>>;
%template(MapStringDouble) std::map<std::string, double>;

%include "../include/Node.h"

