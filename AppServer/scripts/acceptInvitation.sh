echo '{"email":"js5810@hotmail.com"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users/juanPerez2@hotmail.com/invitation

