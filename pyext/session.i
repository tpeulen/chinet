%module(directors="1") chinet

%include "std_set.i";

%{
#include "../include/Session.h"
%}

%shared_ptr(Session)

%include "../include/Session.h"

namespace std{
        %template(set_node) std::set<std::shared_ptr<Node>>;
        %template(set_link) std::set<std::shared_ptr<Link>>;
        %template(set_port) std::set<std::shared_ptr<Port>>;
}
