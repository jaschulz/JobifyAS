echo '{"name": "PLSQL2","description": "Oracle DataBase"}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/skills/categories/software

