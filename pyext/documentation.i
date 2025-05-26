
// File: index.xml

// File: class_mongo_object.xml


%feature("docstring") MongoObject "

The object identification number of the.  

Disconnects the the DB.  

Connects the instance of database.  

The BSON document of the  

Returns
-------  

A BSON document containing the document excluding a set of keys  

Parameters
----------
* `first` :  
    is the first key to exclude.  
* `...` :  
    more keys to exclude  

Returns
-------
a bsoncxx::document::value  

Returns true if the instance of the to the DB  

Writes connected MongoDB  

Returns
-------  

C++ includes: MongoObject.h
";

%feature("docstring") MongoObject::~MongoObject "
MongoObject::~MongoObject";

%feature("docstring") MongoObject::MongoObject "
MongoObject::MongoObject";

%feature("docstring") MongoObject::connect_to_db "
MongoObject::connect_to_db
Parameters
----------
* `uri_string` :  
* `db_string` :  
* `app_string` :  
* `collection_string` :  

Returns
-------
True if connected successfully  
";

%feature("docstring") MongoObject::connect_object_to_db "
MongoObject::connect_object_to_db
Connects other object to the same MongoDB  

templateparam
-------------
* `T` :  

Parameters
----------
* `o` :  
    The object that is connected to the same MongoDB  

Returns
-------
True if connected successfully  
";

%feature("docstring") MongoObject::disconnect_from_db "
MongoObject::disconnect_from_db";

%feature("docstring") MongoObject::is_connected_to_db "
MongoObject::is_connected_to_db";

%feature("docstring") MongoObject::register_instance "
MongoObject::register_instance";

%feature("docstring") MongoObject::unregister_instance "
MongoObject::unregister_instance";

%feature("docstring") MongoObject::write_to_db "
MongoObject::write_to_db";

%feature("docstring") MongoObject::create_copy_in_db "
MongoObject::create_copy_in_db
Creates a copy of the object in the connected MongoDB with a new OID.  

Returns
-------
The OID of the copy  
";

%feature("docstring") MongoObject::read_from_db "
MongoObject::read_from_db
Read the content of an existing BSON document into the current object  

Parameters
----------
* `oid_string` :  
    Object identifier of the queried document in the DB  

Returns
-------
True if successful otherwise false  
";

%feature("docstring") MongoObject::read_json "
MongoObject::read_json
Read the content of a JSON string into a MongoObject  

Parameters
----------
* `json_string` :  

Returns
-------  
";

%feature("docstring") MongoObject::get_own_oid "
MongoObject::get_own_oid
The own object identifier  

Returns
-------  
";

%feature("docstring") MongoObject::set_own_oid "
MongoObject::set_own_oid
Set the own object identifier without duplicate check  

Parameters
----------
* `oid_str` :  
";

%feature("docstring") MongoObject::set_name "
MongoObject::set_name";

%feature("docstring") MongoObject::get_name "
MongoObject::get_name";

%feature("docstring") MongoObject::get_ptr "
MongoObject::get_ptr";

%feature("docstring") MongoObject::set_string "
MongoObject::set_string
Create and / or set a string in the MongoObject accessed by  

Parameters
----------
* `key` :  
* `key` :  
    the key to access the content  
* `str` :  
    The content  
";

%feature("docstring") MongoObject::get_string "
MongoObject::get_string";

%feature("docstring") MongoObject::get_singleton "
MongoObject::get_singleton";

%feature("docstring") MongoObject::set_singleton "
MongoObject::set_singleton";

%feature("docstring") MongoObject::set_oid "
MongoObject::set_oid";

%feature("docstring") MongoObject::get_array "
MongoObject::get_array";

%feature("docstring") MongoObject::set_array "
MongoObject::set_array";

%feature("docstring") MongoObject::get_json "
MongoObject::get_json";

%feature("docstring") MongoObject::get_json_of_key "
MongoObject::get_json_of_key";

%feature("docstring") MongoObject::show "
MongoObject::show";

%feature("docstring") MongoObject::get_instances "
MongoObject::get_instances";

// File: class_node.xml


%feature("docstring") Node "

