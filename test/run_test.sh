mkdir db
mongod --dbpath=db &
nosetests
