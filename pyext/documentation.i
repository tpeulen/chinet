
// File: index.xml

// File: classbson__oid__t.xml

// File: classbson__oid__t.xml

// File: classbson__t.xml

// File: classbson__t.xml

// File: class_curve.xml


%feature("docstring") Curve "

This method multiplies to the y-values of the current instance of a object the
y-values of another  

Parameters
----------
* `v` :  
    An object of the type  
* `valid` :  
    If this parameter is false (default value) the y-values are directly
    multiplied without check for overlap. If this parameter is true only the
    overlapping y-values are multiplied. The overlap is judged by the values of
    the x-axis (TODO).  

C++ includes: Curve.h
";

%feature("docstring") Curve::set_x "
";

%feature("docstring") Curve::set_x "
";

%feature("docstring") Curve::set_ex "
";

%feature("docstring") Curve::set_x_name "
";

%feature("docstring") Curve::set_y "
";

%feature("docstring") Curve::set_y "
";

%feature("docstring") Curve::set_ey "
";

%feature("docstring") Curve::set_y_name "
";

%feature("docstring") Curve::get_x "
";

%feature("docstring") Curve::get_x "
";

%feature("docstring") Curve::get_y "
";

%feature("docstring") Curve::get_y "
";

%feature("docstring") Curve::sum "
";

%feature("docstring") Curve::size "
";

%feature("docstring") Curve::get_dx "

Calculates the difference between the x-Axis values  

Returns
-------  
";

%feature("docstring") Curve::get_dy "

Calculates the difference between the y-Axis values  

Returns
-------  
";

%feature("docstring") Curve::Curve "
";

%feature("docstring") Curve::Curve "
";

%feature("docstring") Curve::Curve "
";

%feature("docstring") Curve::Curve "
";

%feature("docstring") Curve::Curve "
";

%feature("docstring") Curve::Curve "
";

%feature("docstring") Curve::~Curve "
";

%feature("docstring") Curve::shift "

Shifts the x Axis by a floating point number.  

Parameters
----------
* `value` :  
";

%feature("docstring") Curve::resize "
";

%feature("docstring") Curve::to_json "
";

%feature("docstring") Curve::from_json "
";

// File: class_port_1_1get__value__vector.xml

// File: class_mongo_object.xml


%feature("docstring") MongoObject "

The object identification number of the.  

Disconnects the.  

Connects the instance of.  

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
a bson_t document  

database  

the DB  

Returns true if the instance of the to the DB  

Writes connected MongoDB  

Returns
-------  

C++ includes: MongoObject.h
";

%feature("docstring") MongoObject::MongoObject "
";

%feature("docstring") MongoObject::~MongoObject "
";

%feature("docstring") MongoObject::connect_to_db "

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
";

%feature("docstring") MongoObject::is_connected_to_db "
";

%feature("docstring") MongoObject::register_instance "
";

%feature("docstring") MongoObject::unregister_instance "
";

%feature("docstring") MongoObject::write_to_db "
";

%feature("docstring") MongoObject::create_copy_in_db "

Creates a copy of the object in the connected MongoDB with a new OID.  

Returns
-------
The OID of the copy  
";

%feature("docstring") MongoObject::read_from_db "

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

Read the content of a JSON string into a MongoObject  

Parameters
----------
* `json_string` :  

Returns
-------  
";

%feature("docstring") MongoObject::get_own_oid "

The own object identifier  

Returns
-------  
";

%feature("docstring") MongoObject::set_own_oid "

Set the own object identifier without duplicate check  

Parameters
----------
* `oid_str` :  
";

%feature("docstring") MongoObject::set_name "
";

%feature("docstring") MongoObject::get_name "
";

%feature("docstring") MongoObject::get_ptr "
";

%feature("docstring") MongoObject::set_string "

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
";

%feature("docstring") MongoObject::get_singleton "
";

%feature("docstring") MongoObject::set_singleton "
";

%feature("docstring") MongoObject::set_oid "
";

%feature("docstring") MongoObject::get_array "
";

%feature("docstring") MongoObject::set_array "
";

%feature("docstring") MongoObject::get_json "
";

%feature("docstring") MongoObject::get_json_of_key "
";

%feature("docstring") MongoObject::show "
";

%feature("docstring") MongoObject::get_instances "
";

// File: class_node.xml


%feature("docstring") Node "

C++ includes: CNode.h
";

%feature("docstring") Node::Node "
";

%feature("docstring") Node::~Node "
";

%feature("docstring") Node::read_from_db "

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
";

%feature("docstring") Node::is_valid "
";

%feature("docstring") Node::inputs_valid "
";

%feature("docstring") Node::write_to_db "
";

%feature("docstring") Node::get_bson "
";

%feature("docstring") Node::get_name "
";

%feature("docstring") Node::get_input_ports "
";

%feature("docstring") Node::get_output_ports "
";

%feature("docstring") Node::get_ports "
";

%feature("docstring") Node::set_ports "
";

%feature("docstring") Node::get_port "
";

%feature("docstring") Node::add_port "
";

%feature("docstring") Node::add_input_port "
";

%feature("docstring") Node::add_output_port "
";

%feature("docstring") Node::get_input_port "
";

%feature("docstring") Node::get_output_port "
";

%feature("docstring") Node::set_callback "
";

%feature("docstring") Node::set_callback "
";

%feature("docstring") Node::set_valid "
";

// File: class_node_callback.xml


%feature("docstring") NodeCallback "

C++ includes: NodeCallback.h
";

%feature("docstring") NodeCallback::run "
";

