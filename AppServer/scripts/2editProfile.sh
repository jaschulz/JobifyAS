echo '{"email":"js5810@hotmail.com","first_name":"Juan","last_name":"Sanchez","location":{"latitude" : 10,"longitude" : 10}}' | \
curl -X PUT -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users/js5810@hotmail.com