Represents a computational node with ports and callbacks.  

The `Node` class is derived from `MongoObject` and allows interaction with input
and output ports, evaluation of node functionality, and database communication.
A callback mechanism is provided via `NodeCallback`.  

C++ includes: CNode.h
";

/*
 Constructor & Destructor 
*/

/*
Constructs a new `Node` object.  

Parameters
----------
* `name` :  
    The name of the node (optional, default = \"\").  
* `ports` :  
    Map of port names to `Port` objects (optional).  
* `callback_class` :  
    Shared pointer to a `NodeCallback` object (optional).  

*/

/*
 Methods 
*/

/*
Reads the node's data from the database by its object ID.  

Parameters
----------
* `oid_string` :  
    The object ID (OID) of the node in the database.  

Returns
-------
`true` if the operation succeeds, `false` otherwise.  

*/

/*
 Getter Methods 
*/

/*
Retrieves a BSON representation of the node.  

Returns
-------
A BSON object representing the node's properties.  

*/

/*
 Setter Methods 
*/

/*
Configures all ports of a node.  

Parameters
----------
* `ports` :  
    Map of port names to their respective `Port` objects.  

*/

%feature("docstring") Node::Node "
Node::Node";

%feature("docstring") Node::~Node "
Node::~Node
Destroys the `Node` object.  
";

%feature("docstring") Node::read_from_db "
Node::read_from_db
Read the content of an existing BSON document into the current object  

Parameters
----------
* `oid_string` :  
    Object identifier of the queried document in the DB  

Returns
-------
True if successful otherwise false  
";

%feature("docstring") Node::evaluate "
Node::evaluate
Performs evaluation of the node.  
";

%feature("docstring") Node::is_valid "
Node::is_valid
Checks whether the node is valid.  

Returns
-------
`true` if the node is valid, otherwise `false`.  
";

%feature("docstring") Node::inputs_valid "
Node::inputs_valid
Checks the validity of the node's input ports.  

Returns
-------
`true` if all input ports are valid, otherwise `false`.  
";

%feature("docstring") Node::write_to_db "
Node::write_to_db
Writes the node's data to the database.  

Returns
-------
`true` if the operation succeeds, `false` otherwise.  
";

%feature("docstring") Node::get_bson "
Node::get_bson";

%feature("docstring") Node::get_name "
Node::get_name
Retrieves the name of the node.  

Returns
-------
A string containing the node's name.  
";

%feature("docstring") Node::get_input_ports "
Node::get_input_ports
Gets all input ports of the node.  

Returns
-------
A map containing input ports with their names as keys.  
";

%feature("docstring") Node::get_output_ports "
Node::get_output_ports
Gets all output ports of the node.  

Returns
-------
A map containing output ports with their names as keys.  
";

%feature("docstring") Node::get_ports "
Node::get_ports
Gets all ports of the node (both input and output).  

Returns
-------
A map containing all ports with their names as keys.  
";

%feature("docstring") Node::get_port "
Node::get_port
Retrieves a port by its name.  

Parameters
----------
* `port_name` :  
    The name of the port to retrieve.  

Returns
-------
A pointer to the requested `Port` object.  
";

%feature("docstring") Node::get_input_port "
Node::get_input_port
Retrieves an input port by its name.  

Parameters
----------
* `port_name` :  
    The name of the input port.  

Returns
-------
A pointer to the requested input `Port` object.  
";

%feature("docstring") Node::get_output_port "
Node::get_output_port
Retrieves an output port by its name.  

Parameters
----------
* `port_name` :  
    The name of the output port.  

Returns
-------
A pointer to the requested output `Port` object.  
";

%feature("docstring") Node::set_ports "
Node::set_ports";

%feature("docstring") Node::add_port "
Node::add_port
Adds a port to the node.  

Parameters
----------
* `key` :  
    The name of the port.  
* `port` :  
    A shared pointer to the `Port` object.  
* `is_source` :  
    Indicates whether the port is a source port.  
* `fill_in_out` :  
    Determines whether input/output lookups should be updated.  
";

%feature("docstring") Node::add_input_port "
Node::add_input_port
Adds an input port to the node.  

