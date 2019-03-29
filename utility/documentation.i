
// File: index.xml

// File: class_axis.xml


%feature("docstring") Axis "

C++ includes: Histogram.h
";

%feature("docstring") Axis::getNumberOfBins "
";

%feature("docstring") Axis::setAxisType "
";

%feature("docstring") Axis::Axis "
";

%feature("docstring") Axis::Axis "
";

%feature("docstring") Axis::getBins "
";

%feature("docstring") Axis::getBins "
";

%feature("docstring") Axis::setName "
";

%feature("docstring") Axis::getName "
";

%feature("docstring") Axis::getBinIdx "
";

%feature("docstring") Axis::update "

Recalculates the bin edges of the axis  
";

// File: unionbh__overflow__t.xml

// File: unionbh__spc130__record__t.xml

// File: unionbh__spc600__256__record__t.xml

// File: unionbh__spc600__4096__record__t.xml

// File: class_correlator.xml


%feature("docstring") Correlator "

C++ includes: correlate.h
";

%feature("docstring") Correlator::Correlator "
";

%feature("docstring") Correlator::set_n_bins "
";

%feature("docstring") Correlator::set_events "

Parameters
----------
* `Array` :  
    t1 of the time events of the first channel (the array is modified in place)  
* `n_t1` :  
    The number of time events in the first channel  
* `w1` :  
    A vector of weights for the time events of the first channel  
* `n_weights_ch1` :  
    The number of weights of the first channel  
* `t2` :  
    A vector of the time events of the second channel  
* `n_t2` :  
    The number of time events in the second channel  
* `w2` :  
    A vector of weights for the time events of the second channel  
* `n_weights_ch2` :  
    The number of weights of the second channel  
";

%feature("docstring") Correlator::get_n_bins "
";

%feature("docstring") Correlator::set_n_casc "

Sets the number of cascades of the correlation curve and updates the correlation
axis.  

Parameters
----------
* `n_casc` :  
";

%feature("docstring") Correlator::normalize "

Calculates the normalized correlation amplitudes and x-axis  

Makes a copy of the current correlation curve, i.e., the x-axis and and the
corresponding correlation amplitudes and calculates the values of the normalized
correlation.  
";

%feature("docstring") Correlator::~Correlator "
";

%feature("docstring") Correlator::make_fine "

Changes the  

Parameters
----------
* `tac_1` :  
* `n_tac_1` :  
* `tac_2` :  
* `n_tac_2` :  
";

%feature("docstring") Correlator::run "
";

%feature("docstring") Correlator::get_x_axis_normalized "

Get the normalized x-axis of the correlation  

Parameters
----------
* `x_axis` :  
    a pointer to an array that will contain the x-axis  
* `n_out` :  
    a pointer to the an integer that will contain the number of elements of the
    x-axis  
";

%feature("docstring") Correlator::get_x_axis "

Get the x-axis of the correlation  

Parameters
----------
* `x_axis` :  
    a pointer to an array that will contain the x-axis  
* `n_out` :  
    a pointer to the an integer that will contain the number of elements of the
    x-axis  
";

%feature("docstring") Correlator::get_corr_normalized "

Get the normalized correlation.  

Parameters
----------
* `corr` :  
    a pointer to an array that will contain the normalized correlation  
* `n_out` :  
    a pointer to the an integer that will contain the number of elements of the
    normalized x-axis  
";

%feature("docstring") Correlator::get_corr "

Get the correlation.  

Parameters
----------
* `corr` :  
    a pointer to an array that will contain the correlation  
* `n_out` :  
    a pointer to the an integer that will contain the number of elements of the
    x-axis  
";

%feature("docstring") Correlator::get_n_corr "
";

%feature("docstring") Correlator::get_n_casc "

Returns
-------  
";

// File: struct_curve_mapping__t.xml


%feature("docstring") CurveMapping_t "

C++ includes: Header.h
";

// File: class_header.xml


%feature("docstring") Header "

Constructor for the a file pointer and the container type of the file
represented by the file pointer. The container type refers either to a PicoQuant
(PQ) PTU or HT3 file, or a BeckerHickl (BH) spc file. There are three different
types of BH spc files SPC130, SPC600_256 (256 bins in micro time) or SPC600_4096
(4096 bins in micro time). PQ HT3 files may contain different TTTR record types
depending on the counting device (HydraHarp, PicoHarp) and firmware revision of
the counting device. Similarly, PTU files support a diverse set of TTTR records.  

Parameters
----------
* `fpin` :  
    the file pointer to the TTTR file  
* `tttr_container_type` :  
    the container type  

