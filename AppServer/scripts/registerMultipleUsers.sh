echo '{"email":"user1@hotmail.com","password":"123456"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users &
echo '{"email":"user2@hotmail.com","password":"123456"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users &
echo '{"email":"user3@hotmail.com","password":"123456"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users &
echo '{"email":"user4@hotmail.com","password":"123456"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users &
echo '{"email":"user5@hotmail.com","password":"123456"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users &
echo '{"email":"user6@hotmail.com","password":"123456"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/users
