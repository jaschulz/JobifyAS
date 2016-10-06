echo ' ' | \
curl -X GET -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users/javschulz@gmail.com/contacts