Parameters
----------
* `key` :  
    The name of the input port.  
* `port` :  
    A shared pointer to the input `Port` object.  
";

%feature("docstring") Node::add_output_port "
Node::add_output_port
Adds an output port to the node.  

Parameters
----------
* `key` :  
    The name of the output port.  
* `port` :  
    A shared pointer to the output `Port` object.  
";

%feature("docstring") Node::set_callback "
Node::set_callback
Sets the callback for the node.  

Parameters
----------
* `callback` :  
    The callback's name as a string.  
* `callback_type` :  
    The type of the callback as a string.  
";

%feature("docstring") Node::set_callback "
Node::set_callback
Sets the callback of the node using a `NodeCallback` object.  

Parameters
----------
* `cb` :  
    Shared pointer to the `NodeCallback` object.  
";

%feature("docstring") Node::set_valid "
Node::set_valid
Sets the validity of the node.  

Parameters
----------
* `is_valid` :  
    A boolean indicating if the node is valid.  
";

// File: class_node_callback.xml


%feature("docstring") NodeCallback "

C++ includes: NodeCallback.h
";

%feature("docstring") NodeCallback::run "
NodeCallback::run";

%feature("docstring") NodeCallback::NodeCallback "
NodeCallback::NodeCallback";

%feature("docstring") NodeCallback::~NodeCallback "
NodeCallback::~NodeCallback";

// File: classbsoncxx_1_1oid.xml

// File: class_port.xml


%feature("docstring") Port "

C++ includes: Port.h
";

%feature("docstring") Port::current_size "
Port::current_size";

%feature("docstring") Port::get_ptr "
Port::get_ptr";

%feature("docstring") Port::~Port "
Port::~Port";

%feature("docstring") Port::Port "
Port::Port";

%feature("docstring") Port::set_fixed "
Port::set_fixed";

%feature("docstring") Port::is_fixed "
Port::is_fixed";

%feature("docstring") Port::set_port_type "
Port::set_port_type";

%feature("docstring") Port::is_output "
Port::is_output";

%feature("docstring") Port::set_reactive "
Port::set_reactive";

%feature("docstring") Port::is_reactive "
Port::is_reactive";

%feature("docstring") Port::set_bounded "
Port::set_bounded";

%feature("docstring") Port::is_bounded "
Port::is_bounded";

%feature("docstring") Port::set_value_type "
Port::set_value_type";

%feature("docstring") Port::get_value_type "
Port::get_value_type";

%feature("docstring") Port::set_node "
Port::set_node";

%feature("docstring") Port::get_node "
Port::get_node";

%feature("docstring") Port::set_link "
Port::set_link";

%feature("docstring") Port::is_linked "
Port::is_linked";

%feature("docstring") Port::get_link "
Port::get_link";

%feature("docstring") Port::unlink "
Port::unlink";

%feature("docstring") Port::bound_is_valid "
Port::bound_is_valid";

%feature("docstring") Port::set_bounds "
Port::set_bounds";

%feature("docstring") Port::get_bounds "
Port::get_bounds";

%feature("docstring") Port::is_float "
Port::is_float";

%feature("docstring") Port::get_bytes "
Port::get_bytes";

%feature("docstring") Port::set_bytes "
Port::set_bytes";

%feature("docstring") Port::set_buffer_ptr "
Port::set_buffer_ptr";

%feature("docstring") Port::get_buffer_ptr "
Port::get_buffer_ptr";

%feature("docstring") Port::get_linked_ports "
Port::get_linked_ports";

%feature("docstring") Port::set_value "
Port::set_value";

%feature("docstring") Port::get_value "
Port::get_value";

%feature("docstring") Port::get_own_value "
Port::get_own_value";

%feature("docstring") Port::set_value_vector "
Port::set_value_vector";

%feature("docstring") Port::get_value_vector "
Port::get_value_vector";

%feature("docstring") Port::get_bson "
Port::get_bson";

%feature("docstring") Port::update_buffer "
Port::update_buffer";

%feature("docstring") Port::get_buffer "
Port::get_buffer";

