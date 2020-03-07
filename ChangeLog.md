# Changes

## 0.0.5

### 2020-03-07

* Automatic register and de-register MongoObjects

### 2020-03-06

* Improved access and representation of MongoObjects

* Registered MongoObjects. Initialized MongoObjects can be accesses 
  via the attribute MongoObject.get_instances()

* Support for initialization of Node instances using callback functions. 
  It is be possible to initialize a Node using `Node(callback=fun)` 
  where `fun` is a decorated python function.

### 2020-03-05

* Linking against IMP

