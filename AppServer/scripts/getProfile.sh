echo '{"email":"javschulz@gmail.com","first_name":"Javier","last_name":"Schulz"}' | \
curl -X GET -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users/javschulz@gmail.com