%feature("docstring") Port::update_attached_node "
Port::update_attached_node";

%feature("docstring") Port::write_to_db "
Port::write_to_db";

%feature("docstring") Port::read_from_db "
Port::read_from_db
Read the content of an existing BSON document into the current object  

Parameters
----------
* `oid_string` :  
    Object identifier of the queried document in the DB  

Returns
-------
True if successful otherwise false  
";

// File: class_session.xml


%feature("docstring") Session "

C++ includes: Session.h
";

%feature("docstring") Session::Session "
Session::Session";

%feature("docstring") Session::Session "
Session::Session";

%feature("docstring") Session::add_node "
Session::add_node";

%feature("docstring") Session::get_nodes "
Session::get_nodes";

%feature("docstring") Session::write_to_db "
Session::write_to_db";

%feature("docstring") Session::read_from_db "
Session::read_from_db
Read the content of an existing BSON document into the current object  

Parameters
----------
* `oid_string` :  
    Object identifier of the queried document in the DB  

Returns
-------
True if successful otherwise false  
";

%feature("docstring") Session::create_port "
Session::create_port";

%feature("docstring") Session::create_port "
Session::create_port";

%feature("docstring") Session::create_node "
Session::create_node";

%feature("docstring") Session::create_node "
Session::create_node";

%feature("docstring") Session::read_session_template "
Session::read_session_template";

%feature("docstring") Session::get_session_template "
Session::get_session_template";

%feature("docstring") Session::link_nodes "
Session::link_nodes";

// File: classstd_1_1string.xml

// File: namespace_fluorescence_decay.xml

// File: namespace_functions.xml

%feature("docstring") Functions::shift "
Functions::shift
Shifts a vector by a floating number.  

This function shifts the y-axis and consider non-integer values by determining
the integer part of the shift and the floating part of the shift value, e.g.,
for the shift 4.66 the integer part is 4 and the floating part is 0.66. Next the
array is shifted by the integer part and a copy of the array is shifted by the
integer part + 1. Finally, the weighted sum of the both arrays is calculated.  

Parameters
----------
* `value` :  
* `x` :  
";

%feature("docstring") Functions::roll "
Functions::roll
Rolls a vector by an integer  

Parameters
----------
* `value` :  
* `y` :  
";

%feature("docstring") Functions::copy_vector_to_array "
Functions::copy_vector_to_array
Allocates memory on a pointer to an array and copies the content of a vector to
the newly allocated memory.  

This function is mainly used for numpy array SWIGs  

Parameters
----------
* `v` :  
* `out` :  
* `nout` :  
";

%feature("docstring") Functions::copy_vector_to_array "
Functions::copy_vector_to_array
Copy an array and copies the content of a vector to already allocated memory.  

This function is mainly used for numpy array SWIGs  

Parameters
----------
* `v` :  
* `out` :  
* `nout` :  
";

%feature("docstring") Functions::copy_array_to_vector "
Functions::copy_array_to_vector
Parameters
----------
* `in` :  
* `nin` :  
* `v` :  
";

%feature("docstring") Functions::copy_two_vectors_to_interleaved_array "
Functions::copy_two_vectors_to_interleaved_array
This function copies two vectors of equal size to an interleaved array  

Parameters
----------
* `v1` :  
* `v2` :  
* `out` :  
* `nout` :  
";

%feature("docstring") Functions::value2internal "
Functions::value2internal
Maps the array of values  

Parameters
----------
* `in` :  
    place to values to the interval (min, max) determined by the parameters  
* `bound_1` :  
    and  
* `bound_2.` :  
    The values are mapped to the interval by:  
$max(bound_1, bound_2) - abs(bound_1-bound_2)/(exp(value /
abs(bound_1-bound_2))+1)
$  

templateparam
-------------
* `T` :  
    The type of the values  

Parameters
----------
* `values` :  
    (InOut)  
* `n_values` :  
    (In)  
* `lower_bound` :  
    (In)  
* `upper_bound` :  
    (In)  
";

%feature("docstring") Functions::internal2value "
Functions::internal2value";

%feature("docstring") Functions::bound_values "
Functions::bound_values";

