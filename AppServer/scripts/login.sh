echo '{"email":"js5810@hotmail.com","password":"12345"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/session
