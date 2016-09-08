echo '{"email":"locolope@gmail.com","password":"123456"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8080/api/registerUser
