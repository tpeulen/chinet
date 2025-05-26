#include "MemoryObject.h"
#include <ctime>

// Initialize static members
std::list<std::shared_ptr<MemoryObject>> MemoryObject::registered_objects = std::list<std::shared_ptr<MemoryObject>>();
std::unordered_map<std::string, json> MemoryObject::object_store = std::unordered_map<std::string, json>();
std::mutex MemoryObject::object_store_mutex;

MemoryObject::MemoryObject(std::string name) :
    object_name(name),
    document(json::object()),
    time_of_death(0) {

    // Generate a unique OID for this object
    oid_document = generate_oid();

    // Initialize the document with basic fields
    document["_id"] = oid_document;
    document["name"] = name;

    std::clog << "NEW MEMORYOBJECT" << std::endl;
}

MemoryObject::~MemoryObject() {
    std::clog << "DESTROYING MEMORYOBJECT" << std::endl;

    // Remove this object from the registered objects list
    // Don't use shared_from_this() in the destructor as it can throw std::bad_weak_ptr
    unregister_instance(nullptr);

    // Set time of death
    time_of_death = std::time(nullptr);
}

bool MemoryObject::connect_to_db(
        const std::string& uri_string,
        const std::string& db_string,
        const std::string& app_string,
        const std::string& collection_string) {

    // In the memory implementation, we don't actually connect to a database
    // but we'll set the flag to indicate we're "connected"
    is_connected_to_db_ = true;

    // Register this instance
    try {
        register_instance(shared_from_this());
    } catch (const std::bad_weak_ptr&) {
        // If shared_from_this() fails, log a warning but continue
        // This can happen if the object wasn't created with make_shared
        std::clog << "Warning: Could not register instance in connect_to_db (shared_from_this() failed)" << std::endl;
    }

    return true;
}

void MemoryObject::disconnect_from_db() {
    is_connected_to_db_ = false;
}

bool MemoryObject::is_connected_to_db() {
    return is_connected_to_db_;
}

void MemoryObject::register_instance(std::shared_ptr<MemoryObject> x) {
    if (x == nullptr) {
        // If x is null, try to use shared_from_this() to get a valid shared_ptr
        try {
            x = shared_from_this();
        } catch (const std::bad_weak_ptr&) {
            // If shared_from_this() fails, the object is already being destroyed
            // and we can't get a valid shared_ptr, so just return
            std::clog << "Warning: Could not register instance (null shared_ptr and shared_from_this() failed)" << std::endl;
            return;
        }
    }

    // Only add the object if it's not already in the list
    if (std::find(registered_objects.begin(), registered_objects.end(), x) == registered_objects.end()) {
        registered_objects.push_back(x);
    }

    std::clog << "-- Total number of MemoryObject instances: " << registered_objects.size() << std::endl;
}

void MemoryObject::unregister_instance(std::shared_ptr<MemoryObject> x) {
    if (x) {
        registered_objects.remove(x);
    } else {
        // If x is null, try to use shared_from_this() to get a valid shared_ptr
        try {
            registered_objects.remove(shared_from_this());
        } catch (const std::bad_weak_ptr&) {
            // If shared_from_this() fails, the object is already being destroyed
            // and we can't get a valid shared_ptr, so just ignore it
            std::clog << "Warning: Could not unregister instance (null shared_ptr and shared_from_this() failed)" << std::endl;
        }
    }
}

std::list<std::shared_ptr<MemoryObject>> MemoryObject::get_instances() {
    return registered_objects;
}

bool MemoryObject::write_to_db() {
    if (!is_connected_to_db_) {
        std::cerr << "Error: Not connected to database" << std::endl;
        return false;
    }

    // Store the document in the object store
    std::lock_guard<std::mutex> lock(object_store_mutex);
    object_store[oid_document] = document;

    return true;
}

std::string MemoryObject::create_copy_in_db() {
    // Create a new OID for the copy
    std::string new_oid = generate_oid();

    // Create a copy of the document with the new OID
    json copy = document;
    copy["_id"] = new_oid;

    // Store the copy in the object store
    std::lock_guard<std::mutex> lock(object_store_mutex);
    object_store[new_oid] = copy;

    return new_oid;
}

bool MemoryObject::read_from_db(const std::string& oid_string) {
    if (!is_connected_to_db_) {
        std::cerr << "Error: Not connected to database" << std::endl;
        return false;
    }

    // Look up the document in the object store
    std::lock_guard<std::mutex> lock(object_store_mutex);
    auto it = object_store.find(oid_string);
    if (it == object_store.end()) {
        std::cerr << "Error: Document not found with OID " << oid_string << std::endl;
        return false;
    }

    // Copy the document
    document = it->second;
    oid_document = oid_string;

    // Update the object name if it exists in the document
    if (document.contains("name") && document["name"].is_string()) {
        object_name = document["name"].get<std::string>();
    }

    return true;
}

bool MemoryObject::read_json(std::string json_string) {
    try {
        document = json::parse(json_string);

        // Update the OID and name from the document
        if (document.contains("_id") && document["_id"].is_string()) {
            oid_document = document["_id"].get<std::string>();
        }

        if (document.contains("name") && document["name"].is_string()) {
            object_name = document["name"].get<std::string>();
        }

        return true;
    } catch (const json::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return false;
    }
}

std::shared_ptr<MemoryObject> MemoryObject::get_ptr() {
    try {
        return shared_from_this();
    } catch (const std::bad_weak_ptr&) {
        // If shared_from_this() fails, log a warning and return nullptr
        std::clog << "Warning: get_ptr() failed (shared_from_this() threw bad_weak_ptr)" << std::endl;
        return nullptr;
    }
}

std::string MemoryObject::get_json(int indent) {
    return document.dump(indent);
}

std::string MemoryObject::get_json_of_key(std::string key) {
    if (document.contains(key)) {
        return document[key].dump();
    }
    return "{}";
}

std::shared_ptr<MemoryObject> MemoryObject::operator[](std::string key) {
    // This is a placeholder implementation
    // In a real implementation, this would return a child object
    return nullptr;
}
