echo '{"location": {"latitude":-14.6073866,"longitude":-38.3795773}}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users/user1@hotmail.com/location