C++ includes: Header.h
";

%feature("docstring") Header::getTTTRRecordType "

Returns
-------
The TTTR container type of the associated TTTR file as a char  
";

%feature("docstring") Header::~Header "
";

%feature("docstring") Header::Header "

Default constructor  
";

%feature("docstring") Header::Header "
";

// File: class_histogram.xml


%feature("docstring") Histogram "

C++ includes: Histogram.h
";

%feature("docstring") Histogram::setAxis "
";

%feature("docstring") Histogram::setAxis "
";

%feature("docstring") Histogram::setData "
";

%feature("docstring") Histogram::getAxisDimensions "
";

%feature("docstring") Histogram::getHistogram "
";

%feature("docstring") Histogram::setWeights "
";

%feature("docstring") Histogram::Histogram "
";

%feature("docstring") Histogram::update "
";

%feature("docstring") Histogram::~Histogram "
";

%feature("docstring") Histogram::getAxis "
";

// File: struct_param_struct__t.xml


%feature("docstring") ParamStruct_t "

C++ includes: Header.h
";

// File: unionpq__hh__t2__record__t.xml

// File: unionpq__hh__t3__record__t.xml

// File: structpq__ht3__ascii__t.xml


%feature("docstring") pq_ht3_ascii_t "

The following represents the readable ASCII file header portion.  

C++ includes: Header.h
";

// File: structpq__ht3___bin_hdr__t.xml


%feature("docstring") pq_ht3_BinHdr_t "

The following is binary file header information.  

C++ includes: Header.h
";

// File: structpq__ht3__board__settings__t.xml


%feature("docstring") pq_ht3_board_settings_t "

C++ includes: Header.h
";

// File: structpq__ht3___board_hdr.xml


%feature("docstring") pq_ht3_BoardHdr "

C++ includes: Header.h
";

// File: structpq__ht3___t_t_t_r_hdr.xml


%feature("docstring") pq_ht3_TTTRHdr "

C++ includes: Header.h
";

// File: unionpq__ph__t2__record__t.xml

// File: unionpq__ph__t3__record__t.xml

// File: structtag__head__t.xml


%feature("docstring") tag_head_t "

A Header Tag entry of a PTU file.  

C++ includes: Header.h
";

// File: class_t_t_t_r.xml


%feature("docstring") TTTR "

C++ includes: TTTR.h
";

%feature("docstring") TTTR::get_micro_time "
";

%feature("docstring") TTTR::get_filename "

getFilename Getter for the filename of the TTTR file  

Returns
-------
The filename of the TTTR file  
";

%feature("docstring") TTTR::get_macro_time "
";

%feature("docstring") TTTR::get_n_valid_events "
";

%feature("docstring") TTTR::read_file "

Reads the TTTR data contained in a file into the TTTR object  

Parameters
----------
* `fn` :  
    The filename  
* `container_type` :  
    The container type.  

Returns
-------
Returns 1 in case the file was read without errors. Otherwise 0 is returned.  
";

%feature("docstring") TTTR::read_file "
";

%feature("docstring") TTTR::get_event_type "
";

%feature("docstring") TTTR::get_routing_channel "
";

%feature("docstring") TTTR::get_selection_by_channel "

Returns a vector containing indices of records that  

Parameters
----------
* `in` :  
    a pointer to an array of int16_tchannel numbers that are used to select
    indices of photons  
* `n_in` :  
    the length of the channel list.  
";

%feature("docstring") TTTR::~TTTR "

Destructor.  
";

%feature("docstring") TTTR::select "
";

%feature("docstring") TTTR::get_header "

Get header returns the header (if present) as a map of strings.  
";

%feature("docstring") TTTR::TTTR "

Constructor  

Parameters
----------
* `filename` :  
    is the filename of the TTTR file.  
* `container_type` :  
    specifies the file type. parent->children.push_back()  

PQ_PTU_CONTAINER 0 PQ_HT3_CONTAINER 1 BH_SPC130_CONTAINER 2
BH_SPC600_256_CONTAINER 3 BH_SPC600_4096_CONTAINER 4  
";

%feature("docstring") TTTR::TTTR "
";

%feature("docstring") TTTR::TTTR "
";

%feature("docstring") TTTR::TTTR "
";

%feature("docstring") TTTR::TTTR "
";

%feature("docstring") TTTR::TTTR "
";

%feature("docstring") TTTR::get_n_events "

Returns the number of events in the TTTR file for cases no selection is
specified otherwise the number of selected events is returned.  

Returns
-------  
";

// File: class_t_t_t_r_reader.xml


%feature("docstring") TTTRReader "