%feature("docstring") Functions::convolve_sum_of_exponentials "
Functions::convolve_sum_of_exponentials
This function convolves a sum of exponential decays with an instrument response
function (IRF)  

The sum of exponential decays is passed by the lifetime array  

Parameters
----------
* `lifetime_spectrum` :  
    that contains the amplitudes and the corresponding fluorescence lifetimes in
    an interleaved array (amplitude_1, lifetime_1, amplitude_2, lifetime_2,
    ...). The instrument response function is passed by the array  
* `irf.` :  
    The IRF and the and the sum of exponential decays are convolved up to the
    index  
* `convolution_stop.` :  
    In this convolution, the time axis is uniform an linear with a time interval
    specified by the parameter  
* `dt.` :  
* `out` :  
    The array to which the convoluted decay is written to  
* `n_out` :  
    The number of elements in the output array  
* `lifetime_spectrum` :  
    The array containing the fluorescence lifetimes and the amplitudes  
* `n_lifetime_spectrum` :  
    The number of fluorescence lifetimes  
* `irf` :  
    The array containing the instrument response function  
* `n_irf` :  
    The number of elements of the instrument response function  
* `convolution_stop` :  
    The stop of the convolution  
* `dt` :  
    The time resolution  
";

%feature("docstring") Functions::convolve_sum_of_exponentials_periodic "
Functions::convolve_sum_of_exponentials_periodic
This function convolves a sum of exponential decays with an instrument response
function (IRF)  

The sum of exponential decays is passed by the lifetime array  

Parameters
----------
* `lifetime` :  
    that contains the amplitudes and the corresponding fluorescence lifetimes in
    an interleaved array (amplitude_1, lifetime_1, amplitude_2, lifetime_2,
    ...). The instrument response function is passed by the array  
* `irf.` :  
    The IRF and the and the sum of exponential decays are convolved up to the
    index  
* `convolution_stop.` :  
    In this convolution, the time axis is uniform an linear with a time interval
    specified by the parameter  
* `dt.` :  
* `out` :  
    The array to which the convoluted decay is written to  
* `n_out` :  
    The number of elements in the output array  
* `lifetime` :  
    The array containing the fluorescence lifetimes and the amplitudes  
* `n_lifetimes` :  
    The number of fluorescence lifetimes  
* `irf` :  
    The array containing the instrument response function  
* `n_irf` :  
    The number of elements of the instrument response function  
* `start` :  
    The start index of the convolution  
* `stop` :  
    The stop index of the convolution  
* `dt` :  
    The time resolution  
* `period` :  
    The time between repeated excitation intervals  
";

%feature("docstring") Functions::diff "
Functions::diff
Calculates the discrete difference for an vector  

Parameters
----------
* `v` :  

Returns
-------  
";

%feature("docstring") Functions::get_time "
Functions::get_time
Returns the current time in milliseconds as a uint64  

Returns
-------  
";

%feature("docstring") Functions::add_documents "
Functions::add_documents
Adds the content in the bsoncxx document src to the document dst omitting the
keys provided by the vector skip.  
";

%feature("docstring") Functions::bson_element_skip "
Functions::bson_element_skip
Returns true if the key associated to  

Parameters
----------
* `element` :  
    is in the list of vectors  
* `skip` :  
* `element` :  
    reference to a bsoncxx::document::element  
* `skip` :  
    vector of strings containing keys that are skipped  
";

%feature("docstring") Functions::get_vector_of_min_size "
Functions::get_vector_of_min_size
Returns a vector with a size that is is min(a.size(), b.size())  

templateparam
-------------
* `T` :  
    The type of the vectors  

Parameters
----------
* `a` :  
* `b` :  

Returns
-------
a vector of size min(a.size(), b.size())  
";

// File: namespacestd.xml

// File: _c_node_8h.xml

// File: _functions_8h.xml

// File: info_8h.xml

// File: _mongo_object_8h.xml

// File: _node_callback_8h.xml

// File: _port_8h.xml

// File: _session_8h.xml

// File: _r_e_a_d_m_e_8md.xml

// File: dir_d44c64559bbebec7f509842c48db8b23.xml

