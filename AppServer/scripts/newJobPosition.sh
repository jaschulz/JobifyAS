echo '{"name": "Principal","description": "Principal of an education institution."}' | \
curl -X POST -H 'Content-type: application/json' \
-H 'Accept: application/json' -d @- http://localhost:8081/api/job_positions/categories/education