C++ includes: TTTRReader.h
";

%feature("docstring") TTTRReader::TTTRReader "

Constructor.  
";

%feature("docstring") TTTRReader::TTTRReader "

Parameters
----------
* `fn` :  
    the filename of the TTTR file  
";

%feature("docstring") TTTRReader::determine_file_container_type "

Determines based on the filename ending the file is a HT3 or PTU file.  
";

%feature("docstring") TTTRReader::getData "

Returns either a PQ or a BH class object (both are children from TTTR)  
";

// File: namespacestd.xml

// File: correlate_8h.xml

%feature("docstring") coarsen "

Coarsens the time events  

This function coarsens a set of time events by dividing the times by two. In
case two consecutive time events in the array have the same time, the weights of
the two events are added to the following weight element and the value of the
previous weight is set to zero.  

Parameters
----------
* `t` :  
    A vector of the time events of the first channel  
* `w` :  
    A vector of weights for the time events of the first channel  
* `nt` :  
    The number of time events in the first channel  
";

%feature("docstring") normalize_correlation "

Normalizes a correlation curve.  

This normalization applied to correlation curves that were calculated using a
linear/logrithmic binning as described in  

*   Fast calculation of fluorescence correlation data with asynchronous time-
    correlated single-photon counting, Michael Wahl, Ingo Gregor, Matthias
    Patting, Joerg Enderlein, Optics Express Vol. 11, No. 26, p. 3383  

Parameters
----------
* `np1` :  
    The sum of the weights in the first correlation channel  
* `dt1` :  
    The time difference between the first event and the last event in the first
    correlation channel  
* `np2` :  
    The sum of the weights in the second correlation channel  
* `dt2` :  
    The time difference between the first event and the last event in the second
    correlation channel  
* `x_axis` :  
    The x-axis of the correlation  
* `corr` :  
    The array that contains the original correlation that is modified in place.  
* `n_bins` :  
    The number of bins per cascade of the correlation  
";

%feature("docstring") correlate "

Calculates the cross-correlation between two arrays containing time events.  

Cross-correlates two weighted arrays of events using an approach that utilizes a
linear spacing of the bins within a cascade and a logarithmic spacing of the
cascades. The function works inplace on the input times, i.e, during the
correlation the values of the input times and weights are changed to coarsen the
times and weights for every correlation cascade.  

The start position parameters  

Parameters
----------
* `start_1` :  
    and  
* `start_2` :  
    and the end position parameters  
* `end_1` :  
    and  
* `end_1` :  
    define which part of the time array of the first and second correlation
    channel are used for the correlation analysis.  

The correlation algorithm combines approaches of the following papers:  

*   Fast calculation of fluorescence correlation data with asynchronous time-
    correlated single-photon counting, Michael Wahl, Ingo Gregor, Matthias
    Patting, Joerg Enderlein, Optics Express Vol. 11, No. 26, p. 3383  
*   Fast, flexible algorithm for calculating photon correlations, Ted A.
    Laurence, Samantha Fore, Thomas Huser, Optics Express Vol. 31 No. 6, p.829  

Parameters
----------
* `start_1` :  
    The start position on the time event array of the first channel.  
* `end_1` :  
    The end position on the time event array of the first channel.  
* `start_2` :  
    The start position on the time event array of the second channel.  
* `end_2` :  
    The end position on the time event array of the second channel.  
* `i_casc` :  
    The number of the current cascade  
* `n_bins` :  
    The number of bins per cascase  
* `taus` :  
    A vector containing the correlation times of all cascades  
* `corr` :  
    A vector to that the correlation is written by the function  
* `t1` :  
    A vector of the time events of the first channel  
* `w1` :  
    A vector of weights for the time events of the first channel  
* `nt1` :  
    The number of time events in the first channel  
* `t2` :  
    A vector of the time events of the second channel  
* `w2` :  
    A vector of weights for the time events of the second channel  
* `nt2` :  
    The number of time events in the second channel  
";

%feature("docstring") make_fine_times "

Changes the time events by adding the micro time to the macro time  

Changes the time events by adding the micro time to the macro time. The micro
times shoudl match the macro time, i.e., the length of the micro time array
should be the at least the same length as the macro time array.  

Parameters
----------
* `t` :  
    An array containing the time events (macro times)  
* `tac` :  
    An array containing the micro times of the corresponding macro times  
* `n_times` :  
    The number of macro times.  
";

// File: _header_8h.xml

%feature("docstring") read_ht3_header "

Reads the header of a ht3 file and sets the reading routing for  