%feature("docstring") NodeCallback::NodeCallback "
";

%feature("docstring") NodeCallback::~NodeCallback "
";

// File: class_port.xml


%feature("docstring") Port "

C++ includes: Port.h
";

%feature("docstring") Port::getptr "
";

%feature("docstring") Port::current_size "
";

%feature("docstring") Port::~Port "
";

%feature("docstring") Port::Port "
";

%feature("docstring") Port::set_node "
";

%feature("docstring") Port::get_node "
";

%feature("docstring") Port::get_value_type "
";

%feature("docstring") Port::set_value_type "
";

%feature("docstring") Port::set_value "
";

%feature("docstring") Port::update_buffer "
";

%feature("docstring") Port::get_own_value "
";

%feature("docstring") Port::get_value "
";

%feature("docstring") Port::get_bson "
";

%feature("docstring") Port::update_attached_node "
";

%feature("docstring") Port::set_bounded "
";

%feature("docstring") Port::bound_is_valid "
";

%feature("docstring") Port::is_bounded "
";

%feature("docstring") Port::set_bounds "
";

%feature("docstring") Port::get_bounds "
";

%feature("docstring") Port::is_fixed "
";

%feature("docstring") Port::set_fixed "
";

%feature("docstring") Port::is_output "
";

%feature("docstring") Port::set_port_type "
";

%feature("docstring") Port::is_reactive "
";

%feature("docstring") Port::is_float "
";

%feature("docstring") Port::set_reactive "
";

%feature("docstring") Port::write_to_db "
";

%feature("docstring") Port::read_from_db "

Read the content of an existing BSON document into the current object  

Parameters
----------
* `oid_string` :  
    Object identifier of the queried document in the DB  

Returns
-------
True if successful otherwise false  
";

%feature("docstring") Port::get_bytes "
";

%feature("docstring") Port::set_bytes "
";

%feature("docstring") Port::set_buffer_ptr "
";

%feature("docstring") Port::get_buffer_ptr "
";

%feature("docstring") Port::set_link "
";

%feature("docstring") Port::unlink "
";

%feature("docstring") Port::is_linked "
";

%feature("docstring") Port::get_linked_ports "
";

%feature("docstring") Port::get_link "
";

// File: class_session.xml


%feature("docstring") Session "

C++ includes: Session.h
";

%feature("docstring") Session::Session "
";

%feature("docstring") Session::Session "
";

%feature("docstring") Session::add_node "
";

%feature("docstring") Session::get_nodes "
";

%feature("docstring") Session::write_to_db "
";

%feature("docstring") Session::read_from_db "

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
";

%feature("docstring") Session::create_port "
";

%feature("docstring") Session::create_node "
";

%feature("docstring") Session::create_node "
";

%feature("docstring") Session::read_session_template "
";

%feature("docstring") Session::get_session_template "
";

%feature("docstring") Session::link_nodes "
";

// File: classstd_1_1string.xml

// File: classstd_1_1string.xml

// File: namespace_fluorescence_decay.xml

// File: namespace_functions.xml

%feature("docstring") Functions::shift "

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

Rolls a vector by an integer  

Parameters
----------
* `value` :  
* `y` :  
";

%feature("docstring") Functions::copy_vector_to_array "

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

Copy an array and copies the content of a vector to already allocated memory.  

This function is mainly used for numpy array SWIGs  

Parameters
----------
* `v` :  
* `out` :  
* `nout` :  
";

%feature("docstring") Functions::copy_array_to_vector "

Parameters
----------
* `in` :  
* `nin` :  
* `v` :  
";

%feature("docstring") Functions::copy_two_vectors_to_interleaved_array "

This function copies two vectors of equal size to an interleaved array  

Parameters
----------
* `v1` :  
* `v2` :  
* `out` :  
* `nout` :  
";

%feature("docstring") Functions::value2internal "

Maps the array of values  

Parameters
----------
* `in` :  
    place to values to the interval (min, max) determined by the parameters  
* `bound_1` :  
    and  
* `bound_2.` :  
    The values are mapped to the interval by:  
$ max(bound_1, bound_2) - abs(bound_1-bound_2)/(exp(value /
abs(bound_1-bound_2))+1) $  

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
";

%feature("docstring") Functions::bound_values "
";

%feature("docstring") Functions::convolve_sum_of_exponentials "

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

Calculates the discrete difference for an vector  

Parameters
----------
* `v` :  

Returns
-------  
";

%feature("docstring") Functions::get_time "

Returns the current time in milliseconds as a uint64  

Returns
-------  
";

%feature("docstring") Functions::add_documents "

Adds the content in the bson_t document src to the document dst omitting the
keys provided by the vector skip.  
";

%feature("docstring") Functions::bson_iter_skip "

Returns true if the key associated to  

Parameters
----------
* `iter` :  
    is in the list of vectors  
* `skip` :  
* `iter` :  
    pointer to a bson_iter_t  
* `skip` :  
    vector of strings containing keys that are skipped by iter  
";

%feature("docstring") Functions::get_vector_of_min_size "

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

// File: _curve_8h.xml

// File: _c_node_8h.xml

// File: _functions_8h.xml

// File: _mongo_object_8h.xml

// File: _node_callback_8h.xml

// File: _port_8h.xml

// File: _session_8h.xml

// File: _r_e_a_d_m_e_8md.xml

// File: dir_361fb7613cd3bd0eab74c2d5be43de50.xml

// File: dir_d44c64559bbebec7f509842c48db8b23.xml

// File: indexpage.xml

