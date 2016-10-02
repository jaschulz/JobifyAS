echo '{"email":"johndoe20160930003423@gmail.com","password":"Berlin"}' | \
curl -X GET -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/session
