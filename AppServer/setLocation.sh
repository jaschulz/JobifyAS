echo '{"location": {"latitude":"-1","longitude":"-2"}}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users/javschulz@gmail.com/location
