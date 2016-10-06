echo '{"location": {"latitude":"-1","longitude":"-32"}}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users/js5810@hotmail.com/location