Parameters
----------
* `fpin` :  
* `rewind` :  
* `tttr_record_type` :  
* `data` :  
* `macro_time_resolution` :  
* `micro_time_resolution` :  

Returns
-------
The position of the file pointer at the end of the header  
";

%feature("docstring") read_ptu_header "

Reads the header of a ptu file and sets the reading routing for  

Parameters
----------
* `fpin` :  
* `rewind` :  
* `tttr_record_type` :  
* `data` :  
* `macro_time_resolution` :  
* `micro_time_resolution` :  

Returns
-------
The position of the file pointer at the end of the header  
";

// File: _histogram_8h.xml

%feature("docstring") histogram1D "

templateparam
-------------
* `T` :  

Parameters
----------
* `data` :  
* `n_data` :  
* `weights` :  
* `n_weights` :  
* `bin_edges` :  
    contains the edges of the histogram in ascending order (from small to large)  
* `n_bins` :  
    the number of bins in the histogram  
* `hist` :  
* `n_hist` :  
* `axis_type` :  
* `use_weights` :  
    if true the weights specified by  
* `weights` :  
    are used for the calculation of the histogram instead of simply counting the
    frequency.  
";

%feature("docstring") logspace "
";

%feature("docstring") calc_bin_idx "

Calculates for a linear axis the bin index for a particular value.  

templateparam
-------------
* `T` :  

Parameters
----------
* `begin` :  
* `bin_width` :  
* `value` :  

Returns
-------  
";

%feature("docstring") linspace "
";

%feature("docstring") search_bin_idx "

Searches for the bin index of a value within a list of bin edges  

If a value is inside the bounds find the bin. The search partitions the
bin_edges in upper and lower ranges and adapts the edge for the upper and lower
range depending if the target value is bigger or smaller than the bin in the
middle.  

templateparam
-------------
* `T` :  

Parameters
----------
* `value` :  
* `bin_edges` :  
* `n_bins` :  

Returns
-------
negative value if the search value is out of the bounds. Otherwise the bin
number is returned.  
";

%feature("docstring") bincount1D "
";

// File: _record_reader_8h.xml

%feature("docstring") ProcessPHT2 "
";

%feature("docstring") ProcessPHT3 "
";

%feature("docstring") ProcessSPC600_256 "
";

%feature("docstring") ProcessHHT3v2 "
";

%feature("docstring") ProcessHHT3v1 "
";

%feature("docstring") ProcessHHT2v2 "
";

%feature("docstring") ProcessHHT2v1 "
";

%feature("docstring") ProcessSPC600_4096 "
";

%feature("docstring") ProcessSPC130 "
";

// File: _t_t_t_r_8h.xml

%feature("docstring") getIndicesByChannel "

Parameters
----------
* `out` :  
* `n_out` :  
* `in` :  
* `n_in` :  
* `routing_channels` :  
* `n_routing_channels` :  
";

%feature("docstring") selection_by_count_rate "

A count rate (cr) filter that returns an array containing a list of indices
where the cr was smaller than a specified cr.  

The filter is applied to a series of consecutive time events specified by the C
type array  

Parameters
----------
*  :  
";

%feature("docstring") get_ranges_channel "

Parameters
----------
* `ranges` :  
* `n_range` :  
* `time` :  
* `n_time` :  
* `channel` :  
";

%feature("docstring") get_array "
";

%feature("docstring") determine_number_of_records_by_file_size "

Determines the number of records in a TTTR files (for use with not HDF5)  

Calculates the number of records in the file based on the file size. if  

Parameters
----------
* `offset` :  
    is passed the number of records is calculated by the file size the number of
    bytes in the file - offset and  
* `bytes_per_record.` :  
    If  
* `offset` :  
    is not specified the current location of the file pointer is used as an
    offset. If  
* `bytes_per_record` :  
    is not specified the attribute value bytes_per_record of the class instance
    is used.  
* `offset` :  
* `bytes_per_record` :  
";

%feature("docstring") get_ranges_time_window "

Determines time windows for an array of consecutive time events based on  

The function determines for an array of consecutive time events passed to the
function by the argument  

Parameters
----------
* `time` :  
    an interleaved array  
* `time_windows` :  
    containing a list of time windows (tw). In the array  
* `time_windows` :  
    the beginning and the end of the tws are interleaved, e.g., for two time
    windows  

         [begin1, end1, begin2, end2]  

The returned beginnings and ends refer to the index of the photons in the array
of consecutive times  

Parameters
----------
* `time.` :  

The selection of the tws can be adjusted by the parameters  

Parameters
----------
* `tw_min` :  
";

// File: _t_t_t_r_reader_8h.xml

