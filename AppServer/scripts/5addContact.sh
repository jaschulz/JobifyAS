echo '{"email":"juanPerez2@hotmail.com"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users/js5810@hotmail.com/contacts

