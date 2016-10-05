echo '{"email":"javschulz@gmail.com","first_name":"Javier","last_name":"Schulz","location":{"latitude" : "10","longitude" : "10"}}' | \
curl -X PUT -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users/javschulz@gmail.com
