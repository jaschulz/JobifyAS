echo '{"name": "Vantive2","category": "software","description": "a Canned CRM software"}' | \
curl -X PUT -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/skills/categories/software/Vantive2

