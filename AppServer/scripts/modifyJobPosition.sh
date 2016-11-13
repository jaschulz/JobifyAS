echo '{"name": "Principal","description": "the Principal of an education institution.","category": "education"}' | \
curl -X PUT -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/job_positions/categories/education/Principal

