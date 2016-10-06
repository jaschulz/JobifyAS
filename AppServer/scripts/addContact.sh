echo '{"email":"javschulz@gmail.com"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users/js5810@hotmail.com/contacts